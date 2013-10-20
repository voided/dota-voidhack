
#include "vh.h"

#include "entityhelper.h"
#include "convarhelper.h"

#include "scriptmanager.h"
#include "zeusmanager.h"
#include "cameramanager.h"
#include "fogmanager.h"

#include "tier1/interface.h"

#include "cbase.h"

#include "vstdlib/cvar.h"
#include "toolframework/ienginetool.h"
#include "cdll_int.h"
#include "igameevents.h"

#include "dotaplayer.h"
#include "dotahero.h"
#include "dotaresource.h"
#include "dotaability.h"
#include "dotagamerules.h"
#include "dotaitem.h"

#include <sourcehook/sourcehook.h>

#include <windows.h>


BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		VH().Init();
	}
	else if ( fdwReason == DLL_PROCESS_DETACH )
	{
		VH().Shutdown();
	}

	return TRUE;
}



CVH &VH()
{
	static CVH vh;
	return vh;
}


// required sourcehook boilerplate
SourceHook::Impl::CSourceHookImpl g_SourceHook;
SourceHook::ISourceHook *g_SHPtr = &g_SourceHook;
int g_PLID = 0;


SH_DECL_HOOK1_void( IBaseClientDLL, FrameStageNotify, SH_NOATTRIB, 0, ClientFrameStage_t );

// to support CGameEventListener
IGameEventManager2 *gameeventmanager = NULL;


CVH::CVH() :
	m_pEngineClient( NULL ), m_pClientDLL( NULL ),
	m_pEngineTool( NULL ), m_pClientTools( NULL ),
	m_pGameEventManager( NULL ), m_pFileSystem( NULL ),

	m_fnClientFactory( NULL ),
	m_fnEngineFactory( NULL ),
	m_fnCvarFactory( NULL ),
	m_fnFileSystemFactory( NULL )
{
}


CON_COMMAND( vh_test, "Test convar" )
{
	Msg( "In game: %d\n", VH().EngineClient()->IsInGame() );
	Msg( "Ent index: %d\n", VH().ClientTools()->GetEntIndex( VH().ClientTools()->GetLocalPlayer() ) );

	C_DOTAPlayer player = C_DOTAPlayer::GetLocalPlayer();

	if ( player.IsValid() )
	{
		C_DOTAHero hero = player.m_hAssignedHero;

		if ( hero.IsValid() )
		{
			for ( int x = 0 ; x < MAX_ABILITIES ; ++x )
			{
				C_DOTAAbility ability = hero.m_hAbilities[ x ];

				if ( ability.IsValid() )
				{
					Msg(
						"Ability %d (%s), Activated: %d, Hidden: %d, Level: %d, Cooldown: %0.2f, Mana cost: %d\n",
						x, (char *)ability.m_iName, (bool)ability.m_bActivated, (bool)ability.m_bHidden, (int)ability.m_iLevel, ability.GetCooldownTimeRemaining(), (int)ability.m_iManaCost
					);
				}
			}

			for ( int x = 0 ; x < MAX_UNIT_ITEMS ; ++x )
			{
				C_DOTAItem item = hero.m_hItems[ x ];

				if ( item.IsValid() )
				{
					Msg( "Item %d: %s (modifier: %s)\n", x, (char *)item.m_iName, item.GetIntrinsicModifierName() );
				}
			}

			Msg( "Magic Res: %0.2f, Mana: %0.2f, Max Mana: %0.2f\n", ( float )hero.m_flMagicalResistanceValue, ( float )hero.m_flMana, ( float )hero.m_flMaxMana );
			Msg( "Health: %d, Max Health: %d\n", ( int )hero.m_iHealth, ( int )hero.m_iMaxHealth  );
		}
	}

	C_DOTAGameRules gameRules = C_DOTAGameRules::GetGameRules();

	if ( gameRules.IsValid() )
	{
		Msg(
			"State: %d, game time: %0.2f, Time of day: %d, Start time: %0.2f\n",
			(int)gameRules.m_nGameState, (float)gameRules.m_fGameTime, (int)gameRules.m_iNetTimeOfDay, (float)gameRules.m_flGameStartTime
		);
	}

	for ( int x = 1 ; x <= MAX_PLAYERS ; ++x )
	{
		C_DOTAPlayer player = C_DOTAPlayer::GetPlayerByIndex( x );

		if ( !player.IsValid() )
			continue;

		C_DOTAHero hero = player.m_hAssignedHero;

		if ( !hero.IsValid() )
			continue;

		Msg(
			"Player %d (%d) is playing as %d at level %d, with %d/%d/%d kda, team: %d\n",
			x, ( int )hero.m_iPlayerID, hero.GetHeroID(), hero.GetLevel(), hero.GetNumKills(), hero.GetNumDeaths(), hero.GetNumAssists(), ( int )player.m_iTeamNum
		);
	}

}


template <typename T>
T *GetInterface( CreateInterfaceFn factory, const char *version )
{
	T* iface = reinterpret_cast<T *>( factory( version, NULL ) );

	if ( !iface )
	{
		Error( "Unable to get interface %s!", version );
		return NULL;
	}

	return iface;
}

void CVH::Init()
{
	m_fnEngineFactory = Sys_GetFactory( "engine" );
	m_fnClientFactory = Sys_GetFactory( "client" );
	m_fnCvarFactory = VStdLib_GetICVarFactory();
	m_fnFileSystemFactory = Sys_GetFactory( "filesystem_stdio" );

	CreateInterfaceFn factories[] =
	{
		m_fnEngineFactory,
		m_fnClientFactory,
		m_fnCvarFactory,
		m_fnFileSystemFactory,
	};

	// connect tier1
	// passing the vstdlib factory sets up icvar for us
	ConnectTier1Libraries( factories, 4 );

	if ( g_pCVar == NULL )
	{
		Error( "Unable to get ICVar!" );
		return;
	}

	// get engine interfaces
	m_pEngineClient = GetInterface<IVEngineClient>( m_fnEngineFactory, VENGINE_CLIENT_INTERFACE_VERSION );
	m_pEngineTool = GetInterface<IEngineTool>( m_fnEngineFactory, VENGINETOOL_INTERFACE_VERSION );
	m_pGameEventManager = GetInterface<IGameEventManager2>( m_fnEngineFactory, INTERFACEVERSION_GAMEEVENTSMANAGER2 );
	// get client interfaces
	m_pClientDLL = GetInterface<IBaseClientDLL>( m_fnClientFactory, CLIENT_DLL_INTERFACE_VERSION );
	m_pClientTools = GetInterface<IClientTools>( m_fnClientFactory, VCLIENTTOOLS_INTERFACE_VERSION );
	// get filesystem interfaces
	m_pFileSystem = GetInterface<IFileSystem>( m_fnFileSystemFactory, FILESYSTEM_INTERFACE_VERSION );

	// to support CGameEventListener
	gameeventmanager = m_pGameEventManager;
	g_pFullFileSystem = m_pFileSystem;

	SH_ADD_HOOK( IBaseClientDLL, FrameStageNotify, m_pClientDLL, SH_MEMBER( this, &CVH::FrameStageNotify ), true );

	// register our convars and concommands
	ConVar_Register();

	// init our things
	EntityHelper().Init();
	ConVarHelper().Init();
	RenderHelper().Init();

	// init managers
	ScriptManager().Init();
	ZeusManager().Init();
	CameraManager().Init();
	FogManager().Init();
}

void CVH::Shutdown()
{
	// un-init our things
	FogManager().Shutdown();
	CameraManager().Shutdown();
	ZeusManager().Shutdown();
	ScriptManager().Shutdown();

	RenderHelper().Shutdown();
	ConVarHelper().Shutdown();
	EntityHelper().Shutdown();

	ConVar_Unregister();

	SH_REMOVE_HOOK( IBaseClientDLL, FrameStageNotify, m_pClientDLL, SH_MEMBER( this, &CVH::FrameStageNotify ), true );

	m_pClientTools = NULL;
	m_pClientDLL = NULL;

	m_pFileSystem = NULL;
	m_pGameEventManager = NULL;
	m_pEngineTool = NULL;
	m_pEngineClient = NULL;

	DisconnectTier1Libraries();

	m_fnCvarFactory = NULL;
	m_fnClientFactory = NULL;
	m_fnEngineFactory = NULL;

	m_FrameHooks.RemoveAll();
}


void CVH::AddFrameHook( const IFrameManager *manager )
{
	m_FrameHooks.AddToTail( const_cast<IFrameManager *>( manager ) );
}

void CVH::RemoveFrameHook( const IFrameManager *manager )
{
	FOR_EACH_VEC( m_FrameHooks, i )
	{
		const IFrameManager *frameManager = m_FrameHooks[ i ];

		if ( frameManager == manager )
		{
			m_FrameHooks.Remove( i );
			break;
		}
	}
}

void CVH::FrameStageNotify( ClientFrameStage_t curStage )
{
	if ( curStage == FRAME_RENDER_START )
	{
		// entity simulation happens here, so we'll think alongside
		Think();
	}

	RETURN_META( MRES_IGNORED );
}

void CVH::Think()
{
	FOR_EACH_VEC( m_FrameHooks, i )
	{
		// think for every installed frame hook
		m_FrameHooks[ i ]->Think();
	}
}

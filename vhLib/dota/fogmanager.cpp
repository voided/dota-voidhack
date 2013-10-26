
#include "fogmanager.h"

#include "convarhelper.h"


// fog enabled by default, as the usefulness is limited
ConVar vh_fog_enable( "vh_fog_enable", "1", FCVAR_NONE, "Enable or disable game fog" );

void FogManager_OnFogChanged( IConVar *var, const char *pOldValue, float flOldValue )
{
	if ( vh_fog_enable.GetBool() )
	{
		FogManager().EnableFog();
	}
	else
	{
		FogManager().DisableFog();
	}
}


CFogManager &FogManager()
{
	static CFogManager fogManager;
	return fogManager;
}


void CFogManager::Init()
{
	// install the change callback, and invoke it
	// this will perform the enable/disable fog logic
	vh_fog_enable.InstallChangeCallback( FogManager_OnFogChanged );
}

void CFogManager::Shutdown()
{
	// revert and enable fog on shutdown
	EnableFog();
}


void CFogManager::EnableFog()
{
	SetConVar( "fog_override", 0 );
	SetConVar( "fog_enable", 1 );
	SetConVar( "fow_client_visibility", 0 );
	SetConVar( "dota_minimap_draw_fow", 1 );
}

void CFogManager::DisableFog()
{
	SetConVar( "fog_override", 1 );
	SetConVar( "fog_enable", 0 );
	SetConVar( "fow_client_visibility", 1 );
	SetConVar( "dota_minimap_draw_fow", 0 );
}


void CFogManager::SetConVar( const char *convarName, int value )
{
	ConVar *cvar = ConVarHelper().FindConVar( convarName );
	if ( cvar )
	{
		cvar->SetValue( value );
	}
}

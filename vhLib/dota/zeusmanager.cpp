
#include "zeusmanager.h"

#include "scriptmanager.h"

#include "dotaplayer.h"
#include "dotahero.h"
#include "dotaresource.h"
#include "dotaability.h"


ConVar vh_zeus_autoult( "vh_zeus_autoult", "1", FCVAR_NONE, "Enable or disable automatic zeus ulting" );
ConVar vh_zeus_dmg_buffer( "vh_zeus_dmg_buffer", "50", FCVAR_NONE, "Amount of damage buffer to subtract from ult damage calculation" );
ConVar vh_zeus_taunt( "vh_zeus_taunt", "1", FCVAR_NONE, "Do we taunt players after killing them?" );


CZeusManager &ZeusManager()
{
	static CZeusManager zeusManager;
	return zeusManager;
}


CZeusManager::CZeusManager()
	: m_flNextTaunt( FLT_MAX ),
	  m_flLastUlt( 0 )
{
	ListenForGameEvent( "dota_player_kill" );
}


void CZeusManager::Init()
{
	VH().AddFrameHook( this );
}

void CZeusManager::Shutdown()
{
	VH().RemoveFrameHook( this );
}


void CZeusManager::Think()
{
	if ( !VH().EngineTool()->IsInGame() )
		return; // not in game, no logic to perform

	if ( IsPlayingAsZeus() )
	{
		if ( vh_zeus_autoult.GetBool() && IsUltReady() && ShouldUlt() )
		{
			DoUlt();
			return;
		}

		// check if we should taunt players
		if ( VH().EngineTool()->ClientTime() >= m_flNextTaunt )
		{
			m_flNextTaunt = FLT_MAX;
			VH().EngineClient()->ExecuteClientCmd( "say \"You can't run from heaven!\"\n" );
		}
	}
}


// precondition: playing as zeus, and ult is ready for use
bool CZeusManager::ShouldUlt()
{
	C_DOTAPlayer localPlayer = C_DOTAPlayer::GetLocalPlayer();
	C_DOTAHero hero = localPlayer.m_hAssignedHero;
	C_DOTAAbility abilityUlt = hero.m_hAbilities[ 3 ];

	int ultDamage = CalculateDamage( abilityUlt.m_iLevel );

	if ( ultDamage == 0 )
	{
		Warning( "[ZeusManager] Unable to calculate ult damage!\n" );
		return false;
	}

	for ( int i = 1 ; i <= MAX_PLAYERS ; ++i )
	{
		C_DOTAPlayer player = C_DOTAPlayer::GetPlayerByIndex( i );

		if ( !player.IsValid() || player.IsLocalPlayer() )
			continue; // ignore invalid players and self

		if ( player.m_iTeamNum == localPlayer.m_iTeamNum )
			continue; // ignore teammates

		if ( IsPlayerUltable( player, ultDamage ) )
			return true;
	}

	return false;
}

bool CZeusManager::IsPlayerUltable( C_DOTAPlayer &player, int damage )
{
	C_DOTAHero hero = player.m_hAssignedHero;

	if ( !hero.IsValid() )
		return false;

	if ( hero.InAnyState( UnitState_Invisible | UnitState_Invulnerable | UnitState_MagicImmune | UnitState_OutOfGame ) )
		return false; // ult won't affect units in this state

	if ( hero.m_iHealth == 0 )
		return false; // player dead

	// todo: calculate that player's magic resistance from items

	float damageMult = ( 100.0 - hero.m_flMagicalResistanceValue ) / 100.0;
	float effectiveDamage = ( damage * damageMult ) - vh_zeus_dmg_buffer.GetFloat(); // add some buffer room for slightly off calculations

	// todo: ethereal extra damage

	return effectiveDamage >= hero.m_iHealth;
}

int CZeusManager::CalculateDamage( int level )
{
	if ( level == 0 )
		return 0;

	DOTAAbilityInfo_t *pInfo = ScriptManager().GetAbilityInfo( "zuus_thundergods_wrath" );

	if ( !pInfo )
		return 0;

	DOTAAbilitySpecial_t *pSpecial = pInfo->GetSpecialByName( HasScepter() ? "damage_scepter" : "damage" );

	if ( !pSpecial )
		return 0;

	return V_atoi( pSpecial->values[ level - 1 ] );
}

bool CZeusManager::HasScepter()
{
	C_DOTAHero hero = C_DOTAPlayer::GetLocalPlayer().m_hAssignedHero;

	return hero.HasItem( "item_ultimate_scepter" );
}

bool CZeusManager::IsPlayingAsZeus()
{
	C_DOTAHero hero = C_DOTAPlayer::GetLocalPlayer().m_hAssignedHero;

	if ( !hero.IsValid() )
		return false;

	return hero.GetHeroID() == HERO_ZEUS; // 22
}

// precondition: we're playing as zeus
bool CZeusManager::IsUltReady()
{
	if ( HasUltedRecently() )
		return false; // artifical delay so we don't spam the ability command

	C_DOTAHero hero = C_DOTAPlayer::GetLocalPlayer().m_hAssignedHero;

	if ( hero.m_iHealth == 0 )
		return false; // we're dead

	C_DOTAAbility ability = hero.m_hAbilities[ 3 ]; // ability 3 (zero indexed) is ult

	if ( !ability.IsValid() )
		return false;

	if ( ability.m_iLevel == 0 )
		return false; // not skilled

	if ( hero.m_flMana < ability.m_iManaCost )
		return false; // not enough mana

	if ( ability.GetCooldownTimeRemaining() > 0.0 )
		return false; // still waiting for it to cool down

	// ult is ready
	return true;
}

bool CZeusManager::HasUltedRecently()
{
	float timeSinceLastUlt = VH().EngineTool()->ClientTime() - m_flLastUlt;
	return timeSinceLastUlt < 1.0;
}

void CZeusManager::DoUlt()
{
	// just ult
	// difter could learn a thing or two here

	// select our hero
	VH().EngineClient()->ExecuteClientCmd( "+dota_camera_follow\n" );
	VH().EngineClient()->ExecuteClientCmd( "-dota_camera_follow\n" );

	// execute ult
	VH().EngineClient()->ExecuteClientCmd( "dota_ability_execute 5\n" );

	// this is dirty, but queueing up a CUnitOrders would be fragile

	m_flLastUlt = VH().EngineTool()->ClientTime();
}

void CZeusManager::FireGameEvent( IGameEvent *event  )
{
	if ( V_stricmp( event->GetName(), "dota_player_kill" ) != 0 )
		return;

	C_DOTAPlayer victim = C_DOTAPlayer::GetPlayerByPlayerID( event->GetInt( "victim_userid" ) );
	C_DOTAPlayer killer = C_DOTAPlayer::GetPlayerByPlayerID( event->GetInt( "killer1_userid" ) );

	// we're only interested in the first killer
	// if that's not us, we were involved in a kill that wasn't directly caused by us

	if ( !victim.IsValid() || !killer.IsValid() )
		return;

	if ( !killer.IsLocalPlayer() )
		return; // we weren't the killer

	if ( !HasUltedRecently() )
		return; // if we haven't ulted recently, the ult didn't lead to the death

	if ( vh_zeus_taunt.GetBool() )
	{
		m_flNextTaunt = m_flLastUlt + 5.0;
	}
}
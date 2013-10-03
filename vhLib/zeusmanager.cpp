
#include "vh.h"

#include "zeusmanager.h"

#include "dotaplayer.h"
#include "dotahero.h"
#include "dotaresource.h"
#include "dotaability.h"



CZeusManager g_ZeusManager;

CZeusManager &ZeusManager()
{
	return g_ZeusManager;
}


CZeusManager::CZeusManager()
	: m_flNextTaunt( FLT_MAX )
{
}


void CZeusManager::Init()
{
	VH().AddFrameHook( ZeusManager_Think );
}

void CZeusManager::Shutdown()
{
	VH().RemoveFrameHook( ZeusManager_Think );
}


void ZeusManager_Think()
{
	ZeusManager().Think();
}

void CZeusManager::Think()
{
	if ( !VH().EngineTool()->IsInGame() )
		return; // not in game, no logic to perform

	if ( IsPlayingAsZeus() )
	{
		if ( IsUltReady() && ShouldUlt() )
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
	int ultDamage = 0;

	C_DOTAPlayer localPlayer = C_DOTAPlayer::GetLocalPlayer();
	C_DOTAHero hero = localPlayer.m_hAssignedHero;
	C_DOTAAbility abilityUlt = hero.m_hAbilities[ 3 ];

	// todo: this is gross, but pulling values from the KV scripts is a wishlist feature
	// todo: handle having aghs?
	switch ( abilityUlt.m_iLevel )
	{
	case 1:
		ultDamage = 225;
		break;

	case 2:
		ultDamage = 325;
		break;

	case 3:
		ultDamage = 450;
		break;
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
	float effectiveDamage = ( damage * damageMult ) - 25; // add some buffer room for slightly off calculations

	return effectiveDamage >= hero.m_iHealth;
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
	C_DOTAHero hero = C_DOTAPlayer::GetLocalPlayer().m_hAssignedHero;
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

	m_flNextTaunt = VH().EngineTool()->ClientTime() + 5.0;
}

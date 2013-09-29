
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

	if ( IsPlayingAsZeus() && IsUltReady() && ShouldUlt() )
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


bool CZeusManager::ShouldUlt()
{
	// todo: 
	//   check if any players are worth ulting (low hp, calculate magic resistance)

	return false;
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

	// todo: check cooldown time

	return true;
}

void CZeusManager::DoUlt()
{
	// just ult
	// difter could learn a thing or two here

	// todo: select hero first, or perhaps execute the ability directly?
	VH().EngineClient()->ExecuteClientCmd( "dota_ability_execute 5\n" );

	m_flNextTaunt = VH().EngineTool()->ClientTime() + 5.0;
}

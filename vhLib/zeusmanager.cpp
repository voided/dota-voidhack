
#include "vh.h"

#include "zeusmanager.h"

#include "dotaplayer.h"
#include "dotahero.h"
#include "dotaresource.h"



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

	if ( this->ShouldUlt() )
	{
		this->DoUlt();
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
	if ( !IsPlayingAsZeus() )
		return false;

	// todo: 
	//   check if our ult isn't on CD & we have enough mana
	//   check if any players are worth ulting (low hp, calculate magic resistance)

	return false;
}

bool CZeusManager::IsPlayingAsZeus()
{
	C_DOTAPlayer player = C_DOTAPlayer::GetLocalPlayer();

	if ( !player.IsValid() )
		return false;

	C_DOTAHero hero = player.m_hAssignedHero;

	if ( !hero.IsValid() )
		return false;

	C_DOTAResource resourceEnt = C_DOTAResource::GetResourceEntity();

	if ( !resourceEnt.IsValid() )
		return false;

	int heroId = resourceEnt.m_nSelectedHeroID[ hero.m_iPlayerID ];

	return heroId == HERO_ZEUS; // 22
}

void CZeusManager::DoUlt()
{
	// just ult
	// difter could learn a thing or two here

	// todo: select hero first, or perhaps execute the ability directly
	VH().EngineClient()->ExecuteClientCmd( "dota_ability_execute 5\n" );

	m_flNextTaunt = VH().EngineTool()->ClientTime() + 5.0;
}

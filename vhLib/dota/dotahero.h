
#pragma once


#include "dotaentity.h"


class C_DOTAHero : public C_DOTABaseEntity
{
	DOTA_CLASS( C_DOTAHero, C_DOTABaseEntity );

public:
	C_DOTAHero( C_BaseEntity *pEnt );


	int GetHeroID();
	
	int GetNumKills();
	int GetNumAssists();
	int GetNumDeaths();

	int GetLevel();


	CEntProp( int, m_iPlayerID );

	CEntProp( int, m_iHealth );
	CEntProp( int, m_iMaxHealth );

	CEntProp( float, m_flMana );
	CEntProp( float, m_flMaxMana );

	CEntPropEntArray( m_hAbilities );
};

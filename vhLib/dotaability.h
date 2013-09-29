
#pragma once


#include "dotaentity.h"

class C_DOTAAbility : public C_DOTABaseEntity
{
	DOTA_CLASS( C_DOTAAbility, C_DOTABaseEntity );

public:
	C_DOTAAbility( C_BaseEntity *pEnt );


	CEntProp( bool, m_bHidden ); // unknown, always 0?
	CEntProp( bool, m_bActivated ); // unknown, always 1?

	CEntProp( int, m_iLevel ); // ability level, 0 when unskilled

	CEntProp( float, m_fCooldown ); // unknown, 0 bits
	CEntProp( float, m_flCooldownLength ); // total cooldown time for this ability

	CEntProp( int, m_iManaCost ); // total mana cost to activate this ability
};

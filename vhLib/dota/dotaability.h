
#pragma once


#include "dotaentity.h"


// represents an ability and it's associoated data that a hero may have
class C_DOTAAbility : public C_DOTABaseEntity
{
	DOTA_CLASS( C_DOTAAbility, C_DOTABaseEntity );

public:
	C_DOTAAbility( C_BaseEntity *pEnt );


	// gets the number of seconds remaining until this ability can be used again
	float GetCooldownTimeRemaining();


	CEntProp( bool, m_bHidden ); // is this ability hidden?
	CEntProp( bool, m_bActivated );

	CEntProp( int, m_iLevel ); // ability level, 0 when unskilled

	CEntProp( float, m_fCooldown ); // game time at which this ability will be available again
	CEntProp( float, m_flCooldownLength ); // total cooldown time for this ability

	CEntProp( int, m_iManaCost ); // total mana cost to activate this ability
};

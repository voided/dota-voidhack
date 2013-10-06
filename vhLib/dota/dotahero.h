
#pragma once


#include "dotaentity.h"


// various states a unit can be in
enum DOTAUnitState
{
	UnitState_Rooted = 0,
	UnitState_SoftDisarmed = 1,
	UnitState_HardDisarmed = 2,
	UnitState_AttackImmune = 3,
	UnitState_Silenced = 4,
	UnitState_Muted = 5,
	UnitState_Stunned = 6,
	UnitState_Hexed = 7,
	UnitState_Invisible = 8,
	UnitState_Invulnerable = 9,
	UnitState_MagicImmune = 10,
	UnitState_Revealed = 11, // according to sm.js
	UnitState_Nightmared = 12,
	UnitState_BlockDisabled = 13,
	UnitState_EvadeDisabled = 14,
	UnitState_Unselectable = 15,
	UnitState_SpeciallyDeniable = 17,
	UnitState_Frozen = 18,
	UnitState_CommandRestricted = 19,
	UnitState_LowAttackPriority = 22,
	UnitState_Dominated = 29,
	UnitState_Blind = 30,
	UnitState_OutOfGame = 31,
};


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

	// is this hero in all of these states?
	bool InState( int state );
	// is this hero in any of these states?
	bool InAnyState( int state );

	// does this hero own this item?
	bool HasItem( const char *itemName );


	CEntProp( int, m_iPlayerID );

	CEntProp( int, m_iHealth );
	CEntProp( int, m_iMaxHealth );

	CEntProp( float, m_flMana );
	CEntProp( float, m_flMaxMana );

	CEntProp( int, m_nUnitState );

	CDataProp( float, m_flMagicalResistanceValue ); // base magical resistance
	CDataProp( float, m_flPhysicalArmorValue ); // base armor

	CEntPropArray( C_BaseEntity *, m_hAbilities );

	CEntPropArray( C_BaseEntity *, m_hItems );
};

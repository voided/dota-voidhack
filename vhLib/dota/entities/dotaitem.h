
#pragma once

#include "dotaability.h"


class C_DOTAItem : public C_DOTAAbility
{
	DOTA_CLASS( C_DOTAItem, C_DOTAAbility );

public:
	C_DOTAItem( C_BaseEntity *pEnt );


	// returns the internal name of the modifier this item provides
	DECLARE_VFUNC0( const char *, GetIntrinsicModifierName, 228 );

};
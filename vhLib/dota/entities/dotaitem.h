
#pragma once

#include "dotaability.h"


struct DOTAItemInfo_t;


class C_DOTAItem : public C_DOTAAbility
{
	DECLARE_CLASS( C_DOTAItem, C_DOTAAbility );

public:
	C_DOTAItem( C_BaseEntity *pEnt );


	const DOTAItemInfo_t *GetItemInfo();


	// returns the internal name of the modifier this item provides
	DECLARE_VFUNC0( const char *, GetIntrinsicModifierName, 228 );

};
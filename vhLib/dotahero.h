
#pragma once


#include "dotaentity.h"


class C_DOTAHero : public C_DOTABaseEntity
{
	DOTA_CLASS( C_DOTAHero, C_DOTABaseEntity );

public:
	C_DOTAHero( C_BaseEntity *pEnt );


	CEntProp( int, m_iPlayerID );
};

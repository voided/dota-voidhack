
#pragma once


#include "dotaentity.h"


class C_DOTAResource : public C_DOTABaseEntity
{
	DOTA_CLASS( C_DOTAResource, C_DOTABaseEntity );

public:
	C_DOTAResource( C_BaseEntity *pEnt );


	CEntPropArray( int, m_nSelectedHeroID );


	static C_DOTAResource GetResourceEntity();

};
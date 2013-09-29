
#pragma once

#include "dotaentity.h"

#include "entprop.h"




class C_BasePlayer;



class C_DOTAPlayer : public C_DOTABaseEntity
{
	DOTA_CLASS( C_DOTAPlayer, C_DOTABaseEntity );

public:
	C_DOTAPlayer( C_BasePlayer *pEnt );


	CEntPropEnt( m_hAssignedHero );


	static C_DOTAPlayer GetLocalPlayer();

};


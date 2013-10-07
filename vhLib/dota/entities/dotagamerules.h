
#pragma once


#include "dotaentity.h"


// wrapper over gamerules proxy entity, which defines ent props that exist on the gamerules class
// entprop lookup automatically handles the translation so that props are accessed relative to the real gamerules class
class C_DOTAGameRules : public C_DOTABaseEntity
{
	DOTA_CLASS( C_DOTAGameRules, C_DOTABaseEntity );

public:
	C_DOTAGameRules( C_BaseEntity *pEnt );


	CEntProp( int, m_iNetTimeOfDay );
	CEntProp( float, m_fGameTime );

	CEntProp( bool, m_bGamePaused );

	CEntProp( int, m_nGameState );
	CEntProp( int, m_iGameMode );

	CEntProp( float, m_flGameStartTime );


	static C_DOTAGameRules GetGameRules();

};

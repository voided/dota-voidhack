

#include "dotahero.h"

#include "dotaresource.h"


C_DOTAHero::C_DOTAHero( C_BaseEntity *pEnt )
	: BaseClass( pEnt )
{
}


int C_DOTAHero::GetHeroID()
{
	return C_DOTAResource::GetResourceEntity().m_nSelectedHeroID[ m_iPlayerID ];
}

int C_DOTAHero::GetNumKills()
{
	return C_DOTAResource::GetResourceEntity().m_iKills[ m_iPlayerID ];
}
int C_DOTAHero::GetNumAssists()
{
	return C_DOTAResource::GetResourceEntity().m_iAssists[ m_iPlayerID ];
}
int C_DOTAHero::GetNumDeaths()
{
	return C_DOTAResource::GetResourceEntity().m_iDeaths[ m_iPlayerID ];
}

int C_DOTAHero::GetLevel()
{
	return C_DOTAResource::GetResourceEntity().m_iLevel[ m_iPlayerID ];
}

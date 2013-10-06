

#include "dotahero.h"

#include "dotaresource.h"
#include "dotaitem.h"


C_DOTAHero::C_DOTAHero( C_BaseEntity *pEnt )
	: BaseClass( pEnt )
{
}


int C_DOTAHero::GetHeroID()
{
	Assert( IsValid() );
	return C_DOTAResource::GetResourceEntity().m_nSelectedHeroID[ m_iPlayerID ];
}

int C_DOTAHero::GetNumKills()
{
	Assert( IsValid() );
	return C_DOTAResource::GetResourceEntity().m_iKills[ m_iPlayerID ];
}
int C_DOTAHero::GetNumAssists()
{
	Assert( IsValid() );
	return C_DOTAResource::GetResourceEntity().m_iAssists[ m_iPlayerID ];
}
int C_DOTAHero::GetNumDeaths()
{
	Assert( IsValid() );
	return C_DOTAResource::GetResourceEntity().m_iDeaths[ m_iPlayerID ];
}

int C_DOTAHero::GetLevel()
{
	Assert( IsValid() );
	return C_DOTAResource::GetResourceEntity().m_iLevel[ m_iPlayerID ];
}

bool C_DOTAHero::InState( int state )
{
	Assert( IsValid() );

	int bitFlag = 1 << state;
	return ( m_nUnitState & bitFlag ) == bitFlag;
}

bool C_DOTAHero::InAnyState( int state )
{
	Assert( IsValid() );

	int bitFlag = 1 << state;
	return ( m_nUnitState & bitFlag ) != 0;
}

bool C_DOTAHero::HasItem( const char *itemName )
{
	Assert( IsValid() );

	for ( int x = 0 ; x < m_hItems.Size() ; ++x )
	{
		C_DOTAItem item = m_hItems[ x ];

		if ( !item.IsValid() )
			continue;

		if ( V_stricmp( item.GetName(), itemName ) == 0 )
			return true;
	}

	return false;
}

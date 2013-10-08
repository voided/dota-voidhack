
#include "fogmanager.h"

#include "convarhelper.h"


CFogManager &FogManager()
{
	static CFogManager fogManager;
	return fogManager;
}


void CFogManager::Init()
{
	m_pCvarFogOverride = ConVarHelper().FindConVar( "fog_override" );

	if ( m_pCvarFogOverride )
	{
		m_pCvarFogOverride->SetValue( 1 );
	}

	m_pCvarFogEnabled = ConVarHelper().FindConVar( "fog_enable" );

	if ( m_pCvarFogEnabled )
	{
		m_pCvarFogEnabled->SetValue( 0 );
	}

	m_pCvarVisibility = ConVarHelper().FindConVar( "fow_client_visibility" );

	if ( m_pCvarVisibility )
	{
		m_pCvarVisibility->SetValue( 1 );
	}
}

void CFogManager::Shutdown()
{
	m_pCvarVisibility = NULL;
	m_pCvarFogEnabled = NULL;
	m_pCvarFogOverride = NULL;
}

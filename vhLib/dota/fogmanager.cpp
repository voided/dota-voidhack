
#include "fogmanager.h"

#include "convar.h"


CFogManager &FogManager()
{
	static CFogManager fogManager;
	return fogManager;
}


void CFogManager::Init()
{
	m_pCvarFogOverride = g_pCVar->FindVar( "fog_override" );

	if ( m_pCvarFogOverride )
	{
		m_pCvarFogOverride->RemoveFlags( FCVAR_CHEAT );
		m_pCvarFogOverride->SetValue( 1 );
	}

	m_pCvarFogEnabled = g_pCVar->FindVar( "fog_enable" );

	if ( m_pCvarFogEnabled )
	{
		m_pCvarFogEnabled->RemoveFlags( FCVAR_CHEAT );
		m_pCvarFogEnabled->SetValue( 0 );
	}
	m_pCvarVisibility = g_pCVar->FindVar( "fow_client_visibility" );

	if ( m_pCvarVisibility )
	{
		m_pCvarVisibility->RemoveFlags( FCVAR_CHEAT );
		m_pCvarVisibility->SetValue( 1 );
	}
}

void CFogManager::Shutdown()
{
	if ( m_pCvarVisibility )
	{
		m_pCvarVisibility->Revert();
		m_pCvarVisibility->AddFlags( FCVAR_CHEAT );
		m_pCvarVisibility = NULL;
	}

	if ( m_pCvarFogEnabled )
	{
		m_pCvarFogEnabled->Revert();
		m_pCvarFogEnabled->AddFlags( FCVAR_CHEAT );
		m_pCvarFogEnabled = NULL;
	}

	if ( m_pCvarFogOverride )
	{
		m_pCvarFogOverride->Revert();
		m_pCvarFogOverride->AddFlags( FCVAR_CHEAT );
		m_pCvarFogOverride = NULL;
	}
}

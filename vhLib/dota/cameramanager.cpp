
#include "cameramanager.h"

#include "convar.h"


CCameraManager &CameraManager()
{
	static CCameraManager cameraManager;
	return cameraManager;
}


void CCameraManager::Init()
{
	m_pCvarFOVMax = g_pCVar->FindVar( "dota_camera_fov_max" );

	if ( m_pCvarFOVMax )
	{
		m_pCvarFOVMax->RemoveFlags( FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY );
		m_pCvarFOVMax->SetValue( 100 );
	}

	m_pCvarDistance = g_pCVar->FindVar( "dota_camera_distance" );

	if ( m_pCvarDistance )
	{
		m_pCvarDistance->RemoveFlags( FCVAR_CHEAT );
		m_pCvarDistance->SetValue( 1300 );
	}
}

void CCameraManager::Shutdown()
{
	if ( m_pCvarFOVMax )
	{
		m_pCvarFOVMax->Revert();
		m_pCvarFOVMax->AddFlags( FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY );
		m_pCvarFOVMax = NULL;
	}

	if ( m_pCvarDistance )
	{
		m_pCvarDistance->Revert();
		m_pCvarDistance->AddFlags( FCVAR_CHEAT );
		m_pCvarDistance = NULL;
	}
}

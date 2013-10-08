
#include "cameramanager.h"

#include "convarhelper.h"


CCameraManager &CameraManager()
{
	static CCameraManager cameraManager;
	return cameraManager;
}


void CCameraManager::Init()
{
	m_pCvarFOVMax = ConVarHelper().FindConVar( "dota_camera_fov_max" );

	if ( m_pCvarFOVMax )
	{
		m_pCvarFOVMax->SetValue( 100 );
	}

	m_pCvarDistance = ConVarHelper().FindConVar( "dota_camera_distance" );

	if ( m_pCvarDistance )
	{
		m_pCvarDistance->SetValue( 1300 );
	}
}

void CCameraManager::Shutdown()
{
	m_pCvarDistance = NULL;
	m_pCvarFOVMax = NULL;
}

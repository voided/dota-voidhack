
#include "cameramanager.h"

#include "convarhelper.h"


CCameraManager &CameraManager()
{
	static CCameraManager cameraManager;
	return cameraManager;
}


void CCameraManager::Init()
{
	ConVar *dota_camera_fov_max = ConVarHelper().FindConVar( "dota_camera_fov_max" );
	if ( dota_camera_fov_max )
	{
		dota_camera_fov_max->SetValue( 100 );
	}

	ConVar *dota_camera_distance = ConVarHelper().FindConVar( "dota_camera_distance" );
	if ( dota_camera_distance )
	{
		dota_camera_distance->SetValue( 1300 );
	}
}

void CCameraManager::Shutdown()
{
}

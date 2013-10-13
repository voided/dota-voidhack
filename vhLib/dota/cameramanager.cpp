
#include "cameramanager.h"

#include "convar.h"


CCameraManager &CameraManager()
{
	static CCameraManager cameraManager;
	return cameraManager;
}


void CCameraManager::Init()
{
	ConVarRef dota_camera_fov_max( "dota_camera_fov_max" );
	if ( dota_camera_fov_max.IsValid() )
	{
		dota_camera_fov_max.SetValue( 100 );
	}

	ConVarRef dota_camera_distance( "dota_camera_distance" );
	if ( dota_camera_distance.IsValid() )
	{
		dota_camera_distance.SetValue( 1300 );
	}
}

void CCameraManager::Shutdown()
{
}

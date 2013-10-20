
#include "cameramanager.h"

#include "convarhelper.h"
#include "renderhelper.h"

#include "iviewrender.h"

#include "vh.h"
#include <sourcehook/sourcehook.h>


CCameraManager &CameraManager()
{
	static CCameraManager cameraManager;
	return cameraManager;
}


SH_DECL_HOOK0( IViewRender, GetZFar, SH_NOATTRIB, 0, float );


ConVar vh_camera_farz( "vh_camera_farz", "4000.0" );


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

	if ( RenderHelper().ViewRender() )
		{
		SH_ADD_HOOK( IViewRender, GetZFar, RenderHelper().ViewRender(), SH_MEMBER( this, &CCameraManager::GetFarZ ), false );
	}
	else
	{
		Warning( "[CameraManager] Unable to hook GetZFar!\n" );
	}
}

void CCameraManager::Shutdown()
{
	if ( RenderHelper().ViewRender() )
	{
		SH_REMOVE_HOOK( IViewRender, GetZFar, RenderHelper().ViewRender(), SH_MEMBER( this, &CCameraManager::GetFarZ ), false );
	}
}


float CCameraManager::GetFarZ()
{
	RETURN_META_VALUE( MRES_SUPERCEDE, vh_camera_farz.GetFloat() );
}

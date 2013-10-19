
#include "cameramanager.h"

#include "convarhelper.h"

#include "iviewrender.h"

#include "bytescanner.h"
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

	CByteScanner byteScan( "client" );

	void *pFunc = NULL;
	if ( byteScan.FindCodePattern( "\x55\x8B\xEC\xA1\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x33\xDB", "xxxx????xx????xxx", &pFunc ) )
	{
		m_pViewRender = *(IViewRender **)( (uint8 *)pFunc + 0x22D );

		if ( m_pViewRender )
		{
			SH_ADD_HOOK( IViewRender, GetZFar, m_pViewRender, SH_MEMBER( this, &CCameraManager::GetFarZ ), false );
		}
		else
		{
			Warning( "[CameraManager] Unable to find g_ViewRender!\n" );
		}
	}
	else
	{
		Warning( "[CameraManager] Unable to find C_DOTACapture::RenderEditMode!\n" );
	}

}

void CCameraManager::Shutdown()
{
	if ( m_pViewRender )
	{
		SH_REMOVE_HOOK( IViewRender, GetZFar, m_pViewRender, SH_MEMBER( this, &CCameraManager::GetFarZ ), false );

		m_pViewRender = NULL;
	}
}

float CCameraManager::GetFarZ()
{
	RETURN_META_VALUE( MRES_SUPERCEDE, vh_camera_farz.GetFloat() );
}

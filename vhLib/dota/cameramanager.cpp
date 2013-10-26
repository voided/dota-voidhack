
#include "cameramanager.h"

#include "convarhelper.h"
#include "renderhelper.h"

#include "vgui/ISurface.h"
#include "iviewrender.h"

#include "vh.h"
#include <sourcehook/sourcehook.h>



int ScreenTransform( const Vector& point, Vector& screen );


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


bool CCameraManager::GetVectorInScreenSpace( Vector pos, int &iX, int &iY, Vector *vecOffset )
{
	Vector screen;

	// Apply the offset, if one was specified
	if ( vecOffset != NULL )
		pos += *vecOffset;

	// Transform to screen space
	int x = 0, y = 0;
	int screenWidth,
		screenHeight;

	RenderHelper().Surface()->GetScreenSize( screenWidth, screenHeight );

	// Transform to screen space
	int iFacing = ScreenTransform( pos, screen );

	iX = 0.5 * screen[0] * screenWidth;
	iY = -0.5 * screen[1] * screenHeight;
	iX += 0.5 * screenWidth;
	iY += 0.5 * screenHeight;	

	// Make sure the player's facing it
	if ( iFacing )
	{
		// We're actually facing away from the Target. Stomp the screen position.
		iX = -640;
		iY = -640;
		return false;
	}

	return true;
}


float CCameraManager::GetFarZ()
{
	RETURN_META_VALUE( MRES_SUPERCEDE, vh_camera_farz.GetFloat() );
}



//-----------------------------------------------------------------------------
// Purpose: UNDONE: Clean this up some, handle off-screen vertices
// Input  : *point - 
//			*screen - 
// Output : int
//-----------------------------------------------------------------------------
int ScreenTransform( const Vector& point, Vector& screen )
{
	// UNDONE: Clean this up some, handle off-screen vertices
	float w;
	const VMatrix &worldToScreen = engine->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	//	z		 = worldToScreen[2][0] * point[0] + worldToScreen[2][1] * point[1] + worldToScreen[2][2] * point[2] + worldToScreen[2][3];
	w		 = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];

	// Just so we have something valid here
	screen.z = 0.0f;

	bool behind;
	if( w < 0.001f )
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

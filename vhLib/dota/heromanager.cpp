
#include "heromanager.h"

#include "cameramanager.h"

#include "dotaplayer.h"
#include "dotahero.h"


#include "vgui/ISurface.h"


CHeroManager &HeroManager()
{
	static CHeroManager heroManager;
	return heroManager;
}


void CHeroManager::Init()
{
	RenderHelper().AddRenderHook( this );
}

void CHeroManager::Shutdown()
{
	RenderHelper().RemoveRenderHook( this );
}


void CHeroManager::RenderPreHud()
{
	C_DOTAPlayer player = C_DOTAPlayer::GetLocalPlayer();

	if ( !player.IsValid() )
		return;

	C_DOTAHero hero = player.m_hAssignedHero;

	if ( !hero.IsValid() )
		return;

	int x, y;
	CameraManager().GetVectorInScreenSpace( hero.m_vecOrigin, x, y );

	// draw a circle on the screen at the hero's position
	RenderHelper().PaintSurface()->DrawColoredCircle( x, y, 100, 255, 255, 255, 255 );
}


#include "heromanager.h"

#include "cameramanager.h"

#include "vh.h"

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
	if ( VH().EngineClient()->IsTakingScreenshot() )
		return;

	for ( int x = 1 ; x <= MAX_PLAYERS ; ++x )
	{
		C_DOTAPlayer player = C_DOTAPlayer::GetPlayerByIndex( x );

		if ( !player.IsValid() )
			continue;

		if ( player.IsLocalPlayer() )
			continue; // don't draw anything for local player

		C_DOTAHero hero = player.m_hAssignedHero;

		if ( !hero.IsValid() )
			continue; // no assigned hero

		DrawHeroInfo( hero );
	}
}


void CHeroManager::DrawHeroInfo( C_DOTAHero &hero )
{
	DrawManaBar( hero );
}

void CHeroManager::DrawManaBar( C_DOTAHero &hero )
{
	if ( hero.IsDormant() )
		return; // not in PVS, don't render mana bar

	if ( hero.m_iTeamNum == C_DOTAPlayer::GetLocalPlayer().m_iTeamNum )
		return;

	float zOff = hero.m_vecMaxs.Get().z;
	Vector vecOff( 0, 0, zOff );

	int x, y;

	if ( !CameraManager().GetVectorInScreenSpace( hero.m_vecOrigin, x, y, &vecOff ) )
		return;

	const int WIDTH = 100;
	const int HEIGHT = 15;


	// first render outer white border
	CRenderHelper::Rect rect( x - ( WIDTH / 2 ), y - ( HEIGHT / 2 ), WIDTH, HEIGHT, Color( 255, 255, 255, 255 ) );
	rect.Draw();

	int effectiveWidth = hero.GetManaPercent() * WIDTH;

	rect.x += 1;
	rect.y += 1;

	rect.height -= 2;
	rect.width = effectiveWidth - 2;

	rect.color = Color( 0, 0, 255, 255 );

	rect.Draw();
}

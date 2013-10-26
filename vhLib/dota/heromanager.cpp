
#include "heromanager.h"

#include "cameramanager.h"

#include "vh.h"

#include "dotaplayer.h"
#include "dotahero.h"


#include "vgui/ISurface.h"


ConVar vh_manabars_self( "vh_manabars_self", "0", FCVAR_NONE, "Draw manabar of local player" );
ConVar vh_manabars_teammates( "vh_manabars_teammates", "0", FCVAR_NONE, "Draw manabars of teammates" );


CHeroManager &HeroManager()
{
	static CHeroManager heroManager;
	return heroManager;
}


class CManaBar
{

public:
	CManaBar( C_DOTAHero &hero );


	void Layout();
	void Draw();


private:
	static const int WIDTH = 120;
	static const int HEIGHT = 12;

	C_DOTAHero &m_Hero;

	CRenderHelper::Rect m_Border;
	CRenderHelper::Rect m_ManaBar;

};


CManaBar::CManaBar( C_DOTAHero &hero )
	: m_Hero( hero )
{
	m_Border.SetColor( Color( 255, 255, 255, 255 ) );

	m_ManaBar.SetColor( Color( 0, 0, 255, 255 ) );
	m_ManaBar.SetFilled( true );
}

void CManaBar::Layout()
{
	const Vector &vecMaxs = m_Hero.CollisionProp().m_vecMaxs;
	Vector vecOff = Vector( 0, 0, vecMaxs.z );
	
	int x, y;
	CameraManager().GetVectorInScreenSpace( m_Hero.GetAbsOrigin(), x, y, &vecOff );

	m_Border.SetPos( x - ( WIDTH / 2 ), y );
	m_ManaBar.SetPos( x - ( WIDTH / 2 ), y );

	m_Border.SetSize( WIDTH, HEIGHT );
	m_ManaBar.SetSize( WIDTH * m_Hero.ManaFraction(), HEIGHT );
}

void CManaBar::Draw()
{
	m_ManaBar.Draw();
	m_Border.Draw();
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

		if ( player.IsLocalPlayer() && !vh_manabars_self.GetBool() )
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

	if ( hero.IsOnLocalTeam() && !vh_manabars_teammates.GetBool() )
		return;

	CManaBar manaBar( hero );

	manaBar.Layout();
	manaBar.Draw();
}

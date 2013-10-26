
#pragma once


#include "renderhelper.h"


class C_DOTAHero;


class CHeroManager : public IRenderManager
{

public:
	void Init();
	void Shutdown();


private:
	// IRenderManager
	void RenderPreHud();


	void DrawHeroInfo( C_DOTAHero &hero );

	void DrawManaBar( C_DOTAHero &hero );

};

CHeroManager &HeroManager();

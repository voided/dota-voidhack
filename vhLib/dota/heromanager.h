
#pragma once


#include "renderhelper.h"


class CHeroManager : public IRenderManager
{

public:
	void Init();
	void Shutdown();


private:
	// IRenderManager
	void RenderPreHud();

};

CHeroManager &HeroManager();

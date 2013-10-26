
#pragma once


class CFogManager
{

public:
	void Init();
	void Shutdown();

	void DisableFog();
	void EnableFog();

};

CFogManager &FogManager();

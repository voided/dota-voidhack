
#pragma once


class CFogManager
{

public:
	void Init();
	void Shutdown();

	void DisableFog();
	void EnableFog();


private:
	void SetConVar( const char *convarName, int value );

};

CFogManager &FogManager();

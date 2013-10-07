
#pragma once

#include <cstddef>


class ConVar;


class CCameraManager
{

public:
	CCameraManager() :
		m_pCvarFOVMax( NULL ),
		m_pCvarDistance( NULL )
	{
	}

	void Init();
	void Shutdown();


private:
	ConVar *m_pCvarFOVMax;
	ConVar *m_pCvarDistance;

};

CCameraManager &CameraManager();


#pragma once


#include <cstddef>


class ConVar;


class CFogManager
{
public:
	CFogManager() :
		m_pCvarFogEnabled( NULL ),
		m_pCvarFogOverride( NULL ),
		m_pCvarVisibility( NULL )
	{
	}

	void Init();
	void Shutdown();


private:
	ConVar *m_pCvarFogEnabled;
	ConVar *m_pCvarFogOverride;
	ConVar *m_pCvarVisibility;

};

CFogManager &FogManager();


#pragma once

#include "convar.h"

#include "utlmap.h"



struct ConVarInfo_t
{
	ConVar *cvar;
	int oldFlags;
};


class CConVarHelper
{

public:
	CConVarHelper() :
		m_ConVars( CaselessStringLessThan )
	{
	}


	void Init();
	void Shutdown();

	ConVar *FindConVar( const char *name );


private:
	CUtlMap<const char *, ConVarInfo_t> m_ConVars;

};

CConVarHelper &ConVarHelper();

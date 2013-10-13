
#pragma once

#include "convar.h"

#include "utlvector.h"



struct ConVarInfo_t
{
	ConVar *convar;
	int oldFlags;
};


class CConVarHelper
{

public:
	void Init();
	void Shutdown();

	ConVar *FindConVar( const char *name );


private:
	CUtlVector<ConVarInfo_t> m_ConVars;

};

CConVarHelper &ConVarHelper();

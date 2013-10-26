
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

	template <typename T>
	void SetConVar( const char *convarName, T value )
	{
		ConVar *pVar = FindConVar( convarName );
		if ( pVar )
		{
			pVar->SetValue( value );
		}
	}


private:
	CUtlVector<ConVarInfo_t> m_ConVars;

};

CConVarHelper &ConVarHelper();

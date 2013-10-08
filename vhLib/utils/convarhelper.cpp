
#include "convarhelper.h"


CConVarHelper &ConVarHelper()
{
	static CConVarHelper convarHelper;
	return convarHelper;
}


void CConVarHelper::Init()
{
}

void CConVarHelper::Shutdown()
{
	FOR_EACH_MAP_FAST( m_ConVars, i )
	{
		ConVarInfo_t &info = m_ConVars[ i ];

		info.cvar->Revert();
		info.cvar->AddFlags( info.oldFlags );
	}

	m_ConVars.RemoveAll();
}


ConVar *CConVarHelper::FindConVar( const char *name )
{
	auto cacheIndex = m_ConVars.Find( name );

	if ( m_ConVars.IsValidIndex( cacheIndex ) )
	{
		// if we have it cached, return our cached version so we don't store stale flags
		ConVarInfo_t &info = m_ConVars[ cacheIndex ];

		return info.cvar;
	}

	Assert( g_pCVar );
	ConVar *pCvar = g_pCVar->FindVar( name );

	if ( pCvar == NULL )
		return NULL;

	ConVarInfo_t info =
	{
		pCvar,
		pCvar->GetFlags()
	};

	m_ConVars.Insert( name, info );

	pCvar->RemoveFlags( FCVAR_CHEAT | FCVAR_DEVELOPMENTONLY | FCVAR_HIDDEN );

	return pCvar;
}

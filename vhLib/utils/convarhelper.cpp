
#include "convarhelper.h"


// flags which we want to remove from all convars
#define FCVAR_BADFLAGS ( FCVAR_DEVELOPMENTONLY | FCVAR_HIDDEN | FCVAR_CHEAT )


CConVarHelper &ConVarHelper()
{
	static CConVarHelper convarHelper;
	return convarHelper;
}


void CConVarHelper::Init()
{
	Assert( g_pCVar );

	ICvar::Iterator iter( g_pCVar );

	for ( iter.SetFirst() ; iter.IsValid() ; iter.Next() )
	{
		ConCommandBase *cmd = iter.Get();

		if ( cmd->IsCommand() )
			continue;

		ConVar *pVar = reinterpret_cast<ConVar *>( cmd );

		if ( pVar->GetFlags() & FCVAR_BADFLAGS )
		{
			ConVarInfo_t info;
			info.convar = pVar;
			info.oldFlags = pVar->GetFlags();

			m_ConVars.AddToTail( info );

			pVar->RemoveFlags( FCVAR_BADFLAGS );

			DevMsg( "[ConVarHelper] Fixing convar: %s\n", pVar->GetName() );
		}
	}
}

void CConVarHelper::Shutdown()
{
	FOR_EACH_VEC( m_ConVars, i )
	{
		ConVarInfo_t &info = m_ConVars[ i ];
		ConVar *pVar = info.convar;

		DevMsg( "[ConVarHelper] Resetting convar: %s\n", pVar->GetName() );

		if ( V_strcmp( pVar->GetString(), pVar->GetDefault() ) != 0 )
		{
			// if we changed the value, revert it
			pVar->Revert();
		}

		pVar->AddFlags( info.oldFlags );
	}

	m_ConVars.RemoveAll();
}


ConVar *CConVarHelper::FindConVar( const char *name )
{
	Assert( g_pCVar );

	ConVar *pVar = g_pCVar->FindVar( name );

	if ( !pVar )
	{
		Warning( "[ConVarHelper] Unable to find convar: %s\n", name );
		return NULL;
	}

	return pVar;
}

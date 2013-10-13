
#include "scriptmanager.h"

#include "vh.h"


#include "KeyValues.h"
#include "filesystem.h"


CScriptManager &ScriptManager()
{
	static CScriptManager scriptManager;
	return scriptManager;
}


void CScriptManager::Init()
{
	Assert( g_pFullFileSystem );

	m_pKvItems = new KeyValues( "" );
	if ( !m_pKvItems->LoadFromFile( g_pFullFileSystem, "scripts/npc/items.txt" ) )
	{
		Warning( "[ScriptManager] Unable to load item data!\n" );
		m_pKvItems->deleteThis();
		m_pKvItems = NULL;
	}

	m_pKvAbilities = new KeyValues( "" );
	if ( !m_pKvAbilities->LoadFromFile( g_pFullFileSystem, "scripts/npc/npc_abilities.txt" ) )
	{
		Warning( "[ScriptManager] Unable to load ability data!\n" );
		m_pKvAbilities->deleteThis();
		m_pKvAbilities = NULL; 
	}
}


void CScriptManager::Shutdown()
{
	if ( m_pKvItems )
	{
		m_pKvItems->deleteThis();
		m_pKvItems = NULL;
	}

	if ( m_pKvAbilities )
	{
		m_pKvAbilities->deleteThis();
		m_pKvAbilities = NULL;
	}
}


KeyValues *CScriptManager::GetItemInfo( const char *itemName )
{
	if ( !m_pKvItems )
		return NULL;

	return m_pKvItems->FindKey( itemName );
}

KeyValues *CScriptManager::GetAbilityInfo( const char *abilityName )
{
	if ( !m_pKvAbilities )
		return NULL;

	KeyValues *pBaseAbility = m_pKvAbilities->FindKey( "ability_base" );
	KeyValues *pAbility = m_pKvAbilities->FindKey( abilityName );

	pAbility->MergeFrom( pBaseAbility );

	return pAbility;
}

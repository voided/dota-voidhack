
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
	m_pKvItems = LoadScript( "scripts/npc/items.txt" );
	m_pKvAbilities = LoadScript( "scripts/npc/npc_abilities.txt" );
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

	FOR_EACH_MAP_FAST( m_AbilityMap, i )
	{
		delete m_AbilityMap[ i ];
	}
}

KeyValues *CScriptManager::LoadScript( const char *scriptName )
{
	Assert( g_pFullFileSystem );

	KeyValues *pKvScript = new KeyValues( "" );
	if ( !pKvScript->LoadFromFile( g_pFullFileSystem, scriptName ) )
	{
		Warning( "[ScriptManager] Unable to load script: %s\n", scriptName );

		pKvScript->deleteThis();
		return NULL;
	}

	int version = pKvScript->GetInt( "Version" );
	if ( version != 1 )
	{
		Warning( "[ScriptManager] Version mismatch for script '%s' expecting 1, got %d", scriptName, version );

		pKvScript->deleteThis();
		return NULL;
	}

	return pKvScript;
}


KeyValues *CScriptManager::GetItemInfo( const char *itemName )
{
	if ( !m_pKvItems )
		return NULL;

	return m_pKvItems->FindKey( itemName );
}

DOTAAbilityInfo_t *CScriptManager::GetAbilityInfo( const char *abilityName )
{
	if ( !m_pKvAbilities )
		return NULL;

	auto abilityIndex = m_AbilityMap.Find( abilityName );

	if ( m_AbilityMap.IsValidIndex( abilityIndex ))
	{
		// return the cached version
		return m_AbilityMap[ abilityIndex ];
	}

	KeyValues *pAbility = m_pKvAbilities->FindKey( abilityName );

	if ( !pAbility )
		return NULL;

	KeyValues *pBaseAbility = m_pKvAbilities->FindKey( "ability_base" );
	Assert( pBaseAbility );

	// create a temp copy so we don't inflate the memory usage of every ability
	pAbility = pAbility->MakeCopy();
	KeyValuesAD abilityAutoDelete( pAbility );

	// merge in the base ability data
	pAbility->RecursiveMergeKeyValues( pBaseAbility );

	DOTAAbilityInfo_t *pInfo = new DOTAAbilityInfo_t;

	pInfo->id = pAbility->GetInt( "ID" );

	FOR_EACH_SUBKEY( pAbility->FindKey( "AbilitySpecial" ), pSpecialKey )
	{
		DOTAAbilitySpecial_t *pSpecial = new DOTAAbilitySpecial_t;

		// first subkey is the field type
		KeyValues *pSub = pSpecialKey->GetFirstSubKey();

		const char *specialType = pSub->GetString();

		// the rest is the field type and values
		pSub = pSub->GetNextKey();

		const char *specialName = strdup( pSub->GetName() ); // copy so we don't try to free keyvaluesystem's memory on shutdown
		const char *specialValue = pSub->GetString();

		pSpecial->type = specialType;
		V_SplitString( specialValue, " ", pSpecial->values );

		pInfo->abilitySpecial.Insert( specialName, pSpecial );
	}

	m_AbilityMap.Insert( abilityName, pInfo );

	return pInfo;
}

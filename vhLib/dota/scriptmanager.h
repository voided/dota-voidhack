
#pragma once

#include <cstddef>

#include "utlmap.h"
#include "utlstring.h"
#include "utlvector.h"


class KeyValues;


struct DOTAAbilitySpecial_t
{
	~DOTAAbilitySpecial_t()
	{
		// todo: we ~should~ be purging and deleting here because of the V_SplitString usage
		// but since we don't have IMemAlloc, the container will attempt to delete from the wrong heap
		// values.PurgeAndDeleteElements();
	}

	CUtlString type;
	CUtlVector<char *> values;
};

struct DOTAAbilityInfo_t
{
	DOTAAbilityInfo_t()
		: abilitySpecial( CaselessStringLessThan )
	{
	}

	~DOTAAbilityInfo_t()
	{
		FOR_EACH_MAP_FAST( abilitySpecial, i )
		{
			delete abilitySpecial[ i ];
		}
	}


	DOTAAbilitySpecial_t *GetSpecialByName( const char *name )
	{
		auto specialIndex = abilitySpecial.Find( name );

		if ( !abilitySpecial.IsValidIndex( specialIndex ) )
			return NULL;

		return abilitySpecial[ specialIndex ];
	}


	int id;

	CUtlMap<const char *, DOTAAbilitySpecial_t *> abilitySpecial;
};


class CScriptManager
{

public:
	CScriptManager() :
		m_pKvItems( NULL ), m_pKvAbilities( NULL ),
		m_AbilityMap( CaselessStringLessThan )
	{
	}


	void Init();
	void Shutdown();


	KeyValues *GetItemInfo( const char *itemName );
	DOTAAbilityInfo_t *GetAbilityInfo( const char *abilityName );

private:
	KeyValues *LoadScript( const char *scriptName );


private:
	KeyValues *m_pKvItems;
	KeyValues *m_pKvAbilities;

	CUtlMap<const char *, DOTAAbilityInfo_t *> m_AbilityMap;

};

CScriptManager &ScriptManager();

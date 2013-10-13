
#pragma once


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


	float GetFloat( int level ) const
	{
		Assert( type == "FIELD_FLOAT" );

		if ( level < 1 || level > values.Count() )
			return V_atof( values[ 0 ] );

		return V_atof( values[ level - 1 ] );
	}

	int GetInt( int level ) const
	{
		Assert( type == "FIELD_INTEGER" );

		if ( level < 1 || level > values.Count() )
			return V_atoi( values[ 0 ] );

		return V_atoi( values[ level - 1 ] );
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

		// see comment in ~DOTAAbilitySpecial_t for why we can't do this
		// cooldowns.PurgeAndDeleteElements();
		// manaCosts.PurgeAndDeleteElements();
	}


	const DOTAAbilitySpecial_t *GetSpecialByName( const char *name ) const
	{
		auto specialIndex = abilitySpecial.Find( name );

		if ( !abilitySpecial.IsValidIndex( specialIndex ) )
			return NULL;

		return abilitySpecial[ specialIndex ];
	}

	float GetCooldown( int level = 0 ) const
	{
		if ( level < 1 || level > cooldowns.Count() )
			return V_atof( cooldowns[ 0 ] );

		return V_atof( cooldowns[ level - 1 ] );
	}

	int GetManaCost( int level = 0 ) const
	{
		if ( level < 1 || level > manaCosts.Count() )
			return V_atoi( manaCosts[ 0 ] );

		return V_atoi( manaCosts[ level - 1 ] );
	}


	int id;

	CUtlString type;

	CUtlString behavior;

	CUtlString unitTargetTeam;
	CUtlString unitTargetType;
	CUtlString unitTargetFlags;

	CUtlString unitDamageType;

	CUtlVector<char *> cooldowns;
	CUtlVector<char *> manaCosts;

	CUtlMap<const char *, DOTAAbilitySpecial_t *> abilitySpecial;
};

struct DOTAItemInfo_t
{
	~DOTAItemInfo_t()
	{
		delete abilityInfo;
		abilityInfo = NULL;
	}


	DOTAAbilityInfo_t *abilityInfo;

	int cost;

	CUtlString declarations;
};


class CScriptManager
{

public:
	CScriptManager() :
		m_pKvItems( NULL ), m_pKvAbilities( NULL ),
		m_AbilityMap( CaselessStringLessThan ),
		m_ItemMap( CaselessStringLessThan )
	{
	}


	void Init();
	void Shutdown();


	const DOTAItemInfo_t *GetItemInfo( const char *itemName );
	const DOTAAbilityInfo_t *GetAbilityInfo( const char *abilityName );


private:
	// loads and verifies the version of a script file
	KeyValues *LoadScript( const char *scriptName );

	DOTAAbilityInfo_t *ParseAbilityInfo( KeyValues *pAbility );
	DOTAItemInfo_t *ParseItemInfo( KeyValues *pItem );


private:
	KeyValues *m_pKvItems;
	KeyValues *m_pKvAbilities;

	CUtlMap<const char *, DOTAAbilityInfo_t *> m_AbilityMap;
	CUtlMap<const char *, DOTAItemInfo_t *> m_ItemMap;

};

CScriptManager &ScriptManager();

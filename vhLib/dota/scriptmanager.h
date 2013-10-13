
#pragma once

#include <cstddef>


class KeyValues;


class CScriptManager
{

public:
	CScriptManager() :
		m_pKvItems( NULL ), m_pKvAbilities( NULL )
	{
	}


	void Init();
	void Shutdown();


	KeyValues *GetItemInfo( const char *itemName );
	KeyValues *GetAbilityInfo( const char *abilityName );


private:
	KeyValues *m_pKvItems;
	KeyValues *m_pKvAbilities;

};

CScriptManager &ScriptManager();


#include "dotaability.h"

#include "dotagamerules.h"

#include "scriptmanager.h"


C_DOTAAbility::C_DOTAAbility( C_BaseEntity *pEnt )
	: BaseClass( pEnt )
{
}


float C_DOTAAbility::GetCooldownTimeRemaining()
{
	Assert( IsValid() );

	C_DOTAGameRules gameRules = C_DOTAGameRules::GetGameRules();
	Assert( gameRules.IsValid() );

	if ( m_flCooldownLength == 0.0 )
	{
		// if our cooldown length is zero, we haven't used this ability yet
		// therefore it's ready for use
		return 0.0;
	}

	return m_fCooldown - gameRules.m_fGameTime;
}

const DOTAAbilityInfo_t *C_DOTAAbility::GetAbilityInfo()
{
	Assert( IsValid() );

	return ScriptManager().GetAbilityInfo( m_iName );
}

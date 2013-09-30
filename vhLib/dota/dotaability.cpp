
#include "dotaability.h"

#include "dotagamerules.h"


C_DOTAAbility::C_DOTAAbility( C_BaseEntity *pEnt )
	: BaseClass( pEnt )
{
}


float C_DOTAAbility::GetCooldownTimeRemaining()
{
	Assert( m_iLevel > 0 );

	C_DOTAGameRules gameRules = C_DOTAGameRules::GetGameRules();

	if ( !gameRules.IsValid() )
		return 0.0;

	if ( m_flCooldownLength == 0.0 )
	{
		// if our cooldown length is zero, we haven't used this ability yet
		// therefore it's ready for use
		return 0.0;
	}

	return m_fCooldown - gameRules.m_fGameTime;
}

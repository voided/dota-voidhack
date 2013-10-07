
#include "dotagamerules.h"


C_DOTAGameRules::C_DOTAGameRules( C_BaseEntity *pEnt )
	: BaseClass( pEnt )
{
}


C_DOTAGameRules C_DOTAGameRules::GetGameRules()
{
	C_BaseEntity *pEnt = EntityHelper().GetGameRulesProxyEntity();
	return C_DOTAGameRules( pEnt );
}

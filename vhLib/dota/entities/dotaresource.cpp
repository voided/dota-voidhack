
#include "dotaresource.h"


C_DOTAResource::C_DOTAResource( C_BaseEntity *pEnt )
	: BaseClass( pEnt )
{
}


C_DOTAResource C_DOTAResource::GetResourceEntity()
{
	C_BaseEntity *pEnt = EntityHelper().GetResourceEntity();
	return C_DOTAResource( pEnt );
}

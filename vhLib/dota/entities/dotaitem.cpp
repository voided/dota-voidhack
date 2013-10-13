
#include "dotaitem.h"

#include "scriptmanager.h"


C_DOTAItem::C_DOTAItem( C_BaseEntity *pEnt )
	: BaseClass( pEnt )
{
}


const DOTAItemInfo_t *C_DOTAItem::GetItemInfo()
{
	Assert( IsValid() );

	return ScriptManager().GetItemInfo( m_iName );
}

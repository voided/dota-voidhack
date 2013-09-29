
#include "dotaplayer.h"

#include "entityhelper.h"



C_DOTAPlayer::C_DOTAPlayer( C_BasePlayer *pEnt )
	: BaseClass( pEnt )
{
}


C_DOTAPlayer C_DOTAPlayer::GetLocalPlayer()
{
	C_BasePlayer *pPlayer = EntityHelper().GetLocalPlayer();
	return C_DOTAPlayer( pPlayer );
}

C_DOTAPlayer C_DOTAPlayer::GetPlayerByIndex( int index )
{
	C_BasePlayer *pPlayer = reinterpret_cast<C_BasePlayer *>( EntityHelper().GetEntityFromIndex( index ) );
	return C_DOTAPlayer( pPlayer );
}
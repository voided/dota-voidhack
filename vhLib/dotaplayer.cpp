
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

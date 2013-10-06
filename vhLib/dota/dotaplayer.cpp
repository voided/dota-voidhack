
#include "dotaplayer.h"

#include "entityhelper.h"
#include "vh.h"



C_DOTAPlayer::C_DOTAPlayer( C_BasePlayer *pEnt )
	: BaseClass( pEnt )
{
}

bool C_DOTAPlayer::IsLocalPlayer()
{
	Assert( IsValid() );

	C_BasePlayer *pLocalPlayer = EntityHelper().GetLocalPlayer();
	return this->GetEntity() == pLocalPlayer;
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

C_DOTAPlayer C_DOTAPlayer::GetPlayerByPlayerID( int playerId )
{
	for ( int x = 1 ; x < MAX_PLAYERS ; ++x )
	{
		C_DOTAPlayer player = GetPlayerByIndex( x );

		if ( !player.IsValid() )
			continue;

		if ( player.m_iPlayerID == playerId )
			return player;
	}

	return C_DOTAPlayer( NULL );
}
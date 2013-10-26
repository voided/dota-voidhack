
#include "dotaentity.h"

#include "dotaplayer.h"


C_DOTABaseEntity::C_DOTABaseEntity( C_BaseEntity *pEnt )
{
	m_Entity.Set( pEnt );
}


bool C_DOTABaseEntity::IsOnLocalTeam()
{
	C_DOTAPlayer localPlayer = C_DOTAPlayer::GetLocalPlayer();
	Assert( localPlayer.IsValid() );

	return m_iTeamNum == localPlayer.m_iTeamNum;
}


#pragma once


#include "vh.h"

#include "GameEventListener.h"


class C_DOTAPlayer;
class C_DOTAAbility;


// you can't run from heaven
class CZeusManager : public CGameEventListener, public IFrameManager
{

public:
	CZeusManager();


public:
	void Init();
	void Shutdown();


private:
	virtual void Think();

	bool IsPlayingAsZeus();
	bool IsUltReady();
	bool ShouldUlt();
	bool HasUltedRecently();

	bool IsPlayerUltable( C_DOTAPlayer &player, int damage );
	bool HasScepter();

	bool CalculateDamage( C_DOTAAbility &ability, int *pOutDamage );

	void DoUlt();

	// IGameEventListener2
	virtual void FireGameEvent( IGameEvent *event  );


private:
	float m_flNextTaunt;
	float m_flLastUlt;

};

CZeusManager &ZeusManager();

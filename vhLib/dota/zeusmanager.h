
#pragma once

#include "GameEventListener.h"


class C_DOTAPlayer;


// you can't run from heaven
class CZeusManager : public CGameEventListener
{

public:
	CZeusManager();


public:
	void Init();
	void Shutdown();


private:
	friend void ZeusManager_Think();
	void Think();

	bool IsPlayingAsZeus();
	bool IsUltReady();
	bool ShouldUlt();
	bool HasUltedRecently();

	bool IsPlayerUltable( C_DOTAPlayer &player, int damage );
	bool HasScepter();

	void DoUlt();

	// IGameEventListener2
	virtual void FireGameEvent( IGameEvent *event  );


private:
	float m_flNextTaunt;
	float m_flLastUlt;

};

CZeusManager &ZeusManager();

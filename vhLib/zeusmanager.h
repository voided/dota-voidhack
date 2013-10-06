
#pragma once


class C_DOTAPlayer;


// you can't run from heaven
class CZeusManager
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

	bool IsPlayerUltable( C_DOTAPlayer &player, int damage );
	bool HasScepter();

	void DoUlt();


private:
	float m_flNextTaunt;
	float m_flLastUlt;

};

CZeusManager &ZeusManager();

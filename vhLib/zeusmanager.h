
#pragma once


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

	void DoUlt();


private:
	float m_flNextTaunt;

};

CZeusManager &ZeusManager();

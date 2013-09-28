
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


	bool ShouldUlt();
	bool IsPlayingAsZeus();

	void DoUlt();


private:
	float m_flNextTaunt;

};

CZeusManager &ZeusManager();

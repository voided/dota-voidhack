
#pragma once

#include <cstddef>


class IViewRender;


class CCameraManager
{

public:
	CCameraManager() :
		m_pViewRender( NULL )
	{
	}


	IViewRender *ViewRender() { return m_pViewRender; }

	void Init();
	void Shutdown();


private:
	float GetFarZ();


private:
	IViewRender *m_pViewRender;

};

CCameraManager &CameraManager();

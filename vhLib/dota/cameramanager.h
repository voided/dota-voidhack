
#pragma once

#include <cstddef>


class Vector;


class CCameraManager
{

public:
	void Init();
	void Shutdown();


private:
	float GetFarZ(); // IViewRender::GetZFar hook

};

CCameraManager &CameraManager();


#pragma once

#include <cstddef>


class Vector;


class CCameraManager
{

public:
	void Init();
	void Shutdown();

	bool GetVectorInScreenSpace( Vector pos, int &iX, int &iY, Vector *vecOffset = NULL );


private:
	float GetFarZ(); // IViewRender::GetZFar hook

};

CCameraManager &CameraManager();

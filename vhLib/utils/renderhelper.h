
#pragma once


#include <cstddef>


class IViewRender;
class IRenderView;


class CRenderHelper
{

public:
	CRenderHelper() :
		m_pViewRender( NULL ),
		m_pRenderView( NULL )
	{
	}


	void Init();
	void Shutdown();

	IViewRender *ViewRender() { return m_pViewRender; }
	IRenderView *RenderView() { return m_pRenderView; }


private:
	IViewRender *m_pViewRender;
	IRenderView *m_pRenderView;

};

CRenderHelper &RenderHelper();

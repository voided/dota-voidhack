
#pragma once


#include "utlvector.h"


class IViewRender;
class IVRenderView;
class CViewSetup;

namespace vgui
{
	class ISurface;
	class IVGuiPaintSurface;
	typedef unsigned long HFont; 
};


// a game manager that wishes to hook into rendering
class IRenderManager
{
public:
	virtual void RenderPreHud() { }
	virtual void RenderPostHud() { }
};


class CRenderHelper
{

public:
	CRenderHelper() :
		m_pViewRender( NULL ),
		m_pRenderView( NULL ),
		m_pSurface( NULL ),
		m_pPaintSurface( NULL )
	{
	}


	void Init();
	void Shutdown();

	// add and remove render hooks
	void AddRenderHook( const IRenderManager *manager );
	void RemoveRenderHook( const IRenderManager *manger );

	IViewRender *ViewRender() { return m_pViewRender; }
	IVRenderView *RenderView() { return m_pRenderView; }
	vgui::ISurface *Surface() { return m_pSurface; }
	vgui::IVGuiPaintSurface *PaintSurface() { return m_pPaintSurface; }


private:
	void PreVGui_Paint( int mode ); // IVRenderView::VGui_Paint pre hook
	void PostVGui_Paint( int mode ); // IVRenderView::VGui_Paint post hook


private:
	IViewRender *m_pViewRender;
	IVRenderView *m_pRenderView;

	vgui::ISurface *m_pSurface;
	vgui::IVGuiPaintSurface *m_pPaintSurface;

	CUtlVector<IRenderManager *> m_RenderHooks;

};

CRenderHelper &RenderHelper();

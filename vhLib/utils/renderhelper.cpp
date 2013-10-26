
#include "renderhelper.h"

#include "bytescanner.h"

#include "iviewrender.h"
#include "ivrenderview.h"
#include "view_shared.h"

#include "ienginevgui.h"
#include "vgui/ISurface.h"

#include "vh.h"
#include <sourcehook/sourcehook.h>


CRenderHelper &RenderHelper()
{
	static CRenderHelper renderHelper;
	return renderHelper;
}


SH_DECL_HOOK1_void( IVRenderView, VGui_Paint, SH_NOATTRIB, 0, int );


void CRenderHelper::Init()
{
	CByteScanner byteScan( "client" );

	void *pFunc = NULL;
	if ( byteScan.FindCodePattern( "\x55\x8B\xEC\xA1\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x33\xDB", "xxxx????xx????xxx", &pFunc ) )
	{
		m_pViewRender = *(IViewRender **)( (uint8 *)pFunc + 0x22D );

		if ( !m_pViewRender )
		{
			Warning( "[RenderHelper] Unable to find IViewRender!\n" );
		}
	}
	else
	{
		Warning( "[RenderHelper] Unable to find C_DOTACapture::RenderEditMode!\n" );
	}

	m_pRenderView = GetInterface<IVRenderView>( VH().GetFactoryInfo().engineFactory, VENGINE_RENDERVIEW_INTERFACE_VERSION );

	m_pSurface = GetInterface<vgui::ISurface>( VH().GetFactoryInfo().vguiFactory, VGUI_SURFACE_INTERFACE_VERSION );

	m_pPaintSurface = m_pSurface->GetVguiPaintSurface();
	Assert( m_pPaintSurface );

	SH_ADD_HOOK( IVRenderView, VGui_Paint, m_pRenderView, SH_MEMBER( this, &CRenderHelper::PreVGui_Paint ), false );
	SH_ADD_HOOK( IVRenderView, VGui_Paint, m_pRenderView, SH_MEMBER( this, &CRenderHelper::PostVGui_Paint ), true );
}

void CRenderHelper::Shutdown()
{
	SH_REMOVE_HOOK( IVRenderView, VGui_Paint, m_pRenderView, SH_MEMBER( this, &CRenderHelper::PostVGui_Paint ), true );
	SH_REMOVE_HOOK( IVRenderView, VGui_Paint, m_pRenderView, SH_MEMBER( this, &CRenderHelper::PreVGui_Paint ), false );

	m_pPaintSurface = NULL;
	m_pSurface = NULL;

	m_pRenderView = NULL;
}


void CRenderHelper::AddRenderHook( const IRenderManager *manager )
{
	m_RenderHooks.AddToTail( const_cast<IRenderManager *>( manager ) );
}

void CRenderHelper::RemoveRenderHook( const IRenderManager *manager )
{
	FOR_EACH_VEC( m_RenderHooks, i )
	{
		const IRenderManager *renderManager = m_RenderHooks[ i ];

		if ( renderManager == manager )
		{
			m_RenderHooks.Remove( i );
			break;
		}
	}
}


void CRenderHelper::PreVGui_Paint( int mode )
{
	if ( mode == PAINT_UIPANELS )
	{
		FOR_EACH_VEC( m_RenderHooks, i )
		{
			m_RenderHooks[ i ]->RenderPreHud();
		}
	}

	RETURN_META( MRES_IGNORED );
}

void CRenderHelper::PostVGui_Paint( int mode )
{
	if ( mode == PAINT_UIPANELS )
	{
		FOR_EACH_VEC( m_RenderHooks, i )
		{
			m_RenderHooks[ i ]->RenderPostHud();
		}
	}

	RETURN_META( MRES_IGNORED );
}

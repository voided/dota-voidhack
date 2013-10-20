
#include "renderhelper.h"

#include "bytescanner.h"

#include "vh.h"


CRenderHelper &RenderHelper()
{
	static CRenderHelper renderHelper;
	return renderHelper;
}


void CRenderHelper::Init()
{
	CByteScanner byteScan( "client" );

	void *pFunc = NULL;
	if ( byteScan.FindCodePattern( "\x55\x8B\xEC\xA1\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x33\xDB", "xxxx????xx????xxx", &pFunc ) )
	{
		m_pViewRender = *(IViewRender **)( (uint8 *)pFunc + 0x22D );
	}
	else
	{
		Warning( "[RenderHelper] Unable to find C_DOTACapture::RenderEditMode!\n" );
	}
}

void CRenderHelper::Shutdown()
{
	m_pViewRender = NULL;
}

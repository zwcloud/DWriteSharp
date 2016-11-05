#include "DwriteSharpNativeHelper.h"
#include <dwrite.h>
#include <d2d1.h>
#include "DebugUtility.h"

ID2D1Factory* pD2DFactory_ = nullptr;

HRESULT __stdcall DwriteCreateMeshTextRender(
	_In_ REFIID iid,
	_COM_Outptr_ IUnknown **textRender
	)
{
	if (textRender == nullptr)
		return E_POINTER;

	DebugAssert(iid == IDirectWriteMeshTextRenderer::IID_IDirectWriteMeshTextRenderer,
		"IID must be GUID of IDirectWriteMeshTextRenderer");

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_);
	DebugAssert(SUCCEEDED(hr), "D2D1CreateFactory Failed.");

	*textRender = new IDirectWriteMeshTextRenderer(pD2DFactory_);

	return S_OK;
}
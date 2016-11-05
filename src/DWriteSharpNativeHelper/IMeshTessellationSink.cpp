#include "IMeshTessellationSink.h"
#include "DebugUtility.h"
#include "DwriteSharpNativeHelper.h"
#include <vector>

// {BF47A681-2CA8-4E81-B7E2-CFBADE735A6B}
const GUID IMeshTessellationSink::IID_ICairoTessellationSink =
{ 0xbf47a681, 0x2ca8, 0x4e81,{ 0xb7, 0xe2, 0xcf, 0xba, 0xde, 0x73, 0x5a, 0x6b } };

IMeshTessellationSink::IMeshTessellationSink(__notnull RenderContext *cr_) : m_cRefCount(0), renderContext(cr_)
{
	DebugPrintf("IMeshTessellationSink created.");
}

IMeshTessellationSink::~IMeshTessellationSink(void)
{
}

STDMETHODIMP_(void) IMeshTessellationSink::AddTriangles(__in_ecount(trianglesCount) CONST D2D1_TRIANGLE *triangles, UINT trianglesCount)
{
	//DebugPrintf("triangles count: %d\n", trianglesCount);
	for (UINT i = 0; i < trianglesCount; ++i)
	{
		//DebugPrintf("[%d] (%.3f, %.3f) (%.3f, %.3f) (%.3f, %.3f) offset: (%.3f, %.3f)\n", i,
		//	triangles[i].point1.x, triangles[i].point1.y,
		//	triangles[i].point2.x, triangles[i].point2.y,
		//	triangles[i].point3.x, triangles[i].point3.y,
		//	renderContext->offsetX, renderContext->offsetY);

		renderContext->vPosition.push_back(triangles[i].point1.x + renderContext->offsetX);
		renderContext->vPosition.push_back(triangles[i].point1.y + renderContext->offsetY);
		renderContext->vPosition.push_back(triangles[i].point2.x + renderContext->offsetX);
		renderContext->vPosition.push_back(triangles[i].point2.y + renderContext->offsetY);
		renderContext->vPosition.push_back(triangles[i].point3.x + renderContext->offsetX);
		renderContext->vPosition.push_back(triangles[i].point3.y + renderContext->offsetY);
	}
}

STDMETHODIMP IMeshTessellationSink::Close()
{
	return S_OK;
}

STDMETHODIMP_(unsigned long) IMeshTessellationSink::AddRef()
{
	return InterlockedIncrement(&m_cRefCount);
}

STDMETHODIMP_(unsigned long) IMeshTessellationSink::Release()
{
	if (InterlockedDecrement(&m_cRefCount) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRefCount;
}

STDMETHODIMP IMeshTessellationSink::QueryInterface(IID const& riid, void** ppvObject)
{
	if (IID_ICairoTessellationSink == riid)
		*ppvObject = dynamic_cast<ID2D1TessellationSink*>(this);
	else
	if (__uuidof(IUnknown) == riid)
		*ppvObject = dynamic_cast<IUnknown*>(this);
	else
	{
		*ppvObject = nullptr;
		return E_FAIL;
	}
	return S_OK;
}
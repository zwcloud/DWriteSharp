#pragma once
#include <d2d1.h>

struct RenderContext;
struct IMeshTessellationSink : ID2D1TessellationSink
{
	static const GUID IID_ICairoTessellationSink;
	IMeshTessellationSink(__notnull RenderContext* renderContext);
	virtual ~IMeshTessellationSink(void);
	STDMETHOD_(void, AddTriangles)(
		__in_ecount(trianglesCount) CONST D2D1_TRIANGLE *triangles, UINT trianglesCount
		) override;
	STDMETHOD(Close)() override;

	#pragma region IUnknown interface
	unsigned long STDMETHODCALLTYPE AddRef() override;
	unsigned long STDMETHODCALLTYPE Release() override;
	HRESULT STDMETHODCALLTYPE QueryInterface(IID const& riid, void** ppvObject) override;
	#pragma endregion

private:
	LONG volatile m_cRefCount;
	RenderContext* renderContext;
};
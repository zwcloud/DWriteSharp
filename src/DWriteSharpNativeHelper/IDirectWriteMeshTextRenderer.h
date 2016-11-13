#pragma once
#include <dwrite.h>
#include <d2d1.h>

struct SimpleRenderContext;
struct IDirectWriteMeshTextRenderer : IDWriteTextRenderer
{
public:
	static const GUID IID_IDirectWriteMeshTextRenderer;
	IDirectWriteMeshTextRenderer(ID2D1Factory* pD2DFactory);
	virtual ~IDirectWriteMeshTextRenderer();

	STDMETHODIMP Clear();

	#pragma region IDWriteTextRenderer interface
	STDMETHOD(IsPixelSnappingDisabled)(__maybenull void*, __out BOOL*) override;
	STDMETHOD(GetCurrentTransform)(__maybenull void*, __out DWRITE_MATRIX*) override;
	STDMETHOD(GetPixelsPerDip)(__maybenull void*, __out FLOAT*) override;
	STDMETHOD(DrawGlyphRun)(__maybenull void*, FLOAT, FLOAT, DWRITE_MEASURING_MODE, __in const DWRITE_GLYPH_RUN*, __in const DWRITE_GLYPH_RUN_DESCRIPTION*, IUnknown*) override;
	STDMETHOD(DrawUnderline)(__maybenull void*, FLOAT, FLOAT, __in const DWRITE_UNDERLINE*, IUnknown*) override;
	STDMETHOD(DrawStrikethrough)(__maybenull void*, FLOAT, FLOAT, __in const DWRITE_STRIKETHROUGH*, IUnknown*) override;
	STDMETHOD(DrawInlineObject)(__maybenull void*, FLOAT, FLOAT, IDWriteInlineObject*, BOOL, BOOL, IUnknown*) override;
	#pragma endregion

	#pragma region IUnknown interface
	STDMETHODIMP_(ULONG) AddRef() override;
	STDMETHODIMP_(ULONG) Release() override;
	STDMETHODIMP QueryInterface(const IID& riid, void** object) override;
	#pragma endregion

private:
	ULONG volatile count_;
	ID2D1Factory* pD2DFactory_;
};

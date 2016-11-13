#include "IDirectWriteMeshTextRenderer.h"
#include "DebugUtility.h"
#include "ISimpleGeometrySink.h"
#include "Context.h"

//#define DEBUG_PRINT_GLYPH_INFO

// {1A87B373-3BD5-4E9C-BC2E-1811CE7248F2}
const GUID IDirectWriteMeshTextRenderer::IID_IDirectWriteMeshTextRenderer =
{ 0x1a87b373, 0x3bd5, 0x4e9c,{ 0xbc, 0x2e, 0x18, 0x11, 0xce, 0x72, 0x48, 0xf2 } };

ID2D1PathGeometry* pPathGeometry = nullptr;

IDirectWriteMeshTextRenderer::IDirectWriteMeshTextRenderer(ID2D1Factory* pD2DFactory)
	: count_{ 1 }
{
	pD2DFactory_ = pD2DFactory;
	pD2DFactory->AddRef();
}

IDirectWriteMeshTextRenderer::~IDirectWriteMeshTextRenderer()
{
	pD2DFactory_->Release();
}

STDMETHODIMP IDirectWriteMeshTextRenderer::Clear()//TODO remove this
{
	return 0x12345;
}

STDMETHODIMP IDirectWriteMeshTextRenderer::IsPixelSnappingDisabled(__maybenull void*, __out BOOL*)
{
	return S_OK;
}

STDMETHODIMP IDirectWriteMeshTextRenderer::GetCurrentTransform(__maybenull void*, __out DWRITE_MATRIX*)
{
	return S_OK;
}

STDMETHODIMP IDirectWriteMeshTextRenderer::GetPixelsPerDip(__maybenull void*, __out FLOAT*)
{
	return S_OK;
}

STDMETHODIMP IDirectWriteMeshTextRenderer::DrawGlyphRun(
	void* clientDrawingContext,
	FLOAT baselineOriginX,
	FLOAT baselineOriginY,
	DWRITE_MEASURING_MODE measuringMode,
	const DWRITE_GLYPH_RUN* glyphRun,
	const DWRITE_GLYPH_RUN_DESCRIPTION* glyphRunDescription,
	IUnknown* clientDrawingEffect)
{
	DebugPrintf("IDirectWriteMeshTextRenderer::DrawGlyphRun Called. baselineOrigin: (%.3f, %.3f)\n", baselineOriginX, baselineOriginY);
	
	HRESULT hr;

	if (clientDrawingContext == nullptr) {
		return ERROR_BAD_ARGUMENTS;
	}
	
	auto context = (SimpleRenderContext*)clientDrawingContext;

	//Update context
	context->offsetX = baselineOriginX;
	context->offsetY = baselineOriginY;

	//Add data by callbacks
	hr = pD2DFactory_->CreatePathGeometry(&pPathGeometry);
	DebugAssert(SUCCEEDED(hr), "pD2DFactory_->CreatePathGeometry failed.");
	{
		ISimpleGeometrySink *pSGSink = new ISimpleGeometrySink(context);
		DebugAssert(SUCCEEDED(hr), "ID2D1GeometrySink::Open failed.");

		hr = glyphRun->fontFace->GetGlyphRunOutline(
			            glyphRun->fontEmSize,
			            glyphRun->glyphIndices,
			            glyphRun->glyphAdvances,
			            glyphRun->glyphOffsets,
			            glyphRun->glyphCount,
			            glyphRun->isSideways,
			            glyphRun->bidiLevel % 2,
						pSGSink);
		DebugAssert(SUCCEEDED(hr), "glyphRun->fontFace->GetGlyphRunOutline failed.");

		hr = pSGSink->Close();
		DebugAssert(SUCCEEDED(hr), "pSGSink->Close failed");

		pSGSink->Release();
	}
	pPathGeometry->Release();

#ifdef  DEBUG_PRINT_GLYPH_INFO
	static wchar_t* enumStringsOfmeasuringMode[] =
	{
		L"DWRITE_MEASURING_MODE_NATURAL",
		L"DWRITE_MEASURING_MODE_GDI_CLASSIC",
		L"DWRITE_MEASURING_MODE_GDI_NATURAL"
	};
	DebugPrintf(L"Baseline X,Y: (%.3f, %.3f)\n", baselineOriginX, baselineOriginY);
	DebugPrintf(L"Measuring Mode: %s\n", enumStringsOfmeasuringMode[measuringMode]);

	for (int i = 0; i < glyphRun->glyphCount; ++i)
	{
		DebugPrintf(L"----GlyphRun %d----\n", i);
		DebugPrintf(L"Glyph Index: %d\n", glyphRun->glyphIndices[i]);
		DebugPrintf(L"Glyph Advance: %d\n", glyphRun->glyphAdvances[i]);
		DebugPrintf(L"Glyph Advance offset: %d Ascender offset:%d\n", glyphRun->glyphOffsets[i].advanceOffset,
			glyphRun->glyphOffsets[i].ascenderOffset);
	}

	DebugPrintf(L"----GlyphRun Description----\n");
	DebugPrintf(L"	Locale name: %s\n", glyphRunDescription->localeName);
	DebugPrintf(L"	String: %s\n", glyphRunDescription->string);
	DebugPrintf(L"	String length: %d\n", glyphRunDescription->stringLength);
	DebugPrintf(L"	Cluster map:\n		");
	for (int i = 0; i < glyphRunDescription->stringLength; ++i)
	{
		DebugPrintf(L"%d, ", glyphRunDescription->clusterMap[i]);
	}
	DebugPrintf(L"\n");
	DebugPrintf(L"	Text position: %d\n", glyphRunDescription->textPosition);
#endif

	DebugPrintf("IDirectWriteMeshTextRenderer::DrawGlyphRun returned.\n");
	return S_OK;
}

STDMETHODIMP IDirectWriteMeshTextRenderer::DrawUnderline(__maybenull void*, FLOAT, FLOAT, const DWRITE_UNDERLINE*, IUnknown*)
{ return S_OK; }

STDMETHODIMP IDirectWriteMeshTextRenderer::DrawStrikethrough(__maybenull void*, FLOAT, FLOAT, const DWRITE_STRIKETHROUGH*, IUnknown*)
{ return S_OK; }

STDMETHODIMP IDirectWriteMeshTextRenderer::DrawInlineObject(__maybenull void*, FLOAT, FLOAT, IDWriteInlineObject*, BOOL, BOOL, IUnknown*)
{ return S_OK; }

#pragma region IUnknown interface
STDMETHODIMP_(unsigned long) IDirectWriteMeshTextRenderer::AddRef()
{ return InterlockedIncrement(&count_); }

STDMETHODIMP_(unsigned long) IDirectWriteMeshTextRenderer::Release()
{
	const auto count = InterlockedDecrement(&count_);
	if (!count) {
		delete this;
	}
	return count;
}

STDMETHODIMP IDirectWriteMeshTextRenderer::QueryInterface(const IID& riid, void** object)
{
	if (object == nullptr)
		return E_POINTER;
	if (__uuidof(IDWriteTextRenderer) == riid || IID_IDirectWriteMeshTextRenderer == riid || __uuidof(IUnknown) == riid)
	{
		*object = this;
		this->AddRef();
		return S_OK;
	}
	*object = nullptr;
	return E_FAIL;
}
#pragma endregion

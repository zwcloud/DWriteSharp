#pragma once
#include <dwrite.h>
#include <d2d1.h>

struct SimpleRenderContext;
class ISimpleGeometrySink : public IDWriteGeometrySink
{
public:
	static const GUID IID_ISimpleGeometrySink;

	ISimpleGeometrySink(__notnull SimpleRenderContext* renderContext);

	virtual ~ISimpleGeometrySink();

	STDMETHOD_(void, SetFillMode)(
		D2D1_FILL_MODE fillMode
		) override;

	STDMETHOD_(void, SetSegmentFlags)(
		D2D1_PATH_SEGMENT vertexFlags
		) override;

	STDMETHOD_(void, BeginFigure)(
		D2D1_POINT_2F startPoint,
		D2D1_FIGURE_BEGIN figureBegin
		) override;

	STDMETHOD_(void, AddLines)(
		_In_reads_(pointsCount) CONST D2D1_POINT_2F *points,
		UINT32 pointsCount
		) override;

	STDMETHOD_(void, AddBeziers)(
		_In_reads_(beziersCount) CONST D2D1_BEZIER_SEGMENT *beziers,
		UINT32 beziersCount
		) override;

	STDMETHOD_(void, EndFigure)(
		D2D1_FIGURE_END figureEnd
		) override;

	STDMETHOD(Close)(
		) override;

	#pragma region IUnknown interface
	unsigned long STDMETHODCALLTYPE AddRef() override;
	unsigned long STDMETHODCALLTYPE Release() override;
	HRESULT STDMETHODCALLTYPE QueryInterface(IID const& riid, void** ppvObject) override;
	#pragma endregion

private:
	LONG volatile m_cRefCount;
	SimpleRenderContext* context;
};


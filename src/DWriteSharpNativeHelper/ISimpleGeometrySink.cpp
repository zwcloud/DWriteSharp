#include "ISimpleGeometrySink.h"
#include "Context.h"
#include "DebugUtility.h"

// {6092176D-4F27-4600-BD56-B296DD1B3724}
const GUID ISimpleGeometrySink::IID_ISimpleGeometrySink =
{ 0x6092176d, 0x4f27, 0x4600,{ 0xbd, 0x56, 0xb2, 0x96, 0xdd, 0x1b, 0x37, 0x24 } };

ISimpleGeometrySink::ISimpleGeometrySink(__notnull SimpleRenderContext *context) : m_cRefCount(1), context(context)
{
}


ISimpleGeometrySink::~ISimpleGeometrySink()
{
}

STDMETHODIMP_(void) ISimpleGeometrySink::SetFillMode(D2D1_FILL_MODE fillMode)
{
	DebugPrintf("SetFillMode %d\n", fillMode);
}

STDMETHODIMP_(void) ISimpleGeometrySink::SetSegmentFlags(D2D1_PATH_SEGMENT vertexFlags)
{
	DebugPrintf("SetSegmentFlags %d\n", vertexFlags);
}

STDMETHODIMP_(void) ISimpleGeometrySink::BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN figureBegin)
{
	DebugPrintf("BeginFigure %d\n", figureBegin);
	this->context->figureBegin(this->context->offsetX + startPoint.x, this->context->offsetY + startPoint.y);
}

STDMETHODIMP_(void) ISimpleGeometrySink::AddLines(CONST D2D1_POINT_2F * points, UINT32 pointsCount)
{
	for (int i = 0; i < pointsCount; ++i)
	{
		const D2D1_POINT_2F point = points[i];
		this->context->addPoint(this->context->offsetX + point.x, this->context->offsetY + point.y);
	}
}

STDMETHODIMP_(void) ISimpleGeometrySink::AddBeziers(CONST D2D1_BEZIER_SEGMENT * beziers, UINT32 beziersCount)
{
	for (int i = 0; i < beziersCount; ++i)
	{
		const D2D1_BEZIER_SEGMENT bezier = beziers[i];
		this->context->addBezierSegment(
			this->context->offsetX + bezier.point1.x, this->context->offsetY + bezier.point1.y,
			this->context->offsetX + bezier.point2.x, this->context->offsetY + bezier.point2.y,
			this->context->offsetX + bezier.point3.x, this->context->offsetY + bezier.point3.y);
	}
}

STDMETHODIMP_(void) ISimpleGeometrySink::EndFigure(D2D1_FIGURE_END figureEnd)
{
	this->context->pathClose();
	this->context->figureEnd();
}

STDMETHODIMP ISimpleGeometrySink::Close()
{
	return S_OK;
}

STDMETHODIMP_(unsigned long) ISimpleGeometrySink::AddRef()
{
	return InterlockedIncrement(&m_cRefCount);
}

STDMETHODIMP_(unsigned long) ISimpleGeometrySink::Release()
{
	if (InterlockedDecrement(&m_cRefCount) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRefCount;
}

STDMETHODIMP ISimpleGeometrySink::QueryInterface(IID const& riid, void** ppvObject)
{
	if (IID_ISimpleGeometrySink == riid)
		*ppvObject = dynamic_cast<IDWriteGeometrySink*>(this);
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
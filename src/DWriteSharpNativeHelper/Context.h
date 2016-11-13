#pragma once

struct SimpleRenderContext
{
	float offsetX;
	float offsetY;

	void(*addPoint) (float x, float y);
	void(*addBezierSegment) (float x0, float y0, float x1, float y1, float x2, float y2);
	void(*pathClose)();

	void(*figureBegin)(float x, float y);
	void(*figureEnd)();
};
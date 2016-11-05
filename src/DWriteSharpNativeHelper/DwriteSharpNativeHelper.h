#pragma once
#include "IDirectWriteMeshTextRenderer.h"
#include <vector>

struct Context
{
	float* positionBuffer;
	int positionBufferLength;
	std::vector<float>* vPosition;
	float offsetX;
	float offsetY;
};

/// <summary>
/// Creates a MeshTextRender object.
/// </summary>
/// <returns>
/// Standard HRESULT error code.
/// </returns>
extern "C" HRESULT __stdcall DwriteCreateMeshTextRender(
	_In_ REFIID iid,
	_COM_Outptr_ IUnknown **textRender
	);

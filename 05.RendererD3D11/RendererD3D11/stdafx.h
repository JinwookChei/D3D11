#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif  // _DEBUG

#define SYSTEM_API __declspec(dllimport)

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <vector>

#include <System/Math.h>

#include <Renderer/IRenderer.h>

#include "D3D11Renderer.h"

extern D3D11Renderer* GRenderer;
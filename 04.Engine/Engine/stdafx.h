#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif  // _DEBUG

#define ENGINE_API __declspec(dllexport)
#define SYSTEM_API __declspec(dllimport)

#include <Windows.h>
#include <vector>

#include <System/Math.h>
#include <System/LinkedList.h>
#include <System/HashTable.h>

#include <Window/IApplication.h>
#include <Renderer/IRenderer.h>

#include "InputManager.h"

extern InputManager* GInputManager;
extern IRenderer* GRenderer;
extern class Camera* GCurrentCamera;
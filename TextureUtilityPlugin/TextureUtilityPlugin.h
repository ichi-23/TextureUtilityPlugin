#pragma once

#define TEXTURE_UTILITY_PLUGIN_EXPORTS

#ifdef TEXTURE_UTILITY_PLUGIN_EXPORTS
  #define TEXTURE_UTILITY_PLUGIN_API extern "C" __declspec(dllexport)
#else
  #define TEXTURE_UTILITY_PLUGIN_API extern "C" __declspec(dllimport)
#endif

#include "stdafx.h"
#include <cstring>
#include <d3d11.h>
#include "IUnityInterface.h"
#include "IUnityGraphics.h"
#include "IUnityGraphicsD3D11.h"
#include <wrl/client.h>

TEXTURE_UTILITY_PLUGIN_API void SetTextureFromUnity(void* texture, int width, int height);

TEXTURE_UTILITY_PLUGIN_API void SetByteOutDstFromUnity(void* out);

using DebugLogFuncType = void(*)(const char*);

namespace
{
	DebugLogFuncType debugLogFunc = nullptr;
}

TEXTURE_UTILITY_PLUGIN_API void SetDebugLogFunc(DebugLogFuncType func);

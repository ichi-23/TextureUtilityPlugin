#pragma once

#include "IUnityInterface.h""
#include "IUnityGraphics.h"
#include "IUnityGraphicsD3D11.h"
#include "stdafx.h"
#include <d3d11.h>

#define TEXTURE_UTILITY_PLUGIN_EXPORTS

#ifdef TEXTURE_UTILITY_PLUGIN_EXPORTS
  #define TEXTURE_UTILITY_PLUGIN_API extern "C" __declspec(dllexport)
#else
  #define TEXTURE_UTILITY_PLUGIN_API extern "C" __declspec(dllimport)
#endif


TEXTURE_UTILITY_PLUGIN_API void SetTexture(void* texture);

TEXTURE_UTILITY_PLUGIN_API void WriteTextureToBytes(BYTE* array);

TEXTURE_UTILITY_PLUGIN_API void Finalize();



#include "TextureUtilityPlugin.h"
#include <comdef.h>
#include <atlstr.h>
#include <string>
#include <sstream>

using namespace Microsoft::WRL;

static ID3D11Texture2D* targetTexture_ = nullptr;
static BYTE* outDst = nullptr;
static int texWidth = 0;
static int texHeight = 0;

static ComPtr<ID3D11Texture2D> tempTexture_;

static IUnityInterfaces* s_UnityInterfaces = nullptr;
static IUnityGraphics* s_Graphics = nullptr;
static ID3D11Device* s_Device;

static void DebugLog(const char* msg)
{
	if (debugLogFunc != nullptr)
	{
		debugLogFunc(msg);
	}
}

static void DebugAddress(const void* address)
{
	std::stringstream ss;
	ss << address;
	std::string addr_str = ss.str();
	const char * addr_chr = addr_str.c_str();
	debugLogFunc(addr_chr);
	ss.clear();
}


TEXTURE_UTILITY_PLUGIN_API void SetDebugLogFunc(DebugLogFuncType func)
{
	debugLogFunc = func;
}


TEXTURE_UTILITY_PLUGIN_API void SetTextureFromUnity(void* texture, int width, int height)
{
	targetTexture_ = reinterpret_cast<ID3D11Texture2D*>(texture);
	texWidth = width;
	texHeight = height;
}

TEXTURE_UTILITY_PLUGIN_API void SetByteOutDstFromUnity(void* out)
{
	outDst = reinterpret_cast<BYTE*>(out);
}

static void WriteRenderTextureToBytes()
{
	if (!targetTexture_)
	{
		DebugLog("target unity texture pointer is null.");
		return;
	}

	if (!tempTexture_)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = texWidth;
		desc.Height = texHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		if (FAILED(s_Device->CreateTexture2D(&desc, nullptr, &tempTexture_)))
		{
			DebugLog("CreateTexture2D() failed.");
			return;
		}
	}

	{
		ComPtr<ID3D11DeviceContext> context;
		s_Device->GetImmediateContext(&context);
		context->CopyResource(tempTexture_.Get(), targetTexture_);
	}

	ComPtr<IDXGISurface> surface;
	if (FAILED(tempTexture_.As(&surface)))
	{
		DebugLog("tempTexture_.As failed.");
		return;
	}

	DXGI_MAPPED_RECT mappedSurface;
	if (FAILED(surface->Map(&mappedSurface, DXGI_MAP_READ)))
	{
		DebugLog("surface->Map() failed.");
		return;
	}

	// UINT because 8 bit per channel -> 32bit
	const UINT size = texWidth * texHeight * sizeof(UINT);

	std::memcpy(outDst, mappedSurface.pBits, size);

	if (FAILED(surface->Unmap()))
	{
		DebugLog("surface->Unmap() failed: ");
		return;
	}

}

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	switch (eventType)
	{
		case kUnityGfxDeviceEventInitialize:
		{
			s_Device = s_UnityInterfaces ->Get<IUnityGraphicsD3D11>()->GetDevice();
			break;
		}
	}
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	s_UnityInterfaces = unityInterfaces;
	s_Graphics = unityInterfaces->Get<IUnityGraphics>();
	s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

	// in case the graphics device is already initialized
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginUnload()
{
	s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
	s_UnityInterfaces = nullptr;
}

static void UNITY_INTERFACE_API OnRenderEvent(int eventID)
{
	WriteRenderTextureToBytes();
}

extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc()
{
	return OnRenderEvent;
}





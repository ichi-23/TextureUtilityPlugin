// TextureUtilityPlugin.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "TextureUtilityPlugin.h"

ID3D11Texture2D* targetTexture_ = nullptr;
D3D11_MAP MapType = D3D11_MAP_READ;
D3D11_MAPPED_SUBRESOURCE mappedResource;

static IUnityInterfaces* s_UnityInterfaces = nullptr;
static IUnityGraphics* s_Graphics = nullptr;
ID3D11Device* s_Device;
ID3D11DeviceContext* s_deviceContext = nullptr;

TEXTURE_UTILITY_PLUGIN_API void SetTexture(void* texture)
{
	targetTexture_ = reinterpret_cast<ID3D11Texture2D*>(texture);
}

TEXTURE_UTILITY_PLUGIN_API void WriteTextureToBytes(BYTE* array)
{

}

TEXTURE_UTILITY_PLUGIN_API void Finalize()
{
	targetTexture_ = nullptr;
	// other finalization tasks...

}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	s_UnityInterfaces = unityInterfaces;
	s_Graphics = unityInterfaces->Get<IUnityGraphics>();
	s_Graphics->RegisterDeviceEventCallback(UnityOnGraphicsDeviceEvent);

	// in case the graphics device is already initialized
	UnityOnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginUnload()
{
	s_Graphics->UnregisterDeviceEventCallback(UnityOnGraphicsDeviceEvent);
	s_UnityInterfaces = nullptr;
}

static void UNITY_INTERFACE_API UnityOnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	switch (eventType)
	{
		case kUnityGfxDeviceEventInitialize:
		{
			s_Device = s_UnityInterfaces ->Get<IUnityGraphicsD3D11>()->GetDevice();
			break;
		}

		case kUnityGfxDeviceEventShutdown:
		{
			// something
			break;
		}
	}
}

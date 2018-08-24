#pragma once
#ifndef GRAPHICS
#define GRAPHICS
#include <wrl.h>
#include <D3D11.h>
#include <string>

class Color {
public:
	unsigned int data = 0xFFFFFFFF;
	constexpr Color(int r, int g, int b, int a)
		:data((a << 24) | (r << 16) | (g << 8) | b)
	{
	}
	constexpr Color(int r, int g, int b)
		: data((r << 16) | (g << 8) | b)
	{
	}
	constexpr Color() : data() {};
};
class Graphics {
public:
	struct FSQVertex
	{
		float x, y, z;		// position
		float u, v;			// texcoords
	};
	Graphics(HWND hWnd, unsigned int width, unsigned int height);
	~Graphics();
	void BeginFrame(void);
	void EndFrame(void);

	//Drawing functions
	void DrawPixel(int x, int y, int r, int g, int b);
	void DrawPixel(int x, int y, Color c);
	void DrawCircle(int xCenter, int yCenter, int radius, Color c);
	void FillCircle(int xCenter, int yCenter, int radius, Color c);

	void DrawLine(int x1, int y1, int x2, int y2, Color c);
	void DrawRectangle(int xTopLeft, int yTopLeft, int width, int height, Color c);
	void FillRectangle(int xTopLeft, int yTopLeft, int width, int height, Color c);
	void DrawBMP(int x, int y, int drawWidth, int drawHeight, std::string fileName); //Change so bmp is not read every time
private:
	Color* pixelBuffer = NULL;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				 swapchain;             // the pointer to the swap chain interface
	Microsoft::WRL::ComPtr<ID3D11Device>				gfxDevice;                     // the pointer to our Direct3D device interface
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			gfxDeviceCon;           // the pointer to our Direct3D device context
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				systemBuffer;			// the pointer to our back buffer
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	sysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
	D3D11_MAPPED_SUBRESOURCE	mappedSysBufferTexture;
	bool wrapHorizontal = false;
	bool wrapVertical = false;
	unsigned int width;
	unsigned int height;
};
#endif
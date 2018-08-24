#include "Graphics.h"
#include <stdio.h>
namespace Shaders
{
	#include "normalPxShader.shh"
	#include "normalVxShader.shh";
}
#pragma comment( lib,"d3d11.lib" )
using Microsoft::WRL::ComPtr;
Graphics::Graphics(HWND hWnd,unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;


	D3D_FEATURE_LEVEL	featureLevelsRequested = D3D_FEATURE_LEVEL_9_1;
	UINT				numLevelsRequested = 1;
	D3D_FEATURE_LEVEL	featureLevelsSupported;
	HRESULT				hr;
	UINT				createFlags = 0u;
	#ifdef _DEBUG
	#ifdef USE_DIRECT3D_DEBUG_RUNTIME
		createFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
	#endif
	
	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createFlags,
		&featureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&swapchain,
		&gfxDevice,
		&featureLevelsSupported,
		&gfxDeviceCon);


	// get the address of the back buffer
	ComPtr<ID3D11Resource> pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	gfxDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &renderTargetView);
	// set the render target as the back buffer
	gfxDeviceCon->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), NULL);


	//Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;	
	gfxDeviceCon->RSSetViewports(1, &viewport);

	//The system texture (not that relevant in 2D)
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = width;
	sysTexDesc.Height = height;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;

	gfxDevice->CreateTexture2D(&sysTexDesc, nullptr, &systemBuffer);


	//Shaders
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	gfxDevice->CreateShaderResourceView(systemBuffer.Get(), &srvDesc, &sysBufferTextureView);
	gfxDevice->CreatePixelShader(Shaders::normalPxShaderBytecode,sizeof(Shaders::normalPxShaderBytecode), NULL, &pixelShader);
	gfxDevice->CreateVertexShader(Shaders::normalVxShaderBytecode, sizeof(Shaders::normalVxShaderBytecode), NULL, &vertexShader);
	

	//Initialize the device
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FSQVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	gfxDevice->CreateBuffer(&bd, &initData, &vertexBuffer);

	//Input layout (determines data format of vertex)
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	gfxDevice->CreateInputLayout(ied, 2, Shaders::normalVxShaderBytecode, sizeof(Shaders::normalVxShaderBytecode), &inputLayout);


	//Sampler
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	gfxDevice->CreateSamplerState(&sampDesc, &samplerState);


	//Allocate buffer memory
	pixelBuffer = reinterpret_cast<Color*>(_aligned_malloc(sizeof(Color) * width * height, 16u));
}




void Graphics::BeginFrame(void)
{
	memset(pixelBuffer, 0x00, sizeof(Color) * width * height);	//Clear the screen
}

void Graphics::EndFrame(void)
{
	//Write the pixelbuffer to memory, do this by locking the backbuffer
	gfxDeviceCon->Map(systemBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture);
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData);
	const size_t dstPitch = mappedSysBufferTexture.RowPitch/sizeof(Color);
	const size_t srcPitch = width;
	const size_t rowBytes = srcPitch * sizeof(Color);
	for (size_t y = 0u; y < height; y++)
	{
		memcpy(&pDst[y*dstPitch], &pixelBuffer[y * srcPitch], rowBytes);
	}
	gfxDeviceCon->Unmap(systemBuffer.Get(), 0u);

	//Render frame
	const UINT stride = sizeof(FSQVertex);
	const UINT offset = 0u;
	gfxDeviceCon->IASetInputLayout(inputLayout.Get());
	gfxDeviceCon->VSSetShader(vertexShader.Get(), nullptr, 0u);
	gfxDeviceCon->PSSetShader(pixelShader.Get(), nullptr, 0u);
	gfxDeviceCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfxDeviceCon->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
	gfxDeviceCon->PSSetShaderResources(0u, 1u, sysBufferTextureView.GetAddressOf());
	gfxDeviceCon->Draw(6u, 0u);

	//Swap the back and frontbuffer
	swapchain->Present(0, 0);
}


void Graphics::DrawPixel(int x, int y, int r, int g, int b)
{
	DrawPixel(x, y, Color(r, g, b));
}
void  Graphics::DrawPixel(int x, int y, Color color)
{
	if ((x < 0 || x > width) && wrapHorizontal == true)
	{
		x = x % width;
	}
	if ((y < 0 || y > height) && wrapVertical == true)
	{
		y = y % height;
	}
	if (x > 0 && y > 0 && x < width && y < height)
	{
		long location = y*width + x;
		pixelBuffer[location] = color;
	}
}
//Draw a line using Bresenhams algoritm
void Graphics::DrawLine(int x1, int y1, int x2, int y2, Color c)
{
	int dx = abs(x2 - x1), sx = x1<x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1<y2 ? 1 : -1;
	int err = (dx>dy ? dx : -dy) / 2, e2;

	for (;;) {
		DrawPixel(x1, y1,c);
		if (x1 == x2 && y1 == y2) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x1 += sx; }
		if (e2 < dy) { err += dx; y1 += sy; }
	}
}

void Graphics::FillCircle(int xCenter, int yCenter, int radius, Color c)
{
	int xStart = xCenter - radius;
	int yStart = yCenter - radius;

	int radSquared = radius*radius;
	for (int i = -radius; i < radius; i++)
	{
		for (int j = -radius; j < radius; j++)
		{
			if (i*i + j*j < radSquared)
			{
				DrawPixel(i + xStart + radius, j + yStart + radius, c);
			}
		}
	}


}
//Draw a circle using Bresenhams algoritm
void Graphics::DrawCircle(int xCenter, int yCenter, int radius, Color c)
{
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y)
	{
		DrawPixel(xCenter + x, yCenter + y,c);
		DrawPixel(xCenter + y, yCenter + x,c);
		DrawPixel(xCenter - y, yCenter + x,c);
		DrawPixel(xCenter - x, yCenter + y,c);
		DrawPixel(xCenter - x, yCenter - y, c);
		DrawPixel(xCenter - y, yCenter - x, c);
		DrawPixel(xCenter + y, yCenter - x, c);
		DrawPixel(xCenter + x, yCenter - y, c);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}
}
void Graphics::FillRectangle(int xTopLeft, int yTopLeft, int width, int height, Color c)
{
	if (width < 0)
	{
		width = -width;
		xTopLeft -= width;
	}
	if (height < 0)
	{
		height = -height;
		yTopLeft -= height;
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			DrawPixel(xTopLeft + i, yTopLeft + j, c);
		}
	}
}
void Graphics::DrawRectangle(int xTopLeft, int yTopLeft, int width, int height, Color c)
{

	DrawLine(xTopLeft, yTopLeft, xTopLeft + width, yTopLeft, c);
	DrawLine(xTopLeft + width, yTopLeft, xTopLeft + width, yTopLeft + height, c);
	DrawLine(xTopLeft + width, yTopLeft + height, xTopLeft, yTopLeft + height, c);
	DrawLine(xTopLeft, yTopLeft + height, xTopLeft, yTopLeft, c);
}


//from internet, should look into more
 void Graphics::DrawBMP(int x, int y,int drawWidth, int drawHeight, std::string fileName)
{
	int i;

	FILE* f;
	fopen_s(&f, fileName.c_str(), "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

											   // extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;
	float wRatio = (float)drawWidth / (float)width;
	float hRatio = (float)drawHeight / (float)height;
	int size = 3 * width * height;
	for (int i = 0; i < height; i++)
	{

		fread(data, sizeof(unsigned char), row_padded, f);
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;
			for (int k = 0; k < wRatio; k++)
			{
				for (int l = 0; l < wRatio; l++)
				{
					DrawPixel(x + k +  ((j / 3)*wRatio), y + l +((height - i)*hRatio), Color((int)data[j], (int)data[j + 1], (int)data[j + 2]));
				}
			}
		}
	}

	fclose(f);
}
 
// this is the function that cleans up Direct3D and COM
Graphics::~Graphics(void)
{
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
	if (pixelBuffer)
	{
		_aligned_free(pixelBuffer);
		pixelBuffer = nullptr;
	}

	if (gfxDeviceCon != NULL)
	{
		gfxDeviceCon->ClearState();
	}
}

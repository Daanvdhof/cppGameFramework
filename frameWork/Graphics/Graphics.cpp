#include <stdio.h>
#include "Graphics.h"

namespace Shaders
{
	#include "normalPxShader.shh"
	#include "normalVxShader.shh"
}
#pragma comment(lib,"d3d11.lib" )

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
	sd.SampleDesc.Count = 1;		//Anti alliasing number of multisamples
	sd.SampleDesc.Quality = 0;		//Anti alliasing quality
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL	featureLevelsRequested = D3D_FEATURE_LEVEL_9_1;
	UINT				numLevelsRequested = 1;
	D3D_FEATURE_LEVEL	featureLevelsSupported;
	UINT				createFlags = 0u;
	#ifdef _DEBUG
		#ifdef USE_DIRECT3D_DEBUG_RUNTIME
			createFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif
	#endif
	
	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(
		NULL,						//Pointer to video adapter, NULL is default adapter
		D3D_DRIVER_TYPE_HARDWARE,	//The directx driver type
		NULL,						//software rasteriser dll, is null if driver type is not software
		createFlags,				//Run time layers (core, debug, etc.)
		&featureLevelsRequested,	//What direct x features are needed, can pass in an ordered array
		numLevelsRequested,			//number of elements in the feature levels requested
		D3D11_SDK_VERSION,			//SDK version
		&sd,						//Swap chain description pointer
		&swapchain,					//Pointer to create swap chain
		&gfxDevice,					//Pointer to created graphics device 
		&featureLevelsSupported,	//Returns the adress of the first element of an array containing the supported feature levels
		&gfxDeviceCon);				//Pointer to the created devicecontext

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
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;	
	gfxDeviceCon->RSSetViewports(1, &viewport);

	//The system texture (not that relevant in 2D)
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = width;
	sysTexDesc.Height = height;
	sysTexDesc.MipLevels = 1;	//Max number of mipmap levels in texture, 1 for multisampled, 0 for full set of subtextures
	sysTexDesc.ArraySize = 1;	//Number of textures
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	//Texture format
	sysTexDesc.SampleDesc.Count = 1;	//Multisample level
	sysTexDesc.SampleDesc.Quality = 0;	//Multisample quality
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;	//Dynamic means CPU write only and GPU read only
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//Bind to a shader
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//Type of CPU acces allowed
	sysTexDesc.MiscFlags = 0;	//Irrelevant
	gfxDevice->CreateTexture2D(&sysTexDesc, nullptr, &systemBuffer);	//Create the system texture and store it in systemBuffer

	//Shaders
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;	//Specify the 
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	
	srvDesc.Texture2D.MipLevels = 1;

	gfxDevice->CreateShaderResourceView(systemBuffer.Get(), &srvDesc, &sysBufferTextureView);
	gfxDevice->CreatePixelShader(Shaders::normalPxShaderBytecode,sizeof(Shaders::normalPxShaderBytecode), NULL, &pixelShader);
	gfxDevice->CreateVertexShader(Shaders::normalVxShaderBytecode, sizeof(Shaders::normalVxShaderBytecode), NULL, &vertexShader);
	
	//Initialize the device
	//Make the canvas in 3D to which the 2D canvas i mapped
	

	const FSQVertex vertices[] =
	{
		{-1.0f,+1.0f,0.5f,0.0f,0.0f },
		{+1.0f,+1.0f,0.5f,1.0f,0.0f },
		{+1.0f,-1.0f,0.5f,1.0f,1.0f },
		{-1.0f,+1.0f,0.5f,0.0f,0.0f },
		{+1.0f,-1.0f,0.5f,1.0f,1.0f },
		{-1.0f,-1.0f,0.5f,0.0f,1.0f },
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
	const int dstPitch = mappedSysBufferTexture.RowPitch/sizeof(Color);
	const int srcPitch = width;
	const int rowBytes = srcPitch * sizeof(Color);
	for (int y = 0u; y < height; y++)
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
	swapchain->Present(1, 0);
}

Color Graphics::GetPixel(int x, int y) const
{
	Color result = Color(0, 0, 0);
	if (x < width && y < height && x >= 0 && y >= 0)
	{
		result = pixelBuffer[width*y + x];
	}
	return result;
}

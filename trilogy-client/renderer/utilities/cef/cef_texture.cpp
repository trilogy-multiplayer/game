#include "cef_texture.hpp"

#include <mutex>
#include <list>
#include <renderer/renderer.hpp>

const char renderer::utilities::cef::c_cef_texture::m_pixelShaderSource[] = {
	"Texture2D diffuse;\
	sampler sampler0;\
	\
	struct INPUT \
	{\
		float2 uv : TEXCOORD; \
	};\
	\
	float4 main(INPUT input) : SV_TARGET\
	{\
		float4 output = diffuse.Sample(sampler0, input.uv); \
		return output; \
	}"
};

const char renderer::utilities::cef::c_cef_texture::m_vertexShaderSource[] = {
	"struct INPUT\
		{\
			float2 pos : POSITION;\
			float2 uv : TEXCOORD;\
		};\
		\
		struct OUTPUT \
		{ \
			float2 uv : TEXCOORD; \
			float4 pos : SV_POSITION; \
		}; \
		\
		OUTPUT main(INPUT input)\
		{\
			OUTPUT output; \
			output.uv = input.uv; \
			output.pos = float4(input.pos.x, input.pos.y, 0.0f, 1.0f);\
			return output;\
		}"
};

bool renderer::utilities::cef::c_cef_texture::create_shaders()
{
	static auto renderer = c_renderer::instance();

	ID3DBlob* ErrorMsg = nullptr;
	if (FAILED(D3DCompile(m_pixelShaderSource, strlen(m_pixelShaderSource), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &m_pixelBlob, &ErrorMsg)))
	{
		const char* const ERROR_BASE = "Failed to compile pixel shader.";
		char* const error = new char[strlen(ERROR_BASE) + ErrorMsg->GetBufferSize() + strlen(m_pixelShaderSource) + 1 + 2];
		sprintf(error, "%s\n%s\n%s", ERROR_BASE, (const char*)ErrorMsg->GetBufferPointer(), m_pixelShaderSource);
		MessageBoxA(NULL, error, NULL, NULL);
		delete[]error;
		return false;
	}

	if (FAILED(renderer->d3d11_device->CreatePixelShader(m_pixelBlob->GetBufferPointer(), m_pixelBlob->GetBufferSize(), NULL, &m_pixelShader)))
	{
		MessageBoxA(NULL, "Failed to create pixel shader.", NULL, NULL);
		return false;
	}

	if (FAILED(D3DCompile(m_vertexShaderSource, strlen(m_vertexShaderSource), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &m_vertexBlob, &ErrorMsg)))
	{
		const char* const ERROR_BASE = "Failed to compile vertex shader.";
		char* const error = new char[strlen(ERROR_BASE) + ErrorMsg->GetBufferSize() + strlen(m_vertexShaderSource) + 1 + 2];
		sprintf(error, "%s\n%s\n%s", ERROR_BASE, (const char*)ErrorMsg->GetBufferPointer(), m_vertexShaderSource);
		MessageBoxA(NULL, error, NULL, NULL);
		delete[]error;
		return false;
	}

	if (FAILED(renderer->d3d11_device->CreateVertexShader(m_vertexBlob->GetBufferPointer(), m_vertexBlob->GetBufferSize(), NULL, &m_vertexShader)))
	{
		MessageBoxA(NULL, "Failed to create vertex shader.", NULL, NULL);
		return false;
	}

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.f;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0.f;
	desc.MaxLOD = 0.f;

	renderer->d3d11_device->CreateSamplerState(&desc, &m_sampler);

	return true;
}

bool renderer::utilities::cef::c_cef_texture::create_sampler()
{
	static auto renderer = c_renderer::instance();

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.f;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0.f;
	desc.MaxLOD = 0.f;

	if (FAILED(renderer->d3d11_device->CreateSamplerState(&desc, &m_sampler)))
	{
		MessageBoxA(NULL, "Failed to create sampler state.", NULL, NULL);
		return false;
	}
	return true;
}

bool renderer::utilities::cef::c_cef_texture::create_webview_geometry_buffer()
{
	static auto renderer = c_renderer::instance();

	// 1/4 of the screen, top right to bottom left
	/*Vertex2d rect[6] = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },

	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 0.0f }
	};*/

	// fullscreen
	vertex_2d_t rect[6] = {
		{ -1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f, 1.0f },

		{ -1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f }
	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(vertex_2d_t) * 6;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(data));
	data.pSysMem = rect;

	if (FAILED(renderer->d3d11_device->CreateBuffer(&bufferDesc, &data, &m_webViewGeometryBuffer)))
	{
		MessageBoxA(NULL, "Failed to create buffer", NULL, NULL);
		return false;
	}

	if (!m_vertexBlob) return false;

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(vertex_2d_t, x), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(vertex_2d_t, u), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(renderer->d3d11_device->CreateInputLayout(layout, 2, m_vertexBlob->GetBufferPointer(), m_vertexBlob->GetBufferSize(), &m_inputLayout)))
	{
		MessageBoxA(NULL, "Failed to create input layout", NULL, NULL);
		return false;
	}

	m_vertexBlob->Release();
	m_vertexBlob = nullptr;

	m_pixelBlob->Release();
	m_pixelBlob = nullptr;
	return true;
}

bool renderer::utilities::cef::c_cef_texture::create_texture(unsigned Width, unsigned Height)
{
	static auto renderer = c_renderer::instance();

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.SampleDesc.Count = 1;
	desc.MiscFlags = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = renderer->d3d11_device->CreateTexture2D(&desc, NULL, &m_texture);
	if (hr != S_OK)
	{
		char error[256] = { 0 };
		sprintf(error, "Failed to create texture (%X)", hr);
		MessageBoxA(NULL, error, NULL, NULL);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = renderer->d3d11_device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureShaderResourceView);
	if (hr != S_OK)
	{
		char error[256] = { 0 };
		sprintf(error, "Failed to create shader resource view for texture (%X)", hr);
		MessageBoxA(NULL, error, NULL, NULL);
		return false;
	}

	m_textureWidth = Width;
	m_textureHeight = Height;
	return true;
}

void renderer::utilities::cef::c_cef_texture::update_render_texture()
{
	static auto renderer = c_renderer::instance();
	renderer->d3d11_device_context->OMSetRenderTargets(1, &m_mainRenderTargetView, NULL);

	std::unique_ptr<draw_data_t> data;
	{
		std::lock_guard<std::mutex> lock(paintMutex);
		if (m_drawData.empty())
			return;

		data = std::move(m_drawData.front());
		m_drawData.pop_front();
	}

	// In case texture size changed recreate it (also handles creation)
	//LOG_DEBUG("%i : %i - %i : %i", TextureWidth, data->width, TextureHeight, data->height);
	if (m_textureWidth != data->m_width || m_textureHeight != data->m_height)
	{
		destroy_texture();
		create_texture(data->m_width, data->m_height);
	}

	if (!m_texture)
		return;

	D3D11_MAPPED_SUBRESOURCE subRes;
	if (FAILED(renderer->d3d11_device_context->Map(m_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes)))
	{
		c_log::Info(c_log::LGreen, "(c_cef_texture::initialize):", "Device Context map failed.");
		return;
	}

	unsigned* pixels = (unsigned*)subRes.pData;
	const unsigned* cefPixels = (const unsigned*)data->m_pixels;

	memcpy(pixels, cefPixels, 4 * m_textureWidth * m_textureHeight);

	renderer->d3d11_device_context->Unmap(m_texture, 0);
	return;
}

void renderer::utilities::cef::c_cef_texture::destroy_texture()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}
}

void renderer::utilities::cef::c_cef_texture::create_render_target()
{
	static auto renderer = c_renderer::instance();

	DXGI_SWAP_CHAIN_DESC sd;
	renderer->d3d11_swapchain->GetDesc(&sd);

	/*ID3D11Texture2D *pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);*/

	// Create the render target
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = sd.BufferDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderer->d3d11_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	renderer->d3d11_device->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &m_mainRenderTargetView);

	pBackBuffer->Release();
}

void renderer::utilities::cef::c_cef_texture::cleanup_render_target()
{
	if (m_mainRenderTargetView)
	{
		m_mainRenderTargetView->Release();
		m_mainRenderTargetView = NULL;
	}
}

void renderer::utilities::cef::c_cef_texture::draw_webview()
{
	static auto renderer = c_renderer::instance();

	if (!m_webViewGeometryBuffer)
	{
		c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):", "No Geometry Buffer...");
		return;
	}

	// Backup Old
	struct BACKUP_DX11_STATE
	{
		UINT                        ScissorRectsCount, ViewportsCount;
		D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		ID3D11RasterizerState* RS;
		ID3D11BlendState* BlendState;
		FLOAT                       BlendFactor[4];
		UINT                        SampleMask;
		UINT                        StencilRef;
		ID3D11DepthStencilState* DepthStencilState;
		ID3D11ShaderResourceView* PSShaderResource;
		ID3D11SamplerState* PSSampler;
		ID3D11PixelShader* PS;
		ID3D11VertexShader* VS;
		UINT                        PSInstancesCount, VSInstancesCount;
		ID3D11ClassInstance* PSInstances[256], * VSInstances[256];   // 256 is max according to PSSetShader documentation 
		D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
		ID3D11Buffer* IndexBuffer, * VertexBuffer, * VSConstantBuffer;
		UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
		DXGI_FORMAT                 IndexBufferFormat;
		ID3D11InputLayout* InputLayout;
	};
	BACKUP_DX11_STATE old;

	old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	renderer->d3d11_device_context->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
	renderer->d3d11_device_context->RSGetViewports(&old.ViewportsCount, old.Viewports);
	renderer->d3d11_device_context->RSGetState(&old.RS);
	renderer->d3d11_device_context->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
	renderer->d3d11_device_context->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
	renderer->d3d11_device_context->PSGetShaderResources(0, 1, &old.PSShaderResource);
	renderer->d3d11_device_context->PSGetSamplers(0, 1, &old.PSSampler);
	old.PSInstancesCount = old.VSInstancesCount = 256;
	renderer->d3d11_device_context->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
	renderer->d3d11_device_context->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
	renderer->d3d11_device_context->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
	renderer->d3d11_device_context->IAGetPrimitiveTopology(&old.PrimitiveTopology);
	renderer->d3d11_device_context->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
	renderer->d3d11_device_context->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
	renderer->d3d11_device_context->IAGetInputLayout(&old.InputLayout);

	RECT rect;
	GetClientRect(renderer->window, &rect);

	// Start CEF Set state
	const D3D11_VIEWPORT viewport = {
		0.0f, 0.0f,
		rect.left - rect.right, rect.top - rect.bottom,
		0.0f, 1.0f
	};

	renderer->d3d11_device_context->RSSetViewports(1, &viewport);
	
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (FAILED(renderer->d3d11_device->CreateBlendState(&desc, &m_blendState)))
			c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):", "Failed to create Blend State.");
	}


	UINT Stride = sizeof(vertex_2d_t);
	UINT Offset = 0;

	renderer->d3d11_device_context->IASetInputLayout(m_inputLayout);
	renderer->d3d11_device_context->IASetVertexBuffers(0, 1, &m_webViewGeometryBuffer, &Stride, &Offset);
	renderer->d3d11_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	renderer->d3d11_device_context->VSSetShader(m_vertexShader, NULL, 0);

	renderer->d3d11_device_context->PSSetShaderResources(0, 1, &m_textureShaderResourceView);
	renderer->d3d11_device_context->PSSetSamplers(0, 1, &m_sampler);
	renderer->d3d11_device_context->PSSetShader(m_pixelShader, NULL, 0);

	const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
	renderer->d3d11_device_context->OMSetBlendState(m_blendState, blend_factor, 0xffffffff);
	{
		D3D11_RASTERIZER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.FillMode = /*D3D11_FILL_WIREFRAME*/D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.ScissorEnable = true;
		desc.DepthClipEnable = true;
		if (FAILED(renderer->d3d11_device->CreateRasterizerState(&desc, &m_rasterizerState)))
			c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):", "Failed to create Rasterizer State.");

	}

	renderer->d3d11_device_context->RSSetState(m_rasterizerState);
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace = desc.FrontFace;
		if (FAILED(renderer->d3d11_device->CreateDepthStencilState(&desc, &m_depthStencilState)))
			c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):", "Failed to create Depth Stencil State.");
	}

	renderer->d3d11_device_context->OMSetDepthStencilState(m_depthStencilState, 0);
	renderer->d3d11_device_context->Draw(6, 0);

	renderer->d3d11_device_context->RSSetViewports(old.ViewportsCount, old.Viewports);

	// Restore modified DX state 
	renderer->d3d11_device_context->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
	renderer->d3d11_device_context->RSSetState(old.RS); if (old.RS) old.RS->Release();
	renderer->d3d11_device_context->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
	renderer->d3d11_device_context->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
	renderer->d3d11_device_context->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
	renderer->d3d11_device_context->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
	renderer->d3d11_device_context->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
	for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
	renderer->d3d11_device_context->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
	renderer->d3d11_device_context->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
	for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
	renderer->d3d11_device_context->IASetPrimitiveTopology(old.PrimitiveTopology);
	renderer->d3d11_device_context->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
	renderer->d3d11_device_context->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
	renderer->d3d11_device_context->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
}

bool renderer::utilities::cef::c_cef_texture::initialize()
{
	if (!create_shaders())
	{
		c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):",
			c_log::LWhite, "Failed to create shaders.");
		return false;
	}

	if (!create_sampler())
	{
		c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):",
			c_log::LWhite, "Failed to create sampler.");
		return false;
	}

	if (!create_webview_geometry_buffer())
	{
		c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):",
			c_log::LWhite, "Failed to create webview geometry buffer.");
		return false;
	}

	c_log::Info(c_log::LGreen, "(c_cef_texture::initialize):",
		c_log::LWhite, "Initialized cef texture renderer.");

	return true;
}

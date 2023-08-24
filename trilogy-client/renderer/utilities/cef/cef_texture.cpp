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
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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
	desc.SampleDesc.Quality = 0;
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

	hr = renderer->d3d11_device->CreateShaderResourceView(m_texture, NULL, &m_textureShaderResourceView);
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

	if (data->m_is_visible)
		m_current_view_visible = data->m_is_visible;

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

	/**
	  * We need to do this because our texture is flipped because of the shaders,
	  * maybe switch shaders? lg
	  */
	for (int y = 0; y < m_textureHeight; ++y)
	{
		for (int x = 0; x < m_textureWidth; ++x)
		{
			int srcIndex = y * m_textureWidth + x;
			int destIndex = (m_textureHeight - 1 - y) * m_textureWidth + x;

			pixels[destIndex] = cefPixels[srcIndex];
		}
	}

	renderer->d3d11_device_context->Unmap(m_texture, 0);
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

	ID3D11Texture2D* texture_backbuffer = NULL;

	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = sd.BufferDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	renderer->d3d11_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture_backbuffer);

	if (texture_backbuffer) {
		renderer->d3d11_device->CreateRenderTargetView(texture_backbuffer, &render_target_view_desc, &m_mainRenderTargetView);
		texture_backbuffer->Release();
	}
}

void renderer::utilities::cef::c_cef_texture::cleanup_render_target()
{
	if (m_mainRenderTargetView)
	{
		m_mainRenderTargetView->Release();
		m_mainRenderTargetView = NULL;
	}
}

void renderer::utilities::cef::c_cef_texture::resize() {
	static auto renderer = c_renderer::instance();

	RECT rect;
	GetClientRect(renderer->window, &rect);
	c_log::Info("Left", rect.left, "right", rect.right, "top", rect.top, "bottom", rect.bottom);

	destroy_texture();
	create_texture(rect.right, rect.bottom);
}

const BYTE quadPS[] =
{
	 68,  88,  66,  67, 143,   7,
	231, 175,  20,  94, 243, 235,
	204,  15,  80,  20, 247, 182,
	206,  11,   1,   0,   0,   0,
	 92,   1,   0,   0,   3,   0,
	  0,   0,  44,   0,   0,   0,
	132,   0,   0,   0, 184,   0,
	  0,   0,  73,  83,  71,  78,
	 80,   0,   0,   0,   2,   0,
	  0,   0,   8,   0,   0,   0,
	 56,   0,   0,   0,   0,   0,
	  0,   0,   1,   0,   0,   0,
	  3,   0,   0,   0,   0,   0,
	  0,   0,  15,   0,   0,   0,
	 68,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,
	  3,   0,   0,   0,   1,   0,
	  0,   0,   3,   3,   0,   0,
	 83,  86,  95,  80, 111, 115,
	105, 116, 105, 111, 110,   0,
	 84,  69,  88,  67,  79,  79,
	 82,  68,   0, 171, 171, 171,
	 79,  83,  71,  78,  44,   0,
	  0,   0,   1,   0,   0,   0,
	  8,   0,   0,   0,  32,   0,
	  0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   3,   0,
	  0,   0,   0,   0,   0,   0,
	 15,   0,   0,   0,  83,  86,
	 95,  84,  65,  82,  71,  69,
	 84,   0, 171, 171,  83,  72,
	 68,  82, 156,   0,   0,   0,
	 64,   0,   0,   0,  39,   0,
	  0,   0,  90,   0,   0,   3,
	  0,  96,  16,   0,   0,   0,
	  0,   0,  88,  24,   0,   4,
	  0, 112,  16,   0,   0,   0,
	  0,   0,  85,  85,   0,   0,
	 98,  16,   0,   3,  50,  16,
	 16,   0,   1,   0,   0,   0,
	101,   0,   0,   3, 242,  32,
	 16,   0,   0,   0,   0,   0,
	104,   0,   0,   2,   1,   0,
	  0,   0,  69,   0,   0,   9,
	242,   0,  16,   0,   0,   0,
	  0,   0,  70,  16,  16,   0,
	  1,   0,   0,   0,  70, 126,
	 16,   0,   0,   0,   0,   0,
	  0,  96,  16,   0,   0,   0,
	  0,   0,  14,   0,   0,   7,
	114,  32,  16,   0,   0,   0,
	  0,   0,  70,   2,  16,   0,
	  0,   0,   0,   0, 246,  15,
	 16,   0,   0,   0,   0,   0,
	 54,   0,   0,   5, 130,  32,
	 16,   0,   0,   0,   0,   0,
	 58,   0,  16,   0,   0,   0,
	  0,   0,  62,   0,   0,   1
};

const BYTE quadVS[] =
{
	68,  88,  66,  67, 203, 141,
	78, 146,   5, 246, 239, 246,
	166,  36, 242, 232,  80,   1,
	231, 115,   1,   0,   0,   0,
	208,   2,   0,   0,   5,   0,
	0,   0,  52,   0,   0,   0,
	128,   0,   0,   0, 180,   0,
	0,   0,  12,   1,   0,   0,
	84,   2,   0,   0,  82,  68,
	69,  70,  68,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	28,   0,   0,   0,   0,   4,
	254, 255,   0,   1,   0,   0,
	28,   0,   0,   0,  77, 105,
	99, 114, 111, 115, 111, 102,
	116,  32,  40,  82,  41,  32,
	72,  76,  83,  76,  32,  83,
	104,  97, 100, 101, 114,  32,
	67, 111, 109, 112, 105, 108,
	101, 114,  32,  49,  48,  46,
	49,   0,  73,  83,  71,  78,
	44,   0,   0,   0,   1,   0,
	0,   0,   8,   0,   0,   0,
	32,   0,   0,   0,   0,   0,
	0,   0,   6,   0,   0,   0,
	1,   0,   0,   0,   0,   0,
	0,   0,   1,   1,   0,   0,
	83,  86,  95,  86,  69,  82,
	84,  69,  88,  73,  68,   0,
	79,  83,  71,  78,  80,   0,
	0,   0,   2,   0,   0,   0,
	8,   0,   0,   0,  56,   0,
	0,   0,   0,   0,   0,   0,
	1,   0,   0,   0,   3,   0,
	0,   0,   0,   0,   0,   0,
	15,   0,   0,   0,  68,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   3,   0,
	0,   0,   1,   0,   0,   0,
	3,  12,   0,   0,  83,  86,
	95,  80,  79,  83,  73,  84,
	73,  79,  78,   0,  84,  69,
	88,  67,  79,  79,  82,  68,
	0, 171, 171, 171,  83,  72,
	68,  82,  64,   1,   0,   0,
	64,   0,   1,   0,  80,   0,
	0,   0,  96,   0,   0,   4,
	18,  16,  16,   0,   0,   0,
	0,   0,   6,   0,   0,   0,
	103,   0,   0,   4, 242,  32,
	16,   0,   0,   0,   0,   0,
	1,   0,   0,   0, 101,   0,
	0,   3,  50,  32,  16,   0,
	1,   0,   0,   0, 104,   0,
	0,   2,   2,   0,   0,   0,
	54,   0,   0,   8, 194,  32,
	16,   0,   0,   0,   0,   0,
	2,  64,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	128,  63,   1,   0,   0,   7,
	18,   0,  16,   0,   0,   0,
	0,   0,  10,  16,  16,   0,
	0,   0,   0,   0,   1,  64,
	0,   0,   1,   0,   0,   0,
	85,   0,   0,   7, 130,   0,
	16,   0,   0,   0,   0,   0,
	10,  16,  16,   0,   0,   0,
	0,   0,   1,  64,   0,   0,
	1,   0,   0,   0,  86,   0,
	0,   5,  50,   0,  16,   0,
	0,   0,   0,   0, 198,   0,
	16,   0,   0,   0,   0,   0,
	0,   0,   0,  10,  50,   0,
	16,   0,   1,   0,   0,   0,
	70,   0,  16,   0,   0,   0,
	0,   0,   2,  64,   0,   0,
	0,   0,   0, 191,   0,   0,
	0, 191,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   8,  66,   0,  16,   0,
	0,   0,   0,   0,  26,   0,
	16, 128,  65,   0,   0,   0,
	0,   0,   0,   0,   1,  64,
	0,   0,   0,   0, 128,  63,
	54,   0,   0,   5,  50,  32,
	16,   0,   1,   0,   0,   0,
	134,   0,  16,   0,   0,   0,
	0,   0,   0,   0,   0,   7,
	18,  32,  16,   0,   0,   0,
	0,   0,  10,   0,  16,   0,
	1,   0,   0,   0,  10,   0,
	16,   0,   1,   0,   0,   0,
	56,   0,   0,   7,  34,  32,
	16,   0,   0,   0,   0,   0,
	26,   0,  16,   0,   1,   0,
	0,   0,   1,  64,   0,   0,
	0,   0,   0, 192,  62,   0,
	0,   1,  83,  84,  65,  84,
	116,   0,   0,   0,  10,   0,
	0,   0,   2,   0,   0,   0,
	0,   0,   0,   0,   3,   0,
	0,   0,   4,   0,   0,   0,
	0,   0,   0,   0,   2,   0,
	0,   0,   1,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   2,   0,   0,   0,
	0,   0,   0,   0,   1,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0
};

void renderer::utilities::cef::c_cef_texture::draw_webview()
{
	static auto renderer = c_renderer::instance();

	if (!m_webViewGeometryBuffer)
	{
		c_log::Error(c_log::LGreen, "(c_cef_texture::initialize):", "No Geometry Buffer...");
		return;
	}

	if (m_current_view_visible != nullptr)
		if (!(*m_current_view_visible))
			return;

	static std::once_flag of;
	std::call_once(of, [&]
		{
			D3D11_SAMPLER_DESC sd = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
	renderer->d3d11_device->CreateSamplerState(&sd, &ss);

	D3D11_BLEND_DESC bd = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	renderer->d3d11_device->CreateBlendState(&bd, &bs);

	renderer->d3d11_device->CreateVertexShader(quadVS, sizeof(quadVS), nullptr, &vs);
	renderer->d3d11_device->CreatePixelShader(quadPS, sizeof(quadPS), nullptr, &ps);
		});

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

	// Backup Old

	UINT Stride = sizeof(vertex_2d_t);
	UINT Offset = 0;

	RECT rect;
	GetClientRect(renderer->window, &rect);

	const D3D11_VIEWPORT viewport = {
		0.0f, 0.0f,
		rect.left - rect.right, rect.top - rect.bottom,
		0.0f, 1.0f
	};

	renderer->d3d11_device_context->RSSetViewports(1, &viewport);

	renderer->d3d11_device_context->IASetInputLayout(m_inputLayout);
	renderer->d3d11_device_context->IASetVertexBuffers(0, 1, &m_webViewGeometryBuffer, &Stride, &Offset);
	renderer->d3d11_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	renderer->d3d11_device_context->VSSetShader(vs, nullptr, 0);

	renderer->d3d11_device_context->PSSetShaderResources(0, 1, &m_textureShaderResourceView);
	renderer->d3d11_device_context->PSSetSamplers(0, 1, &ss);
	renderer->d3d11_device_context->PSSetShader(ps, nullptr, 0);

	renderer->d3d11_device_context->Draw(4, 0);

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

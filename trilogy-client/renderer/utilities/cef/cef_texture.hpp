#pragma once

#ifndef TRILOGY_CEF_TEXTURE_H
#define TRILOGY_CEF_TEXTURE_H

#include <common.hpp>

#include <d3d11.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <include/cef_render_handler.h>
#include <list>

namespace renderer::utilities::cef {
	struct draw_data_t
	{
		unsigned* m_pixels;
		unsigned m_width;
		unsigned m_height;
		CefRenderHandler::RectList m_dirtyRects;

		draw_data_t(unsigned theWidth, unsigned theHeight, const unsigned* thePixels, const CefRenderHandler::RectList& theDirtyRects)
		{
			m_pixels = new unsigned[theWidth * theHeight];
			m_width = theWidth;
			m_height = theHeight;

			memcpy(m_pixels, thePixels, 4 * m_width * m_height);

			m_dirtyRects = theDirtyRects;
		}

		~draw_data_t()
		{
			delete[] m_pixels;
			m_pixels = nullptr;
		}
	};

	struct vertex_2d_t {
		float x, y;
		float u, v;
	};

	class c_cef_texture : public c_singleton<c_cef_texture>
	{
	private:
		static const char m_pixelShaderSource[];
		static const char m_vertexShaderSource[];

		ID3D11RenderTargetView* m_mainRenderTargetView;

		ID3DBlob* m_pixelBlob;
		ID3DBlob* m_vertexBlob;

		ID3D11Buffer* m_webViewGeometryBuffer;
		ID3D11Texture2D* m_texture;
		ID3D11ShaderResourceView* m_textureShaderResourceView;
		ID3D11InputLayout* m_inputLayout;
		ID3D11PixelShader* m_pixelShader;
		ID3D11SamplerState* m_sampler;
		ID3D11BlendState* m_blendState;
		ID3D11RasterizerState* m_rasterizerState;
		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11VertexShader* m_vertexShader;

		unsigned m_textureWidth = 1;
		unsigned m_textureHeight = 1;

	public:
		bool create_shaders();
		bool create_sampler();

		bool create_webview_geometry_buffer();

		bool create_texture(unsigned Width, unsigned Height);
		void update_render_texture();
		void destroy_texture();

		void create_render_target();
		void cleanup_render_target();

		void draw_webview();

		bool initialize();

		std::mutex paintMutex;

		std::list<
			std::unique_ptr<draw_data_t>
		> m_drawData;
	};
}
#endif
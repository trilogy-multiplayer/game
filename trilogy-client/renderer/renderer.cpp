#include "renderer.hpp"

LRESULT __stdcall h_renderer_wndproc(const HWND handle, UINT message, WPARAM word_param, LPARAM long_param) {
	static c_renderer* renderer = c_renderer::instance();

	if (ImGui_ImplWin32_WndProcHandler(handle, message, word_param, long_param))
		return true;

	return CallWindowProc(renderer->o_wnd_proc, handle, message, word_param, long_param);
}

/**
  * Need to split it later into custom imgui classes
  * - or get rid of imgui later & switch to cef/rmlui/ultralight lol
  */
void imgui_inline_text(std::string key, std::string value) {
	ImGui::Text(key.c_str());
	ImGui::SameLine();
	ImGui::Text(value.c_str());
}

HRESULT __stdcall h_renderer_present(IDXGISwapChain* dxgi_swapchain, UINT sync_interval, UINT flags)
{
	static c_renderer* renderer = c_renderer::instance();

	std::call_once(renderer->is_initialized, [&] {
		if (SUCCEEDED(dxgi_swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&renderer->d3d11_device)))
		{
			renderer->d3d11_device->GetImmediateContext(&renderer->d3d11_device_context);

			DXGI_SWAP_CHAIN_DESC swapchain_desc;
			dxgi_swapchain->GetDesc(&swapchain_desc);

			renderer->window = swapchain_desc.OutputWindow;

			ID3D11Texture2D* d3d11_back_buffer;
			dxgi_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&d3d11_back_buffer);

			if (d3d11_back_buffer) {
				renderer->d3d11_device->CreateRenderTargetView(d3d11_back_buffer, NULL, &renderer->d3d11_render_target);
				d3d11_back_buffer->Release();
			}

			renderer->o_wnd_proc = (WNDPROC)SetWindowLongPtr(renderer->window, GWLP_WNDPROC, (LONG_PTR)h_renderer_wndproc);
			renderer->initialize_imgui();
		}
		else c_log::Info(c_log::LGreen, "(c_renderer::h_renderer_present):", c_log::LWhite, "Failed to create renderer in present hook.");
		});

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("(trilogy-mp): debug");
	{
		static c_networking* networking = c_networking::instance();

		ImGui::Text("Nickname:");
		ImGui::InputText("##trilogy-debug::server_nickname", &networking->m_client_name);


		if (networking->m_is_connected) {
			std::stringstream server_ip_with_port;
			server_ip_with_port << networking->m_address.host;
			server_ip_with_port << ":";
			server_ip_with_port << networking->m_address.port;

			imgui_inline_text("(networking): Connected to:", server_ip_with_port.str());
			imgui_inline_text("(networking): Connected as:", networking->m_client_name);
			imgui_inline_text("(librg): Last update:", std::to_string(networking->m_ctx.last_update));
			imgui_inline_text("(librg): Buffer size:", std::to_string(networking->m_ctx.buffer_size));
			imgui_inline_text("(networking): Free player id", std::to_string(networking->player_id));
			imgui_inline_text("(networking): Player count", std::to_string(networking->m_players.size()));
		}
	}
	ImGui::End();

	ImGui::Render();

	renderer->d3d11_device_context->OMSetRenderTargets(1, &renderer->d3d11_render_target, NULL);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return renderer->o_present(dxgi_swapchain, sync_interval, flags);
}

HRESULT h_renderer_resize(IDXGISwapChain* dxgi_swapchain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags)
{
	static c_renderer* renderer = c_renderer::instance();

	if (renderer->d3d11_render_target) {
		renderer->d3d11_device_context->OMSetRenderTargets(0, 0, 0);
		renderer->d3d11_render_target->Release();
	}

	auto result = renderer->o_resize_buffers(dxgi_swapchain, buffer_count, width, height, new_format, swapchain_flags);

	ID3D11Texture2D* d3d11_surface;
	dxgi_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3d11_surface);

	if (d3d11_surface) {
		renderer->d3d11_device->CreateRenderTargetView(d3d11_surface, NULL, &renderer->d3d11_render_target);
		d3d11_surface->Release();

		renderer->d3d11_device_context->OMSetRenderTargets(1, &renderer->d3d11_render_target, NULL);

		D3D11_VIEWPORT d3d11_viewport;
		d3d11_viewport.Width = width;
		d3d11_viewport.Height = height;
		d3d11_viewport.MinDepth = 0.0f;
		d3d11_viewport.MaxDepth = 1.0f;
		d3d11_viewport.TopLeftX = 0;
		d3d11_viewport.TopLeftY = 0;

		renderer->d3d11_device_context->RSSetViewports(1, &d3d11_viewport);
	}

	return result;
}


void c_renderer::initialize_imgui()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(d3d11_device, d3d11_device_context);
}

void c_renderer::initialize()
{
	/**
	  * Kinda ugly, rework?
	  */
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&o_present, h_renderer_present);
			kiero::bind(13, (void**)&o_resize_buffers, h_renderer_resize);

			c_log::Info(c_log::LGreen, "(c_renderer::initialize):", c_log::LWhite, "The renderer got initialized.");
			init_hook = true;
		}
	} while (!init_hook);
}

void c_renderer::shutdown()
{
	// Useless?
	kiero::shutdown();
}

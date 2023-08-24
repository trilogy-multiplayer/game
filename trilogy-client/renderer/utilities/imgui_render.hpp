#pragma once
#include <fstream>
#include <iostream>
#include <ctime>

#include <dwmapi.h> 
#include <TlHelp32.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <utilities/singleton.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <vendor/imgui/imgui.h>
#include <vendor/imgui/imgui_impl_dx11.h>
#include <vendor/imgui/imgui_impl_win32.h>
#include <vendor/imgui/imgui_internal.h>

#include <renderer/fonts/sffont.hpp>


// Color Struct Storing rgba value
struct RGBA {
	RGBA(int r, int g, int b, int a) :
		r(r), g(g), b(b), a(a) { }

	int r;
	int g;
	int b;
	int a;
};

class c_imgui_render : public c_singleton<c_imgui_render> {
public:
	ImGuiIO io;

	HRESULT hres;
	D3D11_VIEWPORT viewport;
	IDXGISwapChain* ptr_window = nullptr;
	IDXGISwapChain* ptr_swapchain = nullptr;
	ID3D11Device* ptr_device = nullptr;
	ID3D11DeviceContext* ptr_context = nullptr;
	ID3D11RenderTargetView* render_target_view = nullptr;
	typedef void (*vResourceLoadCall)(ID3D11Device*);

	vResourceLoadCall load_call = nullptr;

	void set_resource_load(vResourceLoadCall func);

	void initialize(HWND target_window, IDXGISwapChain* swapchain);

	void begin_scene();
	void end_scene();

	void render();

	bool ready();
	void release();
	void reset(UINT Width, UINT Height);


	float render_text(const std::string& text, const ImVec2& position, float size, RGBA color, bool center, bool outine = false);
	void render_line(const ImVec2& from, const ImVec2& to, RGBA color, float thickness = 1.0f);
	void render_circle(const ImVec2& position, float radius, RGBA color, float thickness = 1.0f, uint32_t segments = 16);
	void render_circle_filled(const ImVec2& position, float radius, RGBA color, uint32_t segments = 16);
	void render_rect(const ImVec2& from, const ImVec2& to, RGBA color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All, float thickness = 1.0f);
	void render_dot(const ImVec2& from, const ImVec2& to, RGBA color, float thickness = 1.0f);
	void render_rect_filled(const ImVec2& from, const ImVec2& to, RGBA color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);

private:
	bool m_finished_init = false;
	ImFont* m_font;
	ImFont* m_font_small;
};
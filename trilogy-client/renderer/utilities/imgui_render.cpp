#pragma once

#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>

#include "imgui_render.hpp"

#define SAFE_RELEASE(pObject) { if(pObject) { (pObject)->Release(); (pObject) = NULL; } }

bool c_imgui_render::ready() {
	return ptr_device && ptr_context;
}

void c_imgui_render::set_resource_load(vResourceLoadCall func) {
	load_call = func;
}

void c_imgui_render::initialize(HWND target_window, IDXGISwapChain* swapchain) {
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	ptr_swapchain = swapchain;

	io.IniFilename = NULL;
	ImGui::StyleColorsDark();

	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(8, 8);
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->WindowRounding = 1.0f;
	style->FramePadding = ImVec2(4, 4);
	style->FrameRounding = 1.0f;
	style->ItemSpacing = ImVec2(2, 2);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	style->PopupRounding = 1.0f;

	ImColor mainColor = ImColor(4, 188, 0, 200);

	style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style->Colors[ImGuiCol_TitleBg] = mainColor;
	style->Colors[ImGuiCol_TitleBgActive] = mainColor;
	style->Colors[ImGuiCol_TitleBgCollapsed] = mainColor;
	style->Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25, 220);
	style->Colors[ImGuiCol_FrameBg] = ImColor(21, 21, 21, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(59, 59, 59, 200);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(59, 59, 59, 200);
	style->Colors[ImGuiCol_Button] = ImColor(51, 51, 51, 150);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(64, 62, 64, 180);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(3, 159, 0, 200);
	style->Colors[ImGuiCol_CheckMark] = ImColor(4, 158, 0, 155);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(4, 158, 0, 155);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(4, 168, 0, 155);
	style->Colors[ImGuiCol_Tab] = ImColor(51, 51, 51, 255);
	style->Colors[ImGuiCol_TabHovered] = ImColor(4, 168, 0, 155);
	style->Colors[ImGuiCol_TabActive] = ImColor(4, 158, 0, 155);
	style->Colors[ImGuiCol_TabUnfocused] = ImColor(51, 51, 51, 255);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImColor(4, 158, 0, 155);
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.80f, 0.80f, 0.83f, 0.49f);
	style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(25, 25, 25, 220);
	style->Colors[ImGuiCol_Header] = ImColor(51, 51, 51, 220);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(51, 100, 51, 220);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(51, 61, 51, 220);
	style->Colors[ImGuiCol_ScrollbarBg] = ImColor(30, 30, 30, 220);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(30, 110, 30, 220);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(30, 110, 30, 220);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(30, 110, 30, 220);
	style->Colors[ImGuiCol_PopupBg] = ImColor(25, 25, 25, 220);
	style->Colors[ImGuiCol_Border] = ImColor(255, 255, 255, 0);
	style->Colors[ImGuiCol_BorderShadow] = ImColor(255, 255, 255, 0);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	if (SUCCEEDED(ptr_swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&ptr_device))) {
		ptr_swapchain->GetDevice(__uuidof(ptr_device), (void**)&ptr_device);
		ptr_device->GetImmediateContext(&ptr_context);
	}

	ImGui_ImplWin32_Init(target_window);
	ImGui_ImplDX11_Init(ptr_device, ptr_context);

	m_font = io.Fonts->AddFontFromMemoryCompressedTTF(sfpro_compressed_data, sfpro_compressed_size, 15, nullptr);
}

void c_imgui_render::release() {
	ptr_context->OMSetRenderTargets(0, 0, 0);
	render_target_view->Release();

	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	ptr_device = nullptr;
	ptr_context = nullptr;
	render_target_view = nullptr;
	ptr_swapchain = nullptr;

	m_finished_init = false;
}

void c_imgui_render::reset(UINT Width, UINT Height) {
	ID3D11Texture2D* pBuffer;
	ptr_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(void**)&pBuffer);
	ptr_device->CreateRenderTargetView(pBuffer, NULL,
		&render_target_view);
	// Perform error handling here!
	pBuffer->Release();

	ptr_context->OMSetRenderTargets(1, &render_target_view, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ptr_context->RSSetViewports(1, &vp);
}

void c_imgui_render::begin_scene() {
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	/*if (render_target_view == nullptr) {
		ID3D11Texture2D* backbuffer = NULL;

		hres = ptr_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
		if (FAILED(hres)) {
			ptr_context->OMGetRenderTargets(1, &render_target_view, NULL);
			return;
		}

		hres = ptr_swapchain->GetDevice(IID_PPV_ARGS(&ptr_device));
		if (FAILED(hres)) {
			ptr_context->OMGetRenderTargets(1, &render_target_view, NULL);

			return;
		}

		hres = ptr_device->CreateRenderTargetView(backbuffer, NULL, &render_target_view);
		backbuffer->Release();
		if (FAILED(hres)) {
			return;
		}
	}
	ptr_context->OMSetRenderTargets(1, &render_target_view, NULL);*/

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
}

void c_imgui_render::render() {
	ImGui::Render();
	// ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void c_imgui_render::end_scene() {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(3);

	ImGui::EndFrame();
}

bool in_screen(ImVec2 pos) {
	if (pos.x < 0 || pos.y < 0 || pos.x > ImGui::GetIO().DisplaySize.x || pos.y > ImGui::GetIO().DisplaySize.y) {
		return false;
	}

	return true;
}

float c_imgui_render::render_text(const std::string& text, const ImVec2& position, float size, RGBA color, bool center, bool outine) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;
	while (std::getline(stream, line)) {
		ImVec2 textSize = m_font->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());

		if (center) {
			if (outine) {
				window->DrawList->AddText(m_font, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(m_font, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(m_font, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(m_font, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
			}
			window->DrawList->AddText(m_font, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, IM_COL32(color.r, color.g, color.b, color.a), text.c_str());
		}
		else {
			if (outine) {
				window->DrawList->AddText(m_font, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(m_font, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(m_font, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
				window->DrawList->AddText(m_font, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, IM_COL32(0, 0, 0, color.a), text.c_str());
			}
			window->DrawList->AddText(m_font, size, { position.x, position.y + textSize.y * i }, IM_COL32(color.r, color.g, color.b, color.a), text.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void c_imgui_render::render_dot(const ImVec2& from, const ImVec2& to, RGBA color, float thickness) {
	if (in_screen(from) && in_screen(to))
		c_imgui_render::render_rect(from, to, color, 5.0f, 0, thickness);
}

void c_imgui_render::render_line(const ImVec2& from, const ImVec2& to, RGBA color, float thickness) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (in_screen(from) && in_screen(to))
		window->DrawList->AddLine(from, to, IM_COL32(color.r, color.g, color.b, color.a), thickness);
}

void c_imgui_render::render_circle(const ImVec2& position, float radius, RGBA color, float thickness, uint32_t segments) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();


	if (in_screen(position))
		window->DrawList->AddCircle(position, radius, IM_COL32(color.r, color.g, color.b, color.a), segments, thickness);
}

void c_imgui_render::render_circle_filled(const ImVec2& position, float radius, RGBA color, uint32_t segments) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (in_screen(position))
		window->DrawList->AddCircleFilled(position, radius, IM_COL32(color.r, color.g, color.b, color.a), segments);
}

void c_imgui_render::render_rect(const ImVec2& from, const ImVec2& to, RGBA color, float rounding, uint32_t roundingCornersFlags, float thickness) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (in_screen(from) && in_screen(to))
		window->DrawList->AddRect(from, to, IM_COL32(color.r, color.g, color.b, color.a), rounding, roundingCornersFlags, thickness);
}

void c_imgui_render::render_rect_filled(const ImVec2& from, const ImVec2& to, RGBA color, float rounding, uint32_t roundingCornersFlags) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	if (in_screen(from) && in_screen(to))
		window->DrawList->AddRectFilled(from, to, IM_COL32(color.r, color.g, color.b, color.a), rounding, roundingCornersFlags);
}


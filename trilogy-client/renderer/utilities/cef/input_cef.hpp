#pragma once

#ifndef __CEFINPUT_H__
#define __CEFINPUT_H__

#include <common.hpp>
#include <renderer/renderer.hpp>

class CefInput
{
private:
	static WNDPROC Hook;
	static bool Visible;
	static MSG msg;

public:

	static void InstallHook();
	static void UninstallHook();
	static int Messages();
	static void ForegroundCheck();

	static bool IsVisible() { return true; }
	static WNDPROC GetHook() { return Hook; }
};

LRESULT WINAPI HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
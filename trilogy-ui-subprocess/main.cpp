#include "main.hpp"
#include <Windows.h>
#include <application.hpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdShow, int nCmdShow)
{
	CefEnableHighDPISupport();

	CefMainArgs main_args(hInstance);
	return CefExecuteProcess(main_args, new CEFApp, nullptr);
}
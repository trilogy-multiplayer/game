#include "input_cef.hpp"

#include <include/cef_app.h>
#include <include/cef_client.h>

#include <renderer/utilities/cef/renderer_cef.hpp>

WNDPROC CefInput::Hook = nullptr;
bool CefInput::Visible = true;
MSG CefInput::msg;

void CefInput::InstallHook()
{
	static auto renderer = c_renderer::instance();
	Hook = (WNDPROC)SetWindowLongPtr(renderer->window, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
}

void CefInput::UninstallHook()
{
	//UnhookWindowsHookEx(Hook);
}

int CefInput::Messages()
{

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
	UninstallHook();
	return (int)msg.wParam;
}


void CefInput::ForegroundCheck()
{
	while (msg.message != WM_QUIT)
	{
		HWND forgnd = GetForegroundWindow();

		int bufferLen = GetWindowTextLength(forgnd) + 1;
		std::string wnd_ftitle(bufferLen, '\0');
		GetWindowTextA(forgnd, &wnd_ftitle[0], bufferLen);

		if (std::strcmp((char*)wnd_ftitle.c_str(), "Grand Theft Auto V") == 0)
		{
			if (!Visible)
				Visible = true;
		}
		else
		{
			if (Visible)
				Visible = false;
		}

		Sleep(100); //10th of a second sleep as we don't need to check that frequently
	}
	return;
}

bool isKeyDown(WPARAM wparam)
{
	return (GetKeyState(wparam) & 0x8000) != 0;
}

int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam)
{

	int modifiers = 0;
	if (isKeyDown(VK_SHIFT))
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (isKeyDown(VK_CONTROL))
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (isKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;

	// Low bit set from GetKeyState indicates "toggled".
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;

	switch (wparam)
	{
	case VK_RETURN:
		if ((lparam >> 16) & KF_EXTENDED)
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		if (!((lparam >> 16) & KF_EXTENDED))
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_NUMLOCK:
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
	case VK_DIVIDE:
	case VK_MULTIPLY:
	case VK_SUBTRACT:
	case VK_ADD:
	case VK_DECIMAL:
	case VK_CLEAR:
		modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_SHIFT:
		if (isKeyDown(VK_LSHIFT))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (isKeyDown(VK_RSHIFT))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_CONTROL:
		if (isKeyDown(VK_LCONTROL))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (isKeyDown(VK_RCONTROL))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_MENU:
		if (isKeyDown(VK_LMENU))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (isKeyDown(VK_RMENU))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_LWIN:
		modifiers |= EVENTFLAG_IS_LEFT;
		break;
	case VK_RWIN:
		modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	}
	return modifiers;
}

int GetCefMouseModifiers(WPARAM wparam)
{
	int modifiers = 0;
	if (wparam & MK_CONTROL)
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (wparam & MK_SHIFT)
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (isKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;
	if (wparam & MK_LBUTTON)
		modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
	if (wparam & MK_MBUTTON)
		modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
	if (wparam & MK_RBUTTON)
		modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
	
	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;
	return modifiers;
}

LRESULT WINAPI HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static auto renderer = c_renderer::instance();

	if (CefInput::IsVisible() && renderer->focus_browser)
	{
		static int lastClickX;
		static int lastClickY;
		static int lastClickCount;
		static LONG lastClickTime;
		static CefBrowserHost::MouseButtonType lastClickButton;
		static bool mouseTracking;

		LONG currentTime = 0;
		bool cancelPreviousClick = false;

		if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
			uMsg == WM_MBUTTONDOWN || uMsg == WM_MOUSEMOVE ||
			uMsg == WM_MOUSELEAVE)
		{
			currentTime = GetMessageTime();
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			cancelPreviousClick =
				(abs(lastClickX - x) > (GetSystemMetrics(SM_CXDOUBLECLK) / 2))
				|| (abs(lastClickY - y) > (GetSystemMetrics(SM_CYDOUBLECLK) / 2))
				|| ((currentTime - lastClickTime) > GetDoubleClickTime());
			if (cancelPreviousClick &&
				(uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSELEAVE))
			{
				lastClickCount = 0;
				lastClickX = 0;
				lastClickY = 0;
				lastClickTime = 0;
			}
		}

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			CefBrowserHost::MouseButtonType btnType =
				((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) ? MBT_LEFT : (
					(uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) ? MBT_RIGHT : MBT_MIDDLE));
			if (!cancelPreviousClick && (btnType == lastClickButton))
			{
				++lastClickCount;
			}
			else
			{
				lastClickCount = 1;
				lastClickX = x;
				lastClickY = y;
			}
			lastClickTime = currentTime;
			lastClickButton = btnType;

			CefMouseEvent mouse_event;
			mouse_event.x = x;
			mouse_event.y = y;
			mouse_event.modifiers = GetCefMouseModifiers(wParam);

			for (auto& view : renderer::utilities::cef::c_renderer_cef::instance()->views) {
				auto& browser = view->m_webview;

				if (browser)
				{
					browser->GetHost()->SendMouseClickEvent(mouse_event, btnType, false, lastClickCount);
				}
			}
		} break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			CefBrowserHost::MouseButtonType btnType =
				(uMsg == WM_LBUTTONUP ? MBT_LEFT : (
					uMsg == WM_RBUTTONUP ? MBT_RIGHT : MBT_MIDDLE));

			for (auto& view : renderer::utilities::cef::c_renderer_cef::instance()->views) {
				auto& browser = view->m_webview;

				if (browser)
				{
					CefMouseEvent mouse_event;
					mouse_event.x = x;
					mouse_event.y = y;
					mouse_event.modifiers = GetCefMouseModifiers(wParam);
					browser->GetHost()->SendMouseClickEvent(mouse_event, btnType, true,
						lastClickCount);
				}
			}

			break;
		}
		case WM_MOUSEMOVE: {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (!mouseTracking)
			{
				// Start tracking mouse leave. Required for the WM_MOUSELEAVE event to
				// be generated.
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				mouseTracking = true;
			}

			for (auto& view : renderer::utilities::cef::c_renderer_cef::instance()->views) {
				auto& browser = view->m_webview;

				if (browser)
				{
					CefMouseEvent mouse_event;
					mouse_event.x = x;
					mouse_event.y = y;
					mouse_event.modifiers = GetCefMouseModifiers(wParam);
					browser->GetHost()->SendMouseMoveEvent(mouse_event, false);
				}
			}

			break;
		}

		case WM_MOUSELEAVE: {
			if (mouseTracking)
			{
				// Stop tracking mouse leave.
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE & TME_CANCEL;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				mouseTracking = false;
			}

			for (auto& view : renderer::utilities::cef::c_renderer_cef::instance()->views) {
				auto& browser = view->m_webview;

				if (browser)
				{
					// Determine the cursor position in screen coordinates.
					POINT p;
					::GetCursorPos(&p);
					::ScreenToClient(hWnd, &p);

					CefMouseEvent mouse_event;
					mouse_event.x = p.x;
					mouse_event.y = p.y;
					mouse_event.modifiers = GetCefMouseModifiers(wParam);
					browser->GetHost()->SendMouseMoveEvent(mouse_event, true);
				}
			}
		} break;

		case WM_MOUSEWHEEL: {
			for (auto& view : renderer::utilities::cef::c_renderer_cef::instance()->views) {
				auto& browser = view->m_webview;

				if (browser)
				{
					POINT screen_point = { LOWORD(lParam), HIWORD(lParam) };
					HWND scrolled_wnd = ::WindowFromPoint(screen_point);
					if (scrolled_wnd != hWnd)
						break;

					ScreenToClient(hWnd, &screen_point);
					int delta = GET_WHEEL_DELTA_WPARAM(wParam);

					CefMouseEvent mouse_event;
					mouse_event.x = screen_point.x;
					mouse_event.y = screen_point.y;
					mouse_event.modifiers = GetCefMouseModifiers(wParam);

					browser->GetHost()->SendMouseWheelEvent(mouse_event,
						isKeyDown(VK_SHIFT) ? delta : 0,
						!isKeyDown(VK_SHIFT) ? delta : 0);
				}
			}

			break;
		}
		}

		if (uMsg == WM_KEYUP || uMsg == WM_KEYDOWN || uMsg == WM_CHAR)
		{
			CefKeyEvent keyEvent;

			keyEvent.windows_key_code = wParam;
			keyEvent.native_key_code = lParam;
			keyEvent.modifiers = GetCefKeyboardModifiers(wParam, lParam);

			if (uMsg != WM_CHAR)
			{
				keyEvent.type = (uMsg == WM_KEYDOWN) ? KEYEVENT_RAWKEYDOWN : KEYEVENT_KEYUP;
			}
			else
			{
				keyEvent.type = KEYEVENT_CHAR;
			}

			for (auto& view : renderer::utilities::cef::c_renderer_cef::instance()->views) {
				auto& browser = view->m_webview;

				if (browser)
				{
					browser->GetHost()->SendKeyEvent(keyEvent);
				}
			}

			return CallWindowProc(CefInput::GetHook(), hWnd, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_INPUT && renderer->focus_browser)
		{
			return CallWindowProc(CefInput::GetHook(), hWnd, uMsg, wParam, lParam);
		}
		else if (uMsg == WM_IME_KEYLAST || uMsg == WM_IME_KEYDOWN || uMsg == WM_IME_KEYUP)
		{
			return CallWindowProc(CefInput::GetHook(), hWnd, uMsg, wParam, lParam);
		}

	}
	return CallWindowProc(CefInput::GetHook(), hWnd, uMsg, wParam, lParam);
}
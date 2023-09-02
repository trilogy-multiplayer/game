#include "cef_app.hpp"

#include <string>
#include <include/cef_browser.h>
#include <include/cef_command_line.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/wrapper/cef_helpers.h>
#include <include/wrapper/cef_stream_resource_handler.h>
#include <include/cef_parser.h>

void renderer::utilities::cef::c_cef_app::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	/**
	  * TODO:
	  * Add context functions
	  */
}

CefRefPtr<CefRenderProcessHandler> renderer::utilities::cef::c_cef_app::GetRenderProcessHandler()
{
	return this;
}

bool renderer::utilities::cef::c_cef_app::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	return false;
}

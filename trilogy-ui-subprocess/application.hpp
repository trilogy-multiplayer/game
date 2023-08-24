#pragma once

#define WIN32_LEAN_AND_MEAN
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/wrapper/cef_helpers.h>
#include <include/cef_parser.h>
#include <include/cef_task.h>
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_life_span_handler.h>
#include <include/internal/cef_types_wrappers.h>
#include <include/internal/cef_win.h>
#include <include/internal/cef_types.h>
#include <include/cef_origin_whitelist.h>
#include <include/cef_browser.h>
#include <include/cef_command_line.h>
#include <vector>

#pragma once

class CEFApp : public CefApp, public CefRenderProcessHandler, public CefV8Handler {

	CefRefPtr<CefFrame> m_pFrame;

	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override
	{
		m_pFrame = frame;

		context->GetGlobal()->SetValue("TriggerEvent", CefV8Value::CreateFunction("TriggerEvent", this), V8_PROPERTY_ATTRIBUTE_NONE);
		context->GetGlobal()->SetValue("Invoke", CefV8Value::CreateFunction("Invoke", this), V8_PROPERTY_ATTRIBUTE_NONE);
	};

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override
	{
		return this;
	};

	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override
	{
		if (arguments.size() == 0)
			return true;

		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);

		auto args = message->GetArgumentList();
		args->SetSize(arguments.size());

		for (int i = 0; i < arguments.size(); i++)
		{
			auto arg = arguments[i];

			if (arg->IsBool()) args->SetBool(i, arg->GetBoolValue());
			else if (arg->IsDouble()) args->SetDouble(i, arg->GetDoubleValue());
			else if (arg->IsInt() || arg->IsUInt()) args->SetInt(i, arg->GetIntValue());
			else if (arg->IsNull()) args->SetNull(i);
			else if (arg->IsString()) args->SetString(i, arg->GetStringValue());
			else args->SetString(i, "Unsupported variable type");
		}

		m_pFrame->GetBrowser()->GetMainFrame()->SendProcessMessage(PID_BROWSER, message);
		return true;
	};

	IMPLEMENT_REFCOUNTING(CEFApp);
};
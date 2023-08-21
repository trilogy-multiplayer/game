#include <include/cef_app.h>
#include <include/cef_scheme.h>

namespace renderer::utilities::cef {
	class c_cef_app : public CefApp, public CefRenderProcessHandler, public CefV8Handler {
		virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
		virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

		IMPLEMENT_REFCOUNTING(c_cef_app);
	};
}
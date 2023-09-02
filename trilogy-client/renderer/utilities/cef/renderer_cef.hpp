#pragma once

#ifndef TRILOGY_RENDERER_CEF_H
#define TRILOGY_RENDERER_CEF_H

#include <common.hpp>
#include <core/core.hpp>

#include <include/internal/cef_win.h>
#include <include/cef_app.h>

#include "cef_app.hpp"
#include "cef_view.hpp"

namespace renderer::utilities::cef {
	class CEFSchemeHandlerFactory : public CefSchemeHandlerFactory
	{
	public:
		virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) {
			return nullptr;
		}

		IMPLEMENT_REFCOUNTING(CEFSchemeHandlerFactory);
	};

	class c_renderer_cef : public c_singleton <c_renderer_cef> {
	public:
		void initialize();
		CefRefPtr<renderer::utilities::cef::c_cef_view> create_browser(const std::string& url, bool visible = true);

		std::function<
			void(CefRefPtr<CefFrame> frame, CefRefPtr<CefListValue> args)
		> get_cef_function_handler(std::string name);

		void register_cef_function(std::string name, std::function<void(CefRefPtr<CefFrame> frame, CefRefPtr<CefListValue> args)> function);
	public:
		std::vector<CefRefPtr<renderer::utilities::cef::c_cef_view>> views;
		CefRefPtr<c_cef_app> app;

		std::map<std::string,
			std::function<void(CefRefPtr<CefFrame> frame, CefRefPtr<CefListValue> args)>
		> m_handlers;
	};
}
#endif

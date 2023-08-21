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

		void create_browser(const std::string& url, bool transparent = true);

	public:
		std::vector<CefRefPtr<renderer::utilities::cef::c_cef_view>> views;
		CefRefPtr<c_cef_app> app;
	};
}
#endif

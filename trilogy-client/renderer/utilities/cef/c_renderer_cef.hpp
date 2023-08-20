#pragma once

#ifndef TRILOGY_RENDERER_CEF_H
#define TRILOGY_RENDERER_CEF_H

#include <common.hpp>
#include <core/core.hpp>

#include <include/internal/cef_win.h>
#include <include/cef_app.h>

namespace renderer::utilities::cef {
	class c_renderer_cef : public c_singleton <c_renderer_cef> {
	public:
		void initialize();

		void create_browser(const std::string& url, bool transparent = true);
	};
}
#endif

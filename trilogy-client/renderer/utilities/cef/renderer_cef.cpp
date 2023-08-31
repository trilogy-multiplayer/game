#include "renderer_cef.hpp"
#include <renderer/renderer.hpp>
#include <renderer/utilities/cef/cef_texture.hpp>

void renderer::utilities::cef::c_renderer_cef::initialize()
{
	static auto core = c_core::instance();
	app = new c_cef_app;

	auto chrome_elf_handle = LoadLibraryA(core->make_trilogy_path("libs/chrome_elf.dll").c_str());
	if (!chrome_elf_handle) {
		c_log::Error(c_log::LGreen, "(renderer::utilities::cef::c_renderer_cef::initialize):",
			c_log::LWhite, "Failed to find chrome_elf.dll in libraries");
		return;
	}

	auto lib_egl_handle = LoadLibraryA(core->make_trilogy_path("libs/libEGL.dll").c_str());
	if (!lib_egl_handle) {
		c_log::Error(c_log::LGreen, "(renderer::utilities::cef::c_renderer_cef::initialize):",
			c_log::LWhite, "Failed to find libEGL.dll in libraries");
		return;
	}

	auto lib_gles_v2_handle = LoadLibraryA(core->make_trilogy_path("libs/libGLESv2.dll").c_str());
	if (!lib_gles_v2_handle) {
		c_log::Error(c_log::LGreen, "(renderer::utilities::cef::c_renderer_cef::initialize):",
			c_log::LWhite, "Failed to find libGLESv2.dll in libraries");
		return;
	}

	auto libcef_handle = LoadLibraryA(core->make_trilogy_path("libs/libcef.dll").c_str());
	if (!libcef_handle) {
		c_log::Error(c_log::LGreen, "(renderer::utilities::cef::c_renderer_cef::initialize):",
			c_log::LWhite, "Failed to find libcef.dll in libraries");
		return;
	}

	CefMainArgs cef_main_args;
	CefSettings settings;

	CefString(&settings.browser_subprocess_path).FromASCII(
		c_log::Join(core->get_trilogy_base_path(), "libs/trilogy-ui-subprocess.exe").c_str());

	CefString(&settings.resources_dir_path).FromASCII(
		c_core::instance()->make_trilogy_path("libs/").c_str());

	CefString(&settings.locales_dir_path).FromASCII(c_core::instance()->make_trilogy_path("libs/locales").c_str());

	CefString(&settings.log_file).FromASCII(
		c_log::Join(core->get_trilogy_base_path(), "cefdebug.txt").c_str());

	settings.log_severity = cef_log_severity_t::LOGSEVERITY_VERBOSE;

	settings.multi_threaded_message_loop = true;
	settings.windowless_rendering_enabled = true;

	CefEnableHighDPISupport();

	bool state = CefInitialize(cef_main_args, settings, app, nullptr);
	CefRegisterSchemeHandlerFactory("http", "trilogy", new CEFSchemeHandlerFactory());

	this->register_cef_function("trilogy.ShowCursor", [](CefRefPtr<CefFrame> frame, CefRefPtr<CefListValue> args) {
		c_log::Info("Set cursor to", args->GetBool(1));
		c_renderer::instance()->focus_browser = args->GetBool(1);
		});

	this->register_cef_function("trilogy.Log", [](CefRefPtr<CefFrame> frame, CefRefPtr<CefListValue> args) {
		c_log::Info("trilogy.Log", args->GetString(1));
		});
}

CefRefPtr<renderer::utilities::cef::c_cef_view> renderer::utilities::cef::c_renderer_cef::create_browser(const std::string& url, bool visible)
{
	CefRefPtr<renderer::utilities::cef::c_cef_view> cef_view = new renderer::utilities::cef::c_cef_view(url, false, visible);
	cef_view->Initialise();

	views.push_back(cef_view);

	return cef_view;
}

std::function<void(CefRefPtr<CefFrame>frame, CefRefPtr<CefListValue>args)> renderer::utilities::cef::c_renderer_cef::get_cef_function_handler(std::string name)
{
	return m_handlers[name];
}

void renderer::utilities::cef::c_renderer_cef::register_cef_function(std::string name, std::function<void(CefRefPtr<CefFrame>frame, CefRefPtr<CefListValue>args)> function)
{
	m_handlers[name] = function;
}

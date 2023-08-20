#include "c_renderer_cef.hpp"
#include <renderer/renderer.hpp>

void renderer::utilities::cef::c_renderer_cef::initialize()
{
    static auto core = c_core::instance();

	CefMainArgs cef_main_args;
    CefSettings settings;
    
    CefString(&settings.browser_subprocess_path).FromASCII(
        c_log::Join(core->get_trilogy_base_path(), "trilogy-ui-subprocess.exe").c_str());

    CefString(&settings.resources_dir_path).FromASCII(
        c_log::Join(core->get_trilogy_base_path(), ".").c_str());

    CefString(&settings.cache_path).FromASCII(
        c_log::Join(core->get_trilogy_base_path(), "cache").c_str());

    CefString(&settings.locales_dir_path).FromASCII(
        c_log::Join(core->get_trilogy_base_path(), "locales").c_str());

    CefString(&settings.log_file).FromASCII(
        c_log::Join(core->get_trilogy_base_path(), "cefdebug.txt").c_str());

    settings.multi_threaded_message_loop = true;
    settings.windowless_rendering_enabled = true;

    bool state = CefInitialize(cef_main_args, settings, nullptr, nullptr);
    CefRegisterSchemeHandlerFactory("http", "trilogy", nullptr);

}

void renderer::utilities::cef::c_renderer_cef::create_browser(const std::string& url, bool transparent)
{
}

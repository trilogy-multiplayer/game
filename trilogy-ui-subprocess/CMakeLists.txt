set(PROJECT_NAME trilogy-ui-subprocess)

################################################################################
# Source groups
################################################################################
set(Headerdateien
    "main.hpp"
)
source_group("Headerdateien" FILES ${Headerdateien})

set(Quelldateien
    "cef/include/base/cef_atomic_ref_count.h"
    "cef/include/base/cef_atomicops.h"
    "cef/include/base/cef_basictypes.h"
    "cef/include/base/cef_bind.h"
    "cef/include/base/cef_bind_helpers.h"
    "cef/include/base/cef_build.h"
    "cef/include/base/cef_callback.h"
    "cef/include/base/cef_callback_forward.h"
    "cef/include/base/cef_callback_helpers.h"
    "cef/include/base/cef_callback_list.h"
    "cef/include/base/cef_cancelable_callback.h"
    "cef/include/base/cef_lock.h"
    "cef/include/base/cef_logging.h"
    "cef/include/base/cef_macros.h"
    "cef/include/base/cef_move.h"
    "cef/include/base/cef_platform_thread.h"
    "cef/include/base/cef_ref_counted.h"
    "cef/include/base/cef_scoped_ptr.h"
    "cef/include/base/cef_string16.h"
    "cef/include/base/cef_template_util.h"
    "cef/include/base/cef_thread_checker.h"
    "cef/include/base/cef_thread_collision_warner.h"
    "cef/include/base/cef_trace_event.h"
    "cef/include/base/cef_tuple.h"
    "cef/include/base/cef_weak_ptr.h"
    "cef/include/base/internal/cef_atomicops_x86_msvc.h"
    "cef/include/base/internal/cef_bind_internal.h"
    "cef/include/base/internal/cef_bind_internal_win.h"
    "cef/include/base/internal/cef_callback_internal.h"
    "cef/include/base/internal/cef_lock_impl.h"
    "cef/include/base/internal/cef_raw_scoped_refptr_mismatch_checker.h"
    "cef/include/base/internal/cef_thread_checker_impl.h"
    "cef/include/capi/cef_accessibility_handler_capi.h"
    "cef/include/capi/cef_app_capi.h"
    "cef/include/capi/cef_auth_callback_capi.h"
    "cef/include/capi/cef_base_capi.h"
    "cef/include/capi/cef_browser_capi.h"
    "cef/include/capi/cef_browser_process_handler_capi.h"
    "cef/include/capi/cef_callback_capi.h"
    "cef/include/capi/cef_client_capi.h"
    "cef/include/capi/cef_command_line_capi.h"
    "cef/include/capi/cef_context_menu_handler_capi.h"
    "cef/include/capi/cef_cookie_capi.h"
    "cef/include/capi/cef_crash_util_capi.h"
    "cef/include/capi/cef_dialog_handler_capi.h"
    "cef/include/capi/cef_display_handler_capi.h"
    "cef/include/capi/cef_dom_capi.h"
    "cef/include/capi/cef_download_handler_capi.h"
    "cef/include/capi/cef_download_item_capi.h"
    "cef/include/capi/cef_drag_data_capi.h"
    "cef/include/capi/cef_drag_handler_capi.h"
    "cef/include/capi/cef_extension_capi.h"
    "cef/include/capi/cef_extension_handler_capi.h"
    "cef/include/capi/cef_file_util_capi.h"
    "cef/include/capi/cef_find_handler_capi.h"
    "cef/include/capi/cef_focus_handler_capi.h"
    "cef/include/capi/cef_frame_capi.h"
    "cef/include/capi/cef_image_capi.h"
    "cef/include/capi/cef_jsdialog_handler_capi.h"
    "cef/include/capi/cef_keyboard_handler_capi.h"
    "cef/include/capi/cef_life_span_handler_capi.h"
    "cef/include/capi/cef_load_handler_capi.h"
    "cef/include/capi/cef_menu_model_capi.h"
    "cef/include/capi/cef_menu_model_delegate_capi.h"
    "cef/include/capi/cef_navigation_entry_capi.h"
    "cef/include/capi/cef_origin_whitelist_capi.h"
    "cef/include/capi/cef_parser_capi.h"
    "cef/include/capi/cef_path_util_capi.h"
    "cef/include/capi/cef_print_handler_capi.h"
    "cef/include/capi/cef_print_settings_capi.h"
    "cef/include/capi/cef_process_message_capi.h"
    "cef/include/capi/cef_process_util_capi.h"
    "cef/include/capi/cef_render_handler_capi.h"
    "cef/include/capi/cef_render_process_handler_capi.h"
    "cef/include/capi/cef_request_capi.h"
    "cef/include/capi/cef_request_context_capi.h"
    "cef/include/capi/cef_request_context_handler_capi.h"
    "cef/include/capi/cef_request_handler_capi.h"
    "cef/include/capi/cef_resource_bundle_capi.h"
    "cef/include/capi/cef_resource_bundle_handler_capi.h"
    "cef/include/capi/cef_resource_handler_capi.h"
    "cef/include/capi/cef_response_capi.h"
    "cef/include/capi/cef_response_filter_capi.h"
    "cef/include/capi/cef_scheme_capi.h"
    "cef/include/capi/cef_server_capi.h"
    "cef/include/capi/cef_ssl_info_capi.h"
    "cef/include/capi/cef_ssl_status_capi.h"
    "cef/include/capi/cef_stream_capi.h"
    "cef/include/capi/cef_string_visitor_capi.h"
    "cef/include/capi/cef_task_capi.h"
    "cef/include/capi/cef_thread_capi.h"
    "cef/include/capi/cef_trace_capi.h"
    "cef/include/capi/cef_urlrequest_capi.h"
    "cef/include/capi/cef_v8_capi.h"
    "cef/include/capi/cef_values_capi.h"
    "cef/include/capi/cef_waitable_event_capi.h"
    "cef/include/capi/cef_web_plugin_capi.h"
    "cef/include/capi/cef_x509_certificate_capi.h"
    "cef/include/capi/cef_xml_reader_capi.h"
    "cef/include/capi/cef_zip_reader_capi.h"
    "cef/include/capi/test/cef_test_helpers_capi.h"
    "cef/include/capi/test/cef_translator_test_capi.h"
    "cef/include/capi/views/cef_box_layout_capi.h"
    "cef/include/capi/views/cef_browser_view_capi.h"
    "cef/include/capi/views/cef_browser_view_delegate_capi.h"
    "cef/include/capi/views/cef_button_capi.h"
    "cef/include/capi/views/cef_button_delegate_capi.h"
    "cef/include/capi/views/cef_display_capi.h"
    "cef/include/capi/views/cef_fill_layout_capi.h"
    "cef/include/capi/views/cef_label_button_capi.h"
    "cef/include/capi/views/cef_layout_capi.h"
    "cef/include/capi/views/cef_menu_button_capi.h"
    "cef/include/capi/views/cef_menu_button_delegate_capi.h"
    "cef/include/capi/views/cef_panel_capi.h"
    "cef/include/capi/views/cef_panel_delegate_capi.h"
    "cef/include/capi/views/cef_scroll_view_capi.h"
    "cef/include/capi/views/cef_textfield_capi.h"
    "cef/include/capi/views/cef_textfield_delegate_capi.h"
    "cef/include/capi/views/cef_view_capi.h"
    "cef/include/capi/views/cef_view_delegate_capi.h"
    "cef/include/capi/views/cef_window_capi.h"
    "cef/include/capi/views/cef_window_delegate_capi.h"
    "cef/include/cef_accessibility_handler.h"
    "cef/include/cef_app.h"
    "cef/include/cef_auth_callback.h"
    "cef/include/cef_base.h"
    "cef/include/cef_browser.h"
    "cef/include/cef_browser_process_handler.h"
    "cef/include/cef_callback.h"
    "cef/include/cef_client.h"
    "cef/include/cef_command_line.h"
    "cef/include/cef_context_menu_handler.h"
    "cef/include/cef_cookie.h"
    "cef/include/cef_crash_util.h"
    "cef/include/cef_dialog_handler.h"
    "cef/include/cef_display_handler.h"
    "cef/include/cef_dom.h"
    "cef/include/cef_download_handler.h"
    "cef/include/cef_download_item.h"
    "cef/include/cef_drag_data.h"
    "cef/include/cef_drag_handler.h"
    "cef/include/cef_extension.h"
    "cef/include/cef_extension_handler.h"
    "cef/include/cef_file_util.h"
    "cef/include/cef_find_handler.h"
    "cef/include/cef_focus_handler.h"
    "cef/include/cef_frame.h"
    "cef/include/cef_image.h"
    "cef/include/cef_jsdialog_handler.h"
    "cef/include/cef_keyboard_handler.h"
    "cef/include/cef_life_span_handler.h"
    "cef/include/cef_load_handler.h"
    "cef/include/cef_menu_model.h"
    "cef/include/cef_menu_model_delegate.h"
    "cef/include/cef_navigation_entry.h"
    "cef/include/cef_origin_whitelist.h"
    "cef/include/cef_pack_resources.h"
    "cef/include/cef_pack_strings.h"
    "cef/include/cef_parser.h"
    "cef/include/cef_path_util.h"
    "cef/include/cef_print_handler.h"
    "cef/include/cef_print_settings.h"
    "cef/include/cef_process_message.h"
    "cef/include/cef_process_util.h"
    "cef/include/cef_render_handler.h"
    "cef/include/cef_render_process_handler.h"
    "cef/include/cef_request.h"
    "cef/include/cef_request_context.h"
    "cef/include/cef_request_context_handler.h"
    "cef/include/cef_request_handler.h"
    "cef/include/cef_resource_bundle.h"
    "cef/include/cef_resource_bundle_handler.h"
    "cef/include/cef_resource_handler.h"
    "cef/include/cef_response.h"
    "cef/include/cef_response_filter.h"
    "cef/include/cef_sandbox_win.h"
    "cef/include/cef_scheme.h"
    "cef/include/cef_server.h"
    "cef/include/cef_ssl_info.h"
    "cef/include/cef_ssl_status.h"
    "cef/include/cef_stream.h"
    "cef/include/cef_string_visitor.h"
    "cef/include/cef_task.h"
    "cef/include/cef_thread.h"
    "cef/include/cef_trace.h"
    "cef/include/cef_urlrequest.h"
    "cef/include/cef_v8.h"
    "cef/include/cef_values.h"
    "cef/include/cef_version.h"
    "cef/include/cef_waitable_event.h"
    "cef/include/cef_web_plugin.h"
    "cef/include/cef_x509_certificate.h"
    "cef/include/cef_xml_reader.h"
    "cef/include/cef_zip_reader.h"
    "cef/include/internal/cef_export.h"
    "cef/include/internal/cef_logging_internal.h"
    "cef/include/internal/cef_ptr.h"
    "cef/include/internal/cef_string.h"
    "cef/include/internal/cef_string_list.h"
    "cef/include/internal/cef_string_map.h"
    "cef/include/internal/cef_string_multimap.h"
    "cef/include/internal/cef_string_types.h"
    "cef/include/internal/cef_string_wrappers.h"
    "cef/include/internal/cef_thread_internal.h"
    "cef/include/internal/cef_time.h"
    "cef/include/internal/cef_trace_event_internal.h"
    "cef/include/internal/cef_types.h"
    "cef/include/internal/cef_types_win.h"
    "cef/include/internal/cef_types_wrappers.h"
    "cef/include/internal/cef_win.h"
    "cef/include/test/cef_test_helpers.h"
    "cef/include/test/cef_translator_test.h"
    "cef/include/views/cef_box_layout.h"
    "cef/include/views/cef_browser_view.h"
    "cef/include/views/cef_browser_view_delegate.h"
    "cef/include/views/cef_button.h"
    "cef/include/views/cef_button_delegate.h"
    "cef/include/views/cef_display.h"
    "cef/include/views/cef_fill_layout.h"
    "cef/include/views/cef_label_button.h"
    "cef/include/views/cef_layout.h"
    "cef/include/views/cef_menu_button.h"
    "cef/include/views/cef_menu_button_delegate.h"
    "cef/include/views/cef_panel.h"
    "cef/include/views/cef_panel_delegate.h"
    "cef/include/views/cef_scroll_view.h"
    "cef/include/views/cef_textfield.h"
    "cef/include/views/cef_textfield_delegate.h"
    "cef/include/views/cef_view.h"
    "cef/include/views/cef_view_delegate.h"
    "cef/include/views/cef_window.h"
    "cef/include/views/cef_window_delegate.h"
    "cef/include/wrapper/cef_byte_read_handler.h"
    "cef/include/wrapper/cef_closure_task.h"
    "cef/include/wrapper/cef_helpers.h"
    "cef/include/wrapper/cef_message_router.h"
    "cef/include/wrapper/cef_resource_manager.h"
    "cef/include/wrapper/cef_scoped_temp_dir.h"
    "cef/include/wrapper/cef_stream_resource_handler.h"
    "cef/include/wrapper/cef_xml_object.h"
    "cef/include/wrapper/cef_zip_archive.h"
    "main.cpp"
)
source_group("Quelldateien" FILES ${Quelldateien})

set(ALL_FILES
    ${Headerdateien}
    ${Quelldateien}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "utilities")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE trilogyuisubprocess)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$(CoreLibraryDependencies)"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")


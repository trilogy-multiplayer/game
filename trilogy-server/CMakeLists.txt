set(PROJECT_NAME trilogy-server)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "common.hpp"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__core
    "core/core.hpp"
)
source_group("Header Files\\core" FILES ${Header_Files__core})

set(Header_Files__networking
    "networking/server_networking.hpp"
)
source_group("Header Files\\networking" FILES ${Header_Files__networking})

set(Header_Files__networking__entities
    "networking/entities/player_entity.hpp"
)
source_group("Header Files\\networking\\entities" FILES ${Header_Files__networking__entities})

set(Header_Files__networking__modules
    "networking/modules/module_player-sync.hpp"
    "networking/modules/module_sync.hpp"
)
source_group("Header Files\\networking\\modules" FILES ${Header_Files__networking__modules})

set(Source_Files
    "server.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__core
    "core/core.cpp"
)
source_group("Source Files\\core" FILES ${Source_Files__core})

set(Source_Files__networking
    "networking/server_networking.cpp"
)
source_group("Source Files\\networking" FILES ${Source_Files__networking})

set(Source_Files__networking__entities
    "networking/entities/player_entity.cpp"
)
source_group("Source Files\\networking\\entities" FILES ${Source_Files__networking__entities})

set(Source_Files__networking__modules
    "networking/modules/module_player-sync.cpp"
)
source_group("Source Files\\networking\\modules" FILES ${Source_Files__networking__modules})

set(ALL_FILES
    ${Header_Files}
    ${Header_Files__core}
    ${Header_Files__networking}
    ${Header_Files__networking__entities}
    ${Header_Files__networking__modules}
    ${Source_Files}
    ${Source_Files__core}
    ${Source_Files__networking}
    ${Source_Files__networking__entities}
    ${Source_Files__networking__modules}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "game")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Includes for CMake from *.props
################################################################################
use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "../trilogy-shared/trilogy-shared.vcxitems")

set(ROOT_NAMESPACE trilogyserver)

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
            "_DEBUG;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "_MBCS"
        ">"
        "_CONSOLE"
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
                /std:c++17;
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


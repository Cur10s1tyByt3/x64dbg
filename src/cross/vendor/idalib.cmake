set(IDASDK ${ida-sdk_SOURCE_DIR}/src)
if(NOT EXISTS ${IDASDK}/include/ida.hpp)
    message(FATAL_ERROR "Missing header file in IDA SDK: ${IDASDK}/include/ida.hpp")
endif()

set(IMPORTED_PROPERTY "IMPORTED_LOCATION")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        message(FATAL_ERROR "Unsupported architecture ${CMAKE_SYSTEM_PROCESSOR} for platform ${CMAKE_SYSTEM_NAME}")
    endif()
    set(PLATFORM_DEFINE -D__LINUX__=1)
    set(IDA_KERNEL_LIB "${IDASDK}/lib/x64_linux_gcc_64/libida.so")
    set(IDA_LIB_LIB "${IDASDK}/lib/x64_linux_gcc_64/libidalib.so")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
        message(FATAL_ERROR "Unsupported architecture ${CMAKE_SYSTEM_PROCESSOR} for platform ${CMAKE_SYSTEM_NAME}")
    endif()
    set(PLATFORM_DEFINE -D__NT__=1)
    set(IDA_KERNEL_LIB "${IDASDK}/lib/x64_win_vc_64/ida.lib")
    set(IDA_LIB_LIB "${IDASDK}/lib/x64_win_vc_64/idalib.lib")
    set(IMPORTED_PROPERTY "IMPORTED_IMPLIB")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM_DEFINE -D__MAC__=1)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(IDA_KERNEL_LIB "${IDASDK}/lib/arm64_mac_clang_64/libida.dylib")
        set(IDA_LIB_LIB "${IDASDK}/lib/arm64_mac_clang_64/libidalib.dylib")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(IDA_KERNEL_LIB "${IDASDK}/lib/x64_mac_clang_64/libida.dylib")
        set(IDA_LIB_LIB "${IDASDK}/lib/x64_mac_clang_64/libidalib.dylib")
    else()
        message(FATAL_ERROR "Unsupported architecture ${CMAKE_SYSTEM_PROCESSOR} for platform ${CMAKE_SYSTEM_NAME}")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

add_library(idasdk::headers INTERFACE IMPORTED)
target_include_directories(idasdk::headers INTERFACE ${IDASDK}/include)
target_compile_definitions(idasdk::headers INTERFACE __EA64__=1 ${PLATFORM_DEFINE})

add_library(idasdk::kernel SHARED IMPORTED)
set_target_properties(idasdk::kernel PROPERTIES
    ${IMPORTED_PROPERTY} ${IDA_KERNEL_LIB}
    INTERFACE_LINK_LIBRARIES idasdk::headers
)

add_library(idasdk::lib SHARED IMPORTED)
set_target_properties(idasdk::lib PROPERTIES
    ${IMPORTED_PROPERTY} ${IDA_LIB_LIB}
    INTERFACE_LINK_LIBRARIES idasdk::kernel
)

target_link_libraries(idalib INTERFACE idasdk::lib)

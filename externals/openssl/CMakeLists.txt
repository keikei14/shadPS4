add_library(ssl INTERFACE)
add_library(crypto INTERFACE)

target_include_directories(ssl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(crypto INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/external/openssl.zip)
    message(STATUS "Downloading OpenSSL prebuilts...")

    file(DOWNLOAD https://github.com/IndySockets/OpenSSL-Binaries/raw/refs/heads/master/openssl-3_x/openssl-3.3.2-win64.zip ${CMAKE_BINARY_DIR}/external/openssl.zip SHOW_PROGRESS
        STATUS FILE_STATUS)

    list(GET FILE_STATUS 0 STATUS_CODE)
    if (NOT STATUS_CODE EQUAL 0)
        file(REMOVE ${CMAKE_BINARY_DIR}/external/openssl.zip)
        message(FATAL_ERROR "Failed to download OpenSSL prebuilts")
    endif()
endif()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/external/openssl/lib)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external/openssl/lib)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/external/openssl.zip DESTINATION ${CMAKE_BINARY_DIR}/external/openssl/lib)
endif()

target_link_libraries(ssl INTERFACE ${CMAKE_BINARY_DIR}/external/openssl/lib/libssl-3-x64.dll)
target_link_libraries(crypto INTERFACE ${CMAKE_BINARY_DIR}/external/openssl/lib/libcrypto-3-x64.dll)

add_library(OpenSSL::SSL ALIAS ssl)
add_library(OpenSSL::Crypto ALIAS crypto)

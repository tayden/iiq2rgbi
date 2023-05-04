cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)

# Add FetchContent functionality
include(FetchContent)

project(iiq2rgbi)

# SDK Major version
set(SDK_MAJOR_VERSION "3")

# System name (lower case)
string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LC)

# File format extension
if(WIN32)
    set(SDK_PACKAGE_EXT ".zip")
else()
    set(SDK_PACKAGE_EXT ".tgz")
endif()

# Account for ARM based linux systems
if(NOT APPLE AND UNIX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(LINUX_ARCH "-arm64")
else()
    set(LINUX_ARCH "")
endif()

# Setup ImageSDK Download
FetchContent_Declare(ImageSDK
        URL https://developer.phaseone.com/sdk/releases/imagesdk/${SDK_MAJOR_VERSION}/p1imagesdk-${SYSTEM_NAME_LC}${LINUX_ARCH}${SDK_PACKAGE_EXT}
        SOURCE_DIR ImageSDK
        )
# Download ImageSDK for the current system
message(STATUS "Downloading ImageSDK...")
FetchContent_Populate(ImageSDK)
find_package(ImageSDK CONFIG REQUIRED HINTS ${CMAKE_CURRENT_BINARY_DIR}/ImageSDK)

## Add executable

add_executable(iiq2rgbi main.cpp)

## Link dynamic with ImageSDK

target_link_libraries(iiq2rgbi PRIVATE
        ImageSDK::ImageSdkCpp
        )

if(WIN32)
    #Copy ImageSDK DLL / SO file to build directory
    add_custom_command(TARGET iiq2rgbi POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:ImageSDK::ImageSdkCpp> $<TARGET_FILE_DIR:iiq2rgbi>
            )
    #Copy Stitching & Stacking libraries deps - needed only if you call stitching or stacking APIs
    add_custom_command(TARGET iiq2rgbi POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ImageSDK/lib/libifcoremd.dll $<TARGET_FILE_DIR:iiq2rgbi>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ImageSDK/lib/libifportmd.dll $<TARGET_FILE_DIR:iiq2rgbi>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ImageSDK/lib/libmmd.dll $<TARGET_FILE_DIR:iiq2rgbi>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ImageSDK/lib/P1CIRSTACK.dll $<TARGET_FILE_DIR:iiq2rgbi>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ImageSDK/lib/Qt5Core.dll $<TARGET_FILE_DIR:iiq2rgbi>
            )
endif()

# Copy the directory where the sensor profiles are located
# to where you program is executed (its current working dir.)
add_custom_command(TARGET iiq2rgbi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/ImageSDK/SensorProfiles $<TARGET_FILE_DIR:iiq2rgbi>/SensorProfiles
        COMMENT "Copy 'SensorProfiles' to build directory"
        )

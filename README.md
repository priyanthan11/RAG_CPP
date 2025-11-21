Using vcpkg (Recommended on Windows)

vcpkg automates installing libraries like curl, zlib, OpenSSL, etc., and integrates easily with CMake.

Step 1: Install vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

Step 2: Install curl
.\vcpkg install curl:x64-windows

This installs libcurl and dependencies (OpenSSL, zlib) for 64-bit Windows.

Step 3: Integrate vcpkg with CMake
.\vcpkg integrate install

This allows CMake to find curl automatically.

Step 4: Update your CMakeLists.txt

# Find curl package

find_package(CURL REQUIRED)

target_link_libraries(rag_app PRIVATE CURL::libcurl)

Now #include <curl/curl.h> works, and linking is automatic.

Manual Prebuilt libcurl

If you prefer manual setup, you can download prebuilt binaries:

Go to https://curl.se/windows/
→ download a Windows 64-bit Visual Studio build.

Extract to external/libcurl/ folder:

rag-cpp/
├─ external/
│ └─ libcurl/
│ ├─ include/ (contains curl/\*.h)
│ └─ lib/ (contains curl.lib and dlls)

Update CMakeLists.txt:

# Add include path

target_include_directories(rag_app PUBLIC ${PROJECT_SOURCE_DIR}/external/libcurl/include)

# Add lib path

link_directories(${PROJECT_SOURCE_DIR}/external/libcurl/lib)

# Link libcurl

target_link_libraries(rag_app PRIVATE libcurl.lib ws2_32.lib winmm.lib wldap32.lib)

Note: On Windows, libcurl depends on ws2_32.lib, winmm.lib, wldap32.lib — you must link them.

Make sure libcurl.dll is in your executable folder or PATH.

cmake_minimum_required(VERSION 3.5)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/Hidend/Documents/VsShit/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
project(GTA_SA_ASI_Mod LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
	RakHook
	GIT_REPOSITORY https://github.com/povargek/RakHook.git #ty povargek for adding R2 and r5!
	GIT_TAG 7685236c9bb929d741b73d9a5f6d390c8cad6e7f
)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(rakhook json)

add_library(${PROJECT_NAME} SHARED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_sources(${PROJECT_NAME} PRIVATE 
"src/main.cpp"
"src/plugin.cpp"
"src/ZoneManager.cpp"
)

find_package(minhook CONFIG REQUIRED)
find_package(cpr REQUIRED)
find_package(websocketpp CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE minhook::minhook rakhook cpr::cpr nlohmann_json::nlohmann_json websocketpp::websocketpp)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".asi")
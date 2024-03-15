cmake_minimum_required(VERSION 3.27)
project(MusicPlayer)

SET(CMAKE_SYSTEM_NAME Windows)

SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
SET(CMAKE_RANLIB x86_64-w64-mingw32-ranlib)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -s")

include_directories(../nya_common)
include_directories(../nya_common/3rdparty)
include_directories(../nya_common/3rdparty/imgui)
include_directories(../nya_common/3rdparty/imgui/backends)
include_directories(../nya_common/3rdparty/taglib)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/taglib)
link_directories(${CMAKE_SOURCE_DIR}/../nya_common/lib64)

add_compile_definitions(IMGUI_USER_CONFIG="../../nya_imconfig.h" NYA_BACKEND_DX11 NYA_AUDIO_BASS NYA_APP_WINDOWNAME="nya :3" NYA_FONTNAME="FranxurterTotallyMedium-gxwjp.ttf" NYA_FONTSIZE=64.0)
add_executable(MusicPlayer
        ../nya_common/3rdparty/imgui/imgui.cpp
        ../nya_common/3rdparty/imgui/backends/imgui_impl_dx11.cpp
        ../nya_common/3rdparty/imgui/imgui_draw.cpp
        ../nya_common/3rdparty/imgui/imgui_tables.cpp
        ../nya_common/3rdparty/imgui/imgui_widgets.cpp
        ../nya_common/nya_commonfuncs.cpp
        ../nya_common/nya_commondraw.cpp
        ../nya_common/nya_commonaudio.cpp
        ../nya_common/nya_dx11_appbase.cpp
        musicplayer_defs.cpp
        musicplayer_song.cpp
        musicplayer_state.cpp
        musicplayer_playlist.cpp
        musicplayer_saveload.cpp
        musicplayer_draw.cpp
        main.cpp)

target_link_libraries(MusicPlayer d3d11)
target_link_libraries(MusicPlayer d3dcompiler)
target_link_libraries(MusicPlayer tag)
target_link_libraries(MusicPlayer zlib)
target_link_libraries(MusicPlayer comdlg32)
target_link_libraries(MusicPlayer bass)
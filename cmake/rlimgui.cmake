
include_directories(lib/imgui .)


set(
    IMGUI_SRCS
    "src/rlImGui.cpp"
    "lib/imgui/imgui.cpp"
    "lib/imgui/imgui_demo.cpp"
    "lib/imgui/imgui_draw.cpp"
    "lib/imgui/imgui_tables.cpp"
    "lib/imgui/imgui_widgets.cpp"

    "lib/imgui/imconfig.h"
    "lib/imgui/imgui_internal.h"
    "lib/imgui/imgui.h"
)

add_library(rlimgui ${IMGUI_SRCS})
target_link_libraries(rlimgui raylib)

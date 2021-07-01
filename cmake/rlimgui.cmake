
include_directories(lib/imgui)
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

include_directories(lib/implot)
set(
    IMPLOT_SRCS
    "lib/implot/implot.cpp"
    "lib/implot/implot_demo.cpp"
    "lib/implot/implot_items.cpp"
    
    "lib/implot/implot.h"
    "lib/implot/implot_internal.h"
)

add_library(rlimgui ${IMGUI_SRCS})
target_link_libraries(rlimgui PRIVATE raylib)
add_library(implot ${IMPLOT_SRCS})
target_link_libraries(implot PRIVATE raylib rlimgui)

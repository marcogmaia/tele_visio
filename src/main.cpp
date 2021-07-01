#include <algorithm>

#include "imgui.h"
#include "implot.h"
#include "raylib.h"
#include "rlImGui.h"

#include "utils/rng.hpp"
#include "utils/texture.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <type_traits>

namespace asio = boost::asio;
using asio::ip::tcp;

// [x] show graph data with imgui
// [ ] receive data from tcp with asio

rng_t rng{};

void main_gui() {
    BeginRLImGui();

    ImGui::Begin("settings");

    if(ImGui::Button("click_me")) {
        static int count = 0;
        ++count;
        fmt::print("oie xD {}\n", count);
    }

    ImGui::End();

    {
        ImGui::Begin("custom plot", nullptr, ImGuiWindowFlags_NoCollapse);

        constexpr auto num_pts = 1001;
        static std::array<float, num_pts> xs;
        static std::array<float, num_pts> ys;
        for(int i = 0; i < 1001; ++i) {
            xs[i] = i * 0.001f;
            ys[i] = rng.random_double();
        }

        ImPlot::BeginPlot("my_plot");
        ImPlot::PlotLine("random", xs.data(), ys.data(), xs.size());
        ImPlot::EndPlot();

        ImGui::End();
    }

    EndRLImGui();
}

void draw_texture(Texture2D& texture) {
    // we need to invert the UV map y axis because of OpenGL
    auto src_rect = Rectangle{
        0,
        0,
        static_cast<float>(texture.width),
        static_cast<float>(-texture.height),
    };
    DrawTextureRec(texture, src_rect, Vector2{0, 0}, WHITE);
}

void test_asio() {}

int main() {
    InitWindow(1024, 768, "TeleVisio");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(60);
    render_texture_t render_tex{1920, 1080};

    SetupRLImGui();
    ImPlot::CreateContext();
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLANK);

        BeginTextureMode(render_tex.render_texture);
        ClearBackground(BLANK);
        main_gui();
        EndTextureMode();

        auto& tex = render_tex.render_texture.texture;
        draw_texture(tex);

        EndDrawing();
    }
    ImPlot::DestroyContext();
    ShutdownRLImGui();
    CloseWindow();
}

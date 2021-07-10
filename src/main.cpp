#include <algorithm>
#include <future>
#include <memory>

#include "imgui.h"
#include "implot.h"
#include "raylib.h"
#include "rlImGui.h"
#include "server.hpp"

#include "utils/rng.hpp"
#include "utils/texture.hpp"
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/detail/error_code.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <string_view>
#include <type_traits>
namespace asio = boost::asio;
using asio::ip::tcp;

// [x] show graph data with imgui
// [ ] receive data from tcp with asio

namespace tv {


rng_t rng{};


std::unique_ptr<Server> init_server(asio::io_context& ioc, std::string_view port_str) {
    try {
        auto port = std::atoi(port_str.data());
        // Server server(ioc, port);
        auto uptr_server = std::make_unique<Server>(ioc, port);
        fmt::print("server initialized at port: {}\n", port);
        return uptr_server;
    } catch(std::exception& e) { std::cerr << "Exception: " << e.what() << "\n"; }
    return nullptr;
}


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

        ImPlot::FitNextPlotAxes();
        ImPlot::BeginPlot("my_plot");
        auto& circular_buffer = Session::circular_buffer;
        ImPlot::PlotLine("Buffer", circular_buffer.linearize(), circular_buffer.size());
        ImPlot::EndPlot();

        ImGui::End();
    }


    {
        static boost::asio::io_context io_context;
        static std::thread ioc_thread;

        ImGui::Begin("Server");
        static bool server_initialized = false;
        static std::array<char, 16> port_buffer;
        static std::unique_ptr<Server> uptr_server;

        ImGui::InputText("port", port_buffer.data(), port_buffer.size());
        ImGui::SameLine();
        if(ImGui::Checkbox("init server", &server_initialized)) {
            if(server_initialized) {
                fmt::print("initializing server...\n");
                uptr_server = init_server(io_context, port_buffer.data());
            } else {
                fmt::print("shutting down the server...\n");
                io_context.stop();
                io_context.restart();
                uptr_server.reset();
                fmt::print("server is off\n");
            }
        }
        if(uptr_server) {
            io_context.poll();
        }
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

}  // namespace tv

int main() {
    using namespace tv;
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

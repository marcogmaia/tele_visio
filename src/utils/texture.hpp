#pragma once

#include <string>

#include "raylib.h"

/**
 * @brief Class wrapper to use RAII idiom
 *
 */
struct render_texture_t {
    RenderTexture2D render_texture = {0};

    render_texture_t(const int width, const int height) {
        render_texture = LoadRenderTexture(width, height);
    }

    render_texture_t(const render_texture_t&) = delete;
    render_texture_t& operator=(const render_texture_t&) = delete;

    ~render_texture_t() {
        // checks if RenderTexture2D is valid is done inside raylib internals
        UnloadRenderTexture(render_texture);
    }

    /**
     * @brief Clears the RenderTexture
     *
     * @param color optional color to clear the texture
     */
    inline void clear(const Color& color = BLANK) {
        BeginTextureMode(render_texture);
        ClearBackground(color);
        EndTextureMode();
    }
};

struct texture_t {
    Texture2D texture = {0};

    texture_t(const std::string& file_path) {
        texture = LoadTexture(file_path.c_str());
    }

    ~texture_t() {
        UnloadTexture(texture);
    }
};

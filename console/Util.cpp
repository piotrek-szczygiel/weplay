#include "Util.hpp"

bool threshold(float x, float y, float threshold) { return std::fabs(x - y) < threshold; }

float fade(float x, float y, float offset)
{
    if (x > y) {
        x -= offset;
        if (x < y) {
            x = y;
        }
    } else if (x < y) {
        x += offset;
        if (x > y) {
            x = y;
        }
    }

    return x;
}

float sin_out_easing(float time, float start, float distance, float duration)
{
    return distance * std::sin(time / duration * (M_PI / 2)) + start;
}

void draw_scaled(RenderTexture2D framebuffer)
{
    auto width = static_cast<float>(framebuffer.texture.width);
    auto height = static_cast<float>(framebuffer.texture.height);
    auto screen_width = static_cast<float>(GetScreenWidth());
    auto screen_height = static_cast<float>(GetScreenHeight());
    float scale = std::min(screen_width / width, screen_height / height);

    Rectangle source {
        0.0F,
        0.0F,
        width,
        -height,
    };

    Rectangle dest {
        (screen_width - width * scale) / 2.0F,
        (screen_height - height * scale) / 2.0F,
        width * scale,
        height * scale,
    };

    DrawTexturePro(framebuffer.texture, source, dest, {}, 0.0F, WHITE);
}

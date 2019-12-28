#include <random>
#include <raylib.h>
#include <vector>

namespace Pong {

struct ParticleProps {
    Vector2 position;
    Vector2 speed, speed_variation;
    Color color_begin, color_end;

    float size_begin, size_end, size_variation;
    float life;
};

class ParticleSystem {
public:
    ParticleSystem() = default;

    void emit(const ParticleProps& particle_props);

    void update(float dt);
    void draw();
    void clear() { m_particles.clear(); }

private:
    struct Particle {
        Vector2 position;
        Vector2 speed;
        Color color_begin, color_end;
        float rotation { 0.0F };
        float size_begin, size_end;

        float life { 1.0F };
        float life_remaining;

        bool active { true };
    };

    std::vector<Particle> m_particles;

    std::mt19937 m_gen { std::random_device {}() };
    std::uniform_real_distribution<> m_dis { 0.0F, 1.0F };
};
}

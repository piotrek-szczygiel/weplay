#include "ParticleSystem.hpp"
#include "random.hpp"

using Random = effolkronium::random_static;

namespace Pong {

Color lerp(Color s, Color e, float time) //  0.0F < time <= 1.0F
{
    float r_diff = e.r - s.r;
    float g_diff = e.g - s.g;
    float b_diff = e.b - s.b;
    float a_diff = e.a - s.a;

    float r_val = s.r + r_diff * time;
    float g_val = s.g + g_diff * time;
    float b_val = s.b + b_diff * time;
    float a_val = s.a + a_diff * time;

    auto r_val_c = static_cast<unsigned char>(r_val);
    auto g_val_c = static_cast<unsigned char>(g_val);
    auto b_val_c = static_cast<unsigned char>(b_val);
    auto a_val_c = static_cast<unsigned char>(a_val);

    return { r_val_c, g_val_c, b_val_c, a_val_c };
}

float lerp(float s, float e, float time)
{
    float diff = e - s;

    return s + diff * time;
}

void ParticleSystem::emit(const ParticleProps& particle_props)
{
    m_particles.emplace_back();

    Particle& particle = m_particles.back();

    particle.position = particle_props.position;
    particle.rotation = Random::get(0.0F, 1.0F) * 2.0F * PI;

    particle.speed = particle_props.speed;
    particle.speed.x += particle_props.speed_variation.x * (Random::get(0.0F, 1.0F) - 0.5F);
    particle.speed.y += particle_props.speed_variation.y * (Random::get(0.0F, 1.0F) - 0.5F);

    particle.color_begin = particle_props.color_begin;
    particle.color_end = particle_props.color_end;

    float random_variation = particle_props.size_variation * (Random::get(0.0F, 1.0F) - 0.5F);

    particle.size_begin = particle_props.size_begin + random_variation;
    particle.size_end = particle_props.size_end;

    particle.life = particle_props.life;
    particle.life_remaining = particle_props.life;
}

void ParticleSystem::update(float dt)
{

    for (auto& particle : m_particles) {
        if (particle.life_remaining <= 0.0F) {
            particle.active = false;
            continue;
        }

        particle.life_remaining -= dt;
        particle.position.x += particle.speed.x * dt;
        particle.position.y += particle.speed.y * dt;
        particle.rotation += 0.05F * dt;
    }

    for (int i = 0; i < m_particles.size(); ++i) {
        if (!m_particles[i].active) {
            m_particles.erase(m_particles.begin() + i);
            --i;
        }
    }
}

void ParticleSystem::draw()
{
    for (auto& particle : m_particles) {
        float life_stamp = 1.0F - particle.life_remaining / particle.life;

        Color color = lerp(particle.color_begin, particle.color_end, life_stamp);

        float size = lerp(particle.size_begin, particle.size_end, life_stamp);

        Rectangle rect { particle.position.x, particle.position.y, size, size };
        Vector2 origin { size / 2, size / 2 };
        DrawRectanglePro(rect, origin, particle.rotation, color);
    }
}

}

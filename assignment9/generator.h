#ifndef ASSIGNMENT9_GENERATOR_HPP
#define ASSIGNMENT9_GENERATOR_HPP
#include <vector>

#include "particle.h"
#include "random.h"
class Generator {
   public:
    // initialization
    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) {
        m_color = color;
        m_dead_color = dead_color;
        m_color_randomness = color_randomness;
    }
    void SetLifespan(float lifespan, float lifespan_randomness,
                     int desired_num_particles) {
        m_lifespan = lifespan;
        m_lifespan_randomness = lifespan_randomness;
        m_desired_num_particles = desired_num_particles;
    }
    void SetMass(float mass, float mass_randomness) {
        m_mass = mass;
        m_mass_randomness = mass_randomness;
    }

    // on each timestep, create some particles
    virtual int numNewParticles(float current_time, float dt) const {
        return int(m_desired_num_particles * dt / m_lifespan);
    }
    virtual Particle* Generate(float current_time, int i) = 0;

    // for the gui
    virtual void Paint() const {}
    void Restart() { m_random = Random(); }
    virtual ~Generator() = default;

   protected:
    Vec3f rollColor() {
        Vec3f base = m_color;
        base += m_random.randomVector() * m_color_randomness;
        base.Clamp(0, 1);
        return base;
    }
    float rollLifespan() {
        float base = m_lifespan;
        base += m_random.next() * m_lifespan_randomness;
        base = std::max(base, 0.0f);
        return base;
    }
    float rollMass() {
        float base = m_mass;
        base += m_random.next() * m_mass_randomness;
        base = std::max(base, 0.0f);
        return base;
    }
    Vec3f m_color;
    Vec3f m_dead_color;
    float m_color_randomness;
    float m_lifespan;
    float m_lifespan_randomness;
    int m_desired_num_particles;
    float m_mass;
    float m_mass_randomness;
    Random m_random;
};
class HoseGenerator : public Generator {
   public:
    HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity,
                  float velocity_randomness)
        : m_position(position),
          m_position_randomness(position_randomness),
          m_velocity(velocity),
          m_velocity_randomness(velocity_randomness) {}
    Particle* Generate(float current_time, int i) override {
        Vec3f position = m_position;
        position += m_random.randomVector() * m_position_randomness;
        Vec3f velocity = m_velocity;
        velocity += m_random.randomVector() * m_velocity_randomness;
        Vec3f color = rollColor();
        float lifespan = rollLifespan();
        float mass = rollMass();
        return new Particle(position, velocity, color, m_dead_color, mass,
                            lifespan);
    }

   private:
    Vec3f m_position;
    float m_position_randomness;
    Vec3f m_velocity;
    float m_velocity_randomness;
};

class RingGenerator : public Generator {
   public:
    RingGenerator(float position_randomness, Vec3f velocity,
                  float velocity_randomness)
        : m_position_randomness(position_randomness),
          m_velocity(velocity),
          m_velocity_randomness(velocity_randomness) {}
    int numNewParticles(float current_time, float dt) const override {
        float scaling_factor = 0.03 * current_time * current_time + 1;
        return int(scaling_factor * m_desired_num_particles * dt / m_lifespan);
    }
    Particle* Generate(float current_time, int i) override {
        float radius =
            current_time * 0.3 + m_random.next() * m_position_randomness;
        float theta = m_random.next() * M_PI * 2.0;
        Vec3f position =
            Vec3f(radius * cos(theta), offsetY, radius * sin(theta));
        Vec3f velocity = m_velocity;
        velocity += m_random.randomVector() * m_velocity_randomness;
        Vec3f color = rollColor();
        float lifespan = rollLifespan();
        float mass = rollMass();
        return new Particle(position, velocity, color, m_dead_color, mass,
                            lifespan);
    }
    void Paint() const override;

   private:
    float offsetY = -4.0f;
    float m_position_randomness;
    Vec3f m_velocity;
    float m_velocity_randomness;
};
#endif /* ASSIGNMENT9_GENERATOR_HPP */

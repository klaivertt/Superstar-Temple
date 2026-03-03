#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "../../Common.hpp"

#pragma region ParticleSystem

namespace Particles
{
    class Sprite;
    typedef struct Animation
    {
        sf::Sprite* sprite;
        sf::Vector2u textureSize;
        unsigned int nbFrame;
        unsigned int frameRate;
        sf::IntRect firstFrame;
        bool loop;

        unsigned int frame;
        float timer;
        bool animIsFinished;
        bool pause;

        std::vector<sf::Texture> imagesTexture;

        int startImageValue;
    } Animation;
}
#pragma endregion


#define PARTICLE_DEFAULT_COLORS_FACTOR { 1.f, 1.f, 1.f, 1.f }

struct Particle
{
    sf::Sprite sprite;
    float lifetime = 0.f;
    float age = 0.f;

    bool destroy = false;
    Particles::Animation animation;

    sf::Vector2f instanteVelocity = { 0.f, 0.f };
    std::array<float, 4> colorFactor = PARTICLE_DEFAULT_COLORS_FACTOR;
};

struct ParticleTextureData
{
	const sf::Texture* texture = nullptr;
    int nbFrame = 0;
    int frameRate = 0;
    bool loopAnimation = false;
    sf::Color startColor = sf::Color::White;
    sf::Color endColor = sf::Color::Transparent;
};

class ParticleSystem
{
public:
    // Constructors
    ParticleSystem() = default;
    // ParticleTextureData need to be completed
    ParticleSystem(
        const ParticleTextureData& textureData,
        sf::Vector2f movementDirection = { 0.f, 0.f },
        sf::Vector2f randomPosArray = { 0.f, 0.f }
    );

    // Core behavior
    void update(float dt, sf::Vector2f externalVelocity);
    void draw(sf::RenderTarget& render);

    // Particle creation
    void addParticle(
        sf::Vector2f position,
        float lifeTime,
        float startAngle,
        float startIntensity,
        std::array<float, 4> colorFactor = PARTICLE_DEFAULT_COLORS_FACTOR
    );

    // Velocity effects
    void addVelocity(sf::Vector2f velocity);
    void addPositionedVelocity(
        sf::Vector2f position,
        float velocity,
        float effectRadius
    );

    // Accessors (optional but useful)
    std::vector<Particle>& getParticles();
    const std::vector<Particle>& getParticles() const;

private:
    ParticleTextureData m_animData;
    std::vector<Particle> m_particles;

    sf::Vector2f m_movementDirection = { 0.f, 0.f };
    sf::Vector2f m_randomPosArray = { 0.f, 0.f };
};









#endif // PARTICLE_SYSTEM_HPP

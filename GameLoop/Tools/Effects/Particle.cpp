#include "Particle.hpp"

#pragma region Animation functions



Particles::Animation CreateAnim(sf::Sprite* _sprite, unsigned int _nbFrame, unsigned int _frameRate, sf::IntRect _firstFrame, bool _loop)
{
	Particles::Animation newAnimation = {}; // initialise tous les champs à zéro / false

	newAnimation.sprite = _sprite;
	newAnimation.nbFrame = _nbFrame;
	newAnimation.frameRate = _frameRate;
	newAnimation.firstFrame = _firstFrame;
	newAnimation.loop = _loop;
	newAnimation.pause = false;

	if (_sprite)
	{
		const sf::Texture* texture = _sprite->getTexture();
		newAnimation.textureSize = texture->getSize();
	}
	return newAnimation;
}

void UpdateAnimation(Particles::Animation* _animation, float _dt)
{
	if (!_animation->pause)
	{
		//Security
		if (!_animation || !&_animation->sprite)
		{
			return;
		}

		_animation->timer += _dt;

		// Frame Gestion
		while (_animation->timer > 1.f / _animation->frameRate)
		{
			_animation->timer -= 1.f / _animation->frameRate;
			_animation->frame += 1;

			// Bust Total Frames
			if (_animation->frame >= _animation->nbFrame)
			{
				if (_animation->loop)
				{
					_animation->frame = 0;
				}
				else
				{
					_animation->frame = _animation->nbFrame - 1;
					_animation->animIsFinished = true;
					break;
				}
			}
		}

		sf::IntRect actualFrame = _animation->firstFrame;
		actualFrame.left = _animation->firstFrame.left + _animation->frame * _animation->firstFrame.width;

		sf::Vector2u textureRect = _animation->textureSize;
		if (textureRect.x != 0 && textureRect.y != 0)
		{
			while (actualFrame.left + actualFrame.width > (float)textureRect.x)
			{
				actualFrame.top += _animation->firstFrame.height;
				actualFrame.left -= textureRect.x;
			}
		}

		_animation->sprite->setTextureRect(actualFrame);
	}
}

bool AnimationIsFinsished(Particles::Animation* _animation)
{
	return _animation->animIsFinished;
}

#pragma endregion

ParticleSystem::ParticleSystem(const ParticleTextureData& textureData, sf::Vector2f movementDirection, sf::Vector2f randomPosArray)
{
	m_animData = textureData;
	m_movementDirection = movementDirection;
	m_randomPosArray = randomPosArray;
}

void ParticleSystem::update(float dt, sf::Vector2f externalVelocity)
{
	// Animation
	for (Particle& particle : m_particles)
	{
		particle.animation.sprite = &particle.sprite;
		UpdateAnimation(&particle.animation, dt);
	}

	// Physics + color
	for (Particle& particle : m_particles)
	{
		float factor = 1.f - (particle.age / particle.lifetime);

		sf::Uint8 r = static_cast<sf::Uint8>(
			std::clamp(255.f * std::pow(factor, particle.colorFactor[0]), 0.f, 255.f));
		sf::Uint8 g = static_cast<sf::Uint8>(
			std::clamp(255.f * std::pow(factor, particle.colorFactor[1]), 0.f, 255.f));
		sf::Uint8 b = static_cast<sf::Uint8>(
			std::clamp(255.f * std::pow(factor, particle.colorFactor[2]), 0.f, 255.f));
		sf::Uint8 a = static_cast<sf::Uint8>(
			std::clamp(255.f * std::pow(factor, particle.colorFactor[3]), 0.f, 255.f));

		particle.sprite.setColor(sf::Color(r, g, b, a));

		float reduction = expf(-5.f * dt);
		particle.instanteVelocity *= reduction;

		sf::Vector2f totalVelocity =
			externalVelocity * dt +
			particle.instanteVelocity * dt +
			m_movementDirection * dt;

		particle.sprite.move(totalVelocity);
	}

	// Lifetime & destruction
	for (Particle& particle : m_particles)
	{
		if (particle.animation.loop)
		{
			particle.age += dt;
			if (particle.age >= particle.lifetime)
				particle.destroy = true;
		}
		else
		{
			if (AnimationIsFinsished(&particle.animation))
				particle.destroy = true;
		}
	}

	// Remove destroyed particles (swap & pop)
	for (int i = static_cast<int>(m_particles.size()) - 1; i >= 0; --i)
	{
		if (m_particles[i].destroy)
		{
			m_particles[i] = m_particles.back();
			m_particles.pop_back();
		}
	}
}

void ParticleSystem::draw(sf::RenderTarget& render)
{
	for (Particle& particle : m_particles)
		render.draw(particle.sprite);
}

void ParticleSystem::addParticle(
	sf::Vector2f position,
	float lifeTime,
	float startAngle,
	float startIntensity,
	std::array<float, 4> colorFactor)
{
	Particle particle = {};
	particle.lifetime = lifeTime;
	particle.age = 0.f;
	particle.destroy = false;
	particle.colorFactor = colorFactor;

	// Random offset
	sf::Vector2f randomOffset(0.f, 0.f);
	if (m_randomPosArray.x != 0)
		randomOffset.x =
		(rand() % int(m_randomPosArray.x * 100)) / 100.f - m_randomPosArray.x / 2.f;
	if (m_randomPosArray.y != 0)
		randomOffset.y =
		(rand() % int(m_randomPosArray.y * 100)) / 100.f - m_randomPosArray.y / 2.f;

	position += randomOffset;


	// Sprite
	sf::IntRect firstFrame = { 0,0, (int)(m_animData.texture->getSize().x / m_animData.nbFrame), int(m_animData.texture->getSize().y) };

	particle.sprite = sf::Sprite();
	particle.sprite.setTexture(*m_animData.texture);
	particle.sprite.setTextureRect(firstFrame);
	particle.sprite.setPosition(position);
	particle.sprite.setOrigin(0.5f * firstFrame.width, 0.5f * firstFrame.height);
	particle.sprite.setRotation((float)(rand() % 360));

	// Animation
	particle.animation = CreateAnim(
		&particle.sprite,
		m_animData.nbFrame,
		m_animData.frameRate,
		firstFrame,
		m_animData.loopAnimation
	);

	// Initial velocity
	sf::Vector2f velocity;
	velocity.x = cosf(startAngle * M_PI / 180.f) * startIntensity;
	velocity.y = sinf(startAngle * M_PI / 180.f) * startIntensity;
	particle.instanteVelocity += velocity;

	m_particles.push_back(particle);
}

void ParticleSystem::addVelocity(sf::Vector2f velocity)
{
	for (Particle& particle : m_particles)
	{
		particle.instanteVelocity += velocity;
	}
}

void ParticleSystem::addPositionedVelocity(sf::Vector2f position, float velocity, float effectRadius)
{
	float radiusSq = effectRadius * effectRadius;

	for (Particle& particle : m_particles)
	{
		sf::Vector2f dir = particle.sprite.getPosition() - position;
		float distSq = dir.x * dir.x + dir.y * dir.y;

		if (distSq < radiusSq)
		{
			float dist = sqrtf(distSq);
			if (dist != 0)
				dir /= dist;

			float effect = 1.f - (dist / effectRadius);
			effect = std::clamp(effect, 0.f, 1.f);

			particle.instanteVelocity += dir * velocity * effect;
		}
	}
}

std::vector<Particle>& ParticleSystem::getParticles()
{
	return m_particles;
}

const std::vector<Particle>& ParticleSystem::getParticles() const
{
	return m_particles;
}

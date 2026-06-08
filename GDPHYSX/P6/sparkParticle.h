#pragma once

#include "particle.h"
#include <glm/glm.hpp>
#include <cstdlib>
#include <cmath>

static float randomFloat(float lo, float hi)
{
	return lo + (hi - lo) 
		* (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

class SparkParticle : public P6::Particle
{
	public:
		glm::vec3 color;
		float radius;

		SparkParticle() : color(1.f, 1.f, 1.f), radius(0.1f), lifespan(1.f), age (0.f)
		{
			Acceleration = glm::vec3(0.f, -10.f, 0.f);
			damping = 0.99f;
		}

		void Spawn(const glm::vec3& origin)
		{
			float spread = randomFloat(0.f, 0.3f);
			isDestroyed = false; 

			lifespan = randomFloat(1.f, 10.f);
			age = 0.f;

			radius = randomFloat(2.f, 10.f);

			// avoid random colors
			color = glm::vec3(randomFloat(0.3f, 1.f), randomFloat(0.3f, 1.f), randomFloat(0.3f, 1.f));

			Position = origin;
			StartPosition = origin;

			// Particle random launch velocity
			float vy = randomFloat(80.f, 120.f);
			float vx = randomFloat(-1.f, 1.f) * vy * spread;
			float vz = randomFloat(-1.f, 1.f) * vy * spread;
			Velocity = glm::vec3(vx, vy, vz);

			accumulatedForce = glm::vec3(0.f);
			Acceleration = glm::vec3(0.f, -10.f, 0.f);

		}

		void Update(float deltaTime) override
		{
			age += deltaTime;
			if (age >= lifespan)
			{
				Destroy();
				return;
			}

			Particle::Update(deltaTime);
		}

		float LifeFraction() const
		{
			return (lifespan > 0.f) ? (1.f - age / lifespan) : 0.f;
		}

	private:
		float lifespan;
		float age;
};
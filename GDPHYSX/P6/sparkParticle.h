#pragma once

#include "particle.h"
#include <glm/glm.hpp>
#include <cstdlib>
#include <cmath>

// Generates a random floating-point value within the specified range
static float randomFloat(float lo, float hi)
{
	return lo + (hi - lo) 
		* (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

/*
	Child class of Particle class used for the fountain firework simulation.
	Each spark particle stores its own color, radius, lifespan, and age
	Particle is automatically destroyed when lifetime expires
*/
class SparkParticle : public P6::Particle
{
	public:
		// Display color of the spark.
		glm::vec3 color;

		// Size
		float radius;

		SparkParticle() : color(1.f, 1.f, 1.f), radius(0.1f), lifespan(1.f), age (0.f)
		{
			Acceleration = glm::vec3(0.f, -10.f, 0.f);
			
			// Slight damping for air resistance.
			damping = 0.99f;
		}


		// Initalizes a spark at the given position and assigns random properties
		// such as lifespan, size, color, and velocity
		void Spawn(const glm::vec3& origin)
		{
			float spread = randomFloat(0.f, 0.3f);
			isDestroyed = false; 

			// Random lifespan between 1 and 10 seconds.
			lifespan = randomFloat(1.f, 10.f);
			age = 0.f;

			// Random radius between 2 and 10 meters.
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

			// Clear accumulated forces
			accumulatedForce = glm::vec3(0.f);

			// Reapply gravity.
			Acceleration = glm::vec3(0.f, -10.f, 0.f);

		}

		// Increments particle age and destroys the particle when lifespan reaches 0
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

	private:
		// Total lifetime of the particle in seconds.
		float lifespan;

		// Current age of the particle in seconds.
		float age;
};
#pragma once

#include <glm/glm.hpp>
#include<list>

#include "P6/particle.h"
#include "forceRegistry.h"
#include "gravityForceGenerator.h"

namespace P6
{
	// Physics manager responsible for updating all active particles in simulation
	class PhysicsWorld
	{
		public:
			// Collection of all active particles in the world.
			std::list<P6::Particle*> Particles;
			ForceRegistry forceRegistry;

			//Adds a particle to the simulation and registers
			// any forces that should affect it.
			void AddParticle(Particle* toAdd);

			// Updates all active particles 
			void Update(float time);

		private:
			//  Removes particles that have been marked for destruction
			void UpdateParticleList();

			GravityForceGenerator Gravity = GravityForceGenerator(glm::vec3(0.0f, -9.8f, 0.0f));

	};
}

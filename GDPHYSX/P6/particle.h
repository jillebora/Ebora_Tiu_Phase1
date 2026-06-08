#pragma once

#include <glm//glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace P6
{
	/*
		Base particle class used by physics engine
		Stores properties required for simulation

	*/
	class Particle
	{
		private:
			// Optional identifier used for debugging and tracking.
			std::string Name;

		protected:
			// Sum of all forces currently acting on the particle.
			glm::vec3 accumulatedForce = glm::vec3(0, 0, 0);

			// Determines whether the particle should be removed.
			bool isDestroyed = false;

			// Updates particle position using current velocity.
			void UpdatePosition(float deltaTime);
			
			// Updates particle velocity using acceleration and forces.
			void UpdateVelocity(float deltaTime);

		public:
			// Current world position.
			glm::vec3 Position;

			// Initial spawn position.
			glm::vec3 StartPosition;

			// Current velocity in units per second.
			glm::vec3 Velocity;
			
			// Current acceleration in units per second squared.
			glm::vec3 Acceleration;

			// Adds force to the particle
			void AddForce(glm::vec3 force);
			
			// Clears all accumulated force
			void ResetForce();

			// Particle mass in kilograms.
			float mass = 1.f;	//KG

			// Default damping = or "Air Drag"
			float damping = 0.9f;

			// Marks the particle for removal.
			void Destroy();

			// Returns whether the particle has been destroyed.
			bool IsDestroyed();

			// Updates particle physics.
			// called once per frame
			virtual void Update(float deltaTime);

			// Creates a normalized direction vector from a starting pos toward target pos
			static glm::vec3 makeVec(float scalar, glm::vec3 start, glm::vec3 target = glm::vec3(0.f))
			{
				// move towards the center
				return scalar * glm::normalize(target - start);
			}

			// Name setter and getter
			void setName(std::string n) { Name = n; }
			std::string getName() { return Name; }
			
			// Default constructor~
			Particle();

	};

}

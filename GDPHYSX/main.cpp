
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <list>
#include <chrono>
#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


//#include "P6/particle.h"
#include "P6/sparkParticle.h"
#include "physicsWorld.h"
#include "renderParticle.h"
#include "shader.h"
#include "model.h"
#include "inputHandler.h"

using namespace std;
using namespace std::chrono_literals;

static glm::mat4 BuildView(float yaw, float pitch, float radius)
{
	float pitchRad = glm::radians(pitch);
	float yawRad = glm::radians(yaw);

	float x = radius * cosf(pitchRad) * sinf(yawRad);
	float y = radius * sinf(pitchRad);
	float z = radius * cosf(pitchRad) * cosf(yawRad);

	glm::vec3 eye(x, y, z);
	glm::vec3 target(0.f);

	glm::vec3 up = (fabsf(pitch) > 89.f) ? glm::vec3(0.f, 0.f, (pitch > 0.f) ? -1.f : 1.f) : glm::vec3(0.f, 1.f, 0.f);

	return glm::lookAt(eye, target, up);

}

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	int sparkCount = 1000;
	std::cout << "P6 Fountain Firework Simulator\n";
	std::cout << "How many sparks to simulate? ";
	std::cin >> sparkCount;

	if (sparkCount < 1)
		sparkCount = 1;

	// Initialize GLFW
	if (!glfwInit())
	{
		return -1;
	}

	float windowWidth = 800;
	float windowHeight = 800;

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Phase 1 - Ebora and Tiu", NULL, NULL);

	P6::PhysicsWorld pWorld = P6::PhysicsWorld();


	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	InputHandler input;
	input.Register(window);

	// SHADERS
	Shader shader("Shaders/sphere.vert", "Shaders/sphere.frag");

	// MODEL
	Model sphere;
	sphere.loadFromObj("3D/sphere.obj");
	sphere.setScale(glm::vec3(1.f));

	// CAMERA

	glm::mat4 orthoProj = glm::ortho(-400.f, 400.f, -400.f, 400.f, -1000.f, 1000.f);
	glm::mat4 perspProj = glm::perspective(glm::radians(45.f), windowWidth / windowHeight, 1.f, 2000.f);

	glm::mat4 proj = orthoProj;
	glm::mat4 view = glm::mat4(1.f);

	// ---- Camera orbit state ------------------------------------------------
	float camYaw = 0.f;
	float camPitch = 0.f;
	float camRadius = 700.f;

	// Wire shader + camera into the model's RenderObject
	sphere.setShader(&shader.ID);
	sphere.setCamera(&proj, &view);
	//view = BuildView(45.f, 20.f, 700.f);

	// ==============================
	// ========= PARTICLES ==========
	// ==============================

	std::vector<SparkParticle*> sparks;
	std::list<RenderParticle*> renderParticles;

	for (int i = 0; i < sparkCount; i++)
	{
		SparkParticle* spark = new SparkParticle();

		float x = randomFloat(-5.f, 5.f);
		float z = randomFloat(-5.f, 5.f);

		spark->Spawn(glm::vec3(x, -350.f, z));

		pWorld.AddParticle(spark);
		sparks.push_back(spark);

		RenderParticle* rp = new RenderParticle(spark, sphere.getRenderObject(), spark->color);

		rp->Scale = glm::vec3(spark->radius);

		renderParticles.push_back(rp);
	}

	// ===================== TIME ======================

	using clock = std::chrono::high_resolution_clock;

	constexpr std::chrono::nanoseconds timestep(16ms);

	auto curr_time = clock::now();
	auto prev_time = curr_time;

	std::chrono::nanoseconds curr_ns(0);

	int currentRank = 1;
	bool printedResults = false;

	// LOOP

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		curr_time = clock::now();

		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(
			curr_time - prev_time
		);

		prev_time = curr_time;

		curr_ns += dur;

		if (curr_ns >= timestep)
		{
			constexpr float timestep_sec = timestep.count() / (float)1E09;

			curr_ns -= timestep;

			input.ProcessInput(
				timestep_sec,
				camYaw,
				camPitch
			);

			if (input.IsOrtho())
			{
				proj = orthoProj;
			}
			else
			{
				proj = perspProj;
			}

			view = BuildView(
				camYaw,
				camPitch,
				camRadius
			);

			pWorld.Update(timestep_sec);

			for (auto p = sparks.begin(); p != sparks.end();)
			{
				if ((*p)->IsDestroyed())
				{
					p = sparks.erase(p);
				}
				else
				{
					++p;
				}
			}
		}


		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// DRAW LIST

		for (auto* rp : renderParticles)
			rp->Draw();

		glfwSwapBuffers(window);
	}

	sphere.destroy();
	shader.destroy();

	glfwTerminate();
	return 0;
}

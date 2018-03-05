#include "game.hpp"

#include <glad/glad.h>

Game::Game() :
	m_window("GFX Coursework", {1280, 720}), // Create window
	m_viewport(m_window.size()),             // Create viewport
	m_camera({-15, 3, 5}, {-5, 3, -5}),      // Create camera
	m_shader("res/shaders/textured"),        // Create shader
	m_streetModel(m_shader, "res/models/street/street.obj"), // Create street model
	m_lampModel(m_shader, "res/models/lamp/lamp.obj"),       // Create lamp model
	m_houseModel(m_shader, "res/models/house/house.obj"),    // Create house model
	m_buildingModel1(m_shader, "res/models/buildings/building12.obj"), // Create building model 1
	m_buildingModel2(m_shader, "res/models/buildings/building07.obj"), // Create building model 2
	m_buildingModel3(m_shader, "res/models/buildings/building03.obj")  // Create building model 3
{
	// Enable vertical synchronisation
	m_window.verticalSync(true);

	// Set up OpenGL
	glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void Game::run()
{
	while (!m_window.shouldClose())
	{
		// Update and render
		update();
		render();
	}
}

void Game::update()
{
	// Process window events
	m_window.processEvents();

	// Process input events
	m_input.processEvents(m_window.events());

	// Check input
	if (m_input.keyJustReleased(SDLK_ESCAPE))
		m_window.close();
}

void Game::render()
{
	// Clear the back buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the street model
	mat4 streetTransform = rotate(radians(270.f), vec3{0,1,0}) * scale(vec3{-0.5f, 0.5f, 0.5f});
	m_streetModel.draw(streetTransform, m_camera.view(), m_viewport.projection());

	// Draw the lamp model
	mat4 lampTransform1 = translate(vec3{-1.5f, 0, -5}) * rotate(radians(90.f), vec3{0,1,0}) * scale(vec3(1, 2, 1));
	mat4 lampTransform2 = translate(vec3{-7.5f, 0, -5}) * rotate(radians(90.f), vec3{0,1,0}) * scale(vec3(1, 2, 1));
	mat4 lampTransform3 = translate(vec3{-13, 0, -5}) * rotate(radians(90.f), vec3{0,1,0}) * scale(vec3(1, 2, 1));
	m_lampModel.draw(lampTransform1, m_camera.view(), m_viewport.projection());
	m_lampModel.draw(lampTransform2, m_camera.view(), m_viewport.projection());
	m_lampModel.draw(lampTransform3, m_camera.view(), m_viewport.projection());

	// Draw the house model
	mat4 houseTransform = translate(vec3{3.25f, 0, -10}) * rotate(radians(180.f), vec3{0,1,0}) * scale(vec3(0.05f));
	m_houseModel.draw(houseTransform, m_camera.view(), m_viewport.projection());

	// Draw the building model 1
	mat4 buildingTransform1 = translate(vec3{-3.25f, 0, -11});
	m_buildingModel1.draw(buildingTransform1, m_camera.view(), m_viewport.projection());

	// Draw the building model 2
	mat4 buildingTransform2 = translate(vec3{-13.25f, 0, -13.25f});
	m_buildingModel2.draw(buildingTransform2, m_camera.view(), m_viewport.projection());

	// Draw the building model 3
	mat4 buildingTransform3 = translate(vec3{22.5f, 0, -9});
	mat4 buildingTransform4 = translate(vec3{22.5f, 0, -27});
	m_buildingModel3.draw(buildingTransform3, m_camera.view(), m_viewport.projection());
	m_buildingModel3.draw(buildingTransform4, m_camera.view(), m_viewport.projection());

	// Swap front and back buffers
	m_window.swapBuffers();
}

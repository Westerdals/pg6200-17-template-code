#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <memory>
#include <map>

#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>

#include "Timer.h"
#include "GLUtils/GLUtils.hpp"
#include "GLUtils/CubeMap.hpp"
#include "Model.h"
#include "VirtualTrackball.h"
#include "ScreenshotFBO.h"

/**
 * This class handles the game logic and display.
 * Uses SDL as the display manager, and glm for 
 * vector and matrix computations
 */
class GameManager {
public:

	/**
	 * Constructor
	 */
	GameManager();

	/**
	 * Destructor
	 */
	~GameManager();

	/**
	 * Initializes the game, including the OpenGL context
	 * and data required
	 */
	void init();

	/**
	 * The main loop of the game. Runs the SDL main loop
	 */
	void play();

	/**
	 * Quit function
	 */
	void quit();

	/**
	 * Function that handles rendering into the OpenGL context
	 */
	void render();

protected:
	/**
	 * Creates the OpenGL context using SDL
	 */
	void createOpenGLContext();

	/*
	*
	*/
	void initSDL();

	/**
	 * Sets states for OpenGL that we want to keep persistent
	 * throughout the game
	 */
	void setOpenGLStates();

	/**
	 * Creates the matrices for the OpenGL transformations,
	 * perspective, etc.
	 */
	void createMatrices();

	/**
	 * Compiles, attaches, links, and sets uniforms for
	 * a simple OpenGL program
	 */
	void createSimpleProgram();

	/**
	 * Creates vertex array objects
	 */
	void createVAO();

	static const unsigned int window_width = 800;
	static const unsigned int window_height = 600;

	static const float cube_vertices_data[];
	static const float cube_normals_data[];

	float near_plane;
	float far_plane;
	float fovy;

	bool showDebugView;

	int screenshot_number;

private:
	enum RenderMode {
		RENDERMODE_PHONG,
		RENDERMODE_WIREFRAME,
		RENDERMODE_HIDDEN_LINE,
		RENDERMODE_FLAT,
	};

	void zoomIn();
	void zoomOut();
	void GameManager::initDebugView();
	void GameManager::renderDebugView();

	void (GameManager::*render_model)(); // TODO
	static void renderMeshRecursive(MeshPart& mesh, const std::shared_ptr<GLUtils::Program>& program, const glm::mat4& modelview, const glm::mat4& transform, 
		glm::mat4& projection_matrix, glm::vec3(light_pos));
	void GameManager::renderCubeMap(glm::mat4 view);

	void GameManager::screenshot();

	SDL_Window* main_window; //< Our window handle
	SDL_GLContext main_context; //< Our opengl context handle 
	RenderMode render_mode; //< The current method of rendering

	// vao arrays like this is handy for one "scene"
	GLuint main_scene_vao[2]; //< number of different "collection" of vbo's we have
	// Different scenes can be structured with different vaos
	GLuint debugview_vao;

	std::map<std::string, std::shared_ptr<Model>> models;
	std::map<std::string, std::shared_ptr<GLUtils::Program>> shaders;

	std::shared_ptr<GLUtils::CubeMap> diffuse_cubemap;
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER> > cube_vertices, cube_normals;

	// we make the quad vbo without help from program.hpp
	// this is just like the code for triangle primitives in lab_01_solution
	// and that you will find inside program.hpp now
	GLuint debugview;

	// FBO screenshot
	std::shared_ptr<ScreenshotFBO> screenshot_fbo;

	float zoom;
	Timer fps_timer;
	VirtualTrackball cam_trackball;

	struct {
		glm::vec3 position;
		glm::mat4 projection;
		glm::mat4 view;
	} light;

	struct {
		glm::mat4 projection;
		glm::mat4 view;
	} camera;

	std::shared_ptr<Model> model;
	std::shared_ptr<GLUtils::Program> program, cube_program, debugview_program;
	glm::mat4 model_matrix; // TODO should be in a struct with the model mesh
};

#endif // _GAMEMANAGER_H_

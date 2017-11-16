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
#include "GLUtils/ShadowProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/detail/_vectorize.hpp>
#include <glm/detail/_vectorize.hpp>
#include <glm/detail/_vectorize.hpp>
#include "GLUtils/DebugOutput.h"

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

	void printDebug();
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
	 * a simple OpenGL blinn_phong_program
	 */
	void createSimpleProgram();

	/**
	 * Creates vertex array objects
	 */
	void createVAO();
	void init_shadowFBO();
	void render_to_shadow_fbo();
	void render_cubemap_depth(const mat4& view);
	void render_bunny_shadow_recursive(MeshPart& mesh,
							   const std::shared_ptr<Program>& program,
							   const mat4& view_matrix,
							   const mat4& model_matrix,
							   mat4& projection_matrix) const;

	static const unsigned int window_width = 800;
	static const unsigned int window_height = 600;

	static const float cube_vertices_data[];
	static const float cube_normals_data[];

	float near_plane;
	float far_plane;
	float fovy;

	bool showDebugView;

	int screenshot_number;
	GLuint depth_texture;
	GLuint depth_fbo;
	int depth_fbo_width;
	int depth_fbo_height;
	glm::mat4 scale_bias_matrix;
	mat4 cube_depthMVP;
	mat4 bunny_depthMVP;

private:
	struct POV_entity{
		glm::vec3 position;
		glm::mat4 projection;
		glm::mat4 view;
	};

	enum MeshIndex{
		BUNNY = 0,
		CUBE
	};
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
	void renderMeshRecursive(
		MeshPart& mesh,
		const std::shared_ptr<Program>& program,
		const mat4& view_matrix,
		const mat4& model_matrix,
		const mat4& shadow_matrix,
		mat4& projection_matrix,
		glm::vec3 light_position
	);

	void render_bunny_depth(MeshPart& mesh,
							   const std::shared_ptr<Program>& program,
							   const mat4& view_matrix,
							   const mat4& model_matrix,
							   const mat4& projection_matrix);

	void GameManager::renderCubeMap(const glm::mat4& view, const glm::mat4& projection, const mat4& shadow_matrix);
//	void GameManager::renderCubeMap(const ::GameManager::POV_entity& view, const mat4& shadow_matrix);

	void GameManager::screenshot();

	SDL_Window* main_window; //< Our window handle
	SDL_GLContext main_context; //< Our opengl context handle 
	RenderMode render_mode_enum = RENDERMODE_FLAT; //< The current method of rendering

	// vao arrays like this is handy for one "scene"
	GLuint main_scene_vao[2]; // number of different "collection" of vbo's we have
	// Different scenes can be structured with different vaos
	GLuint debugview_vao;

	std::map<std::string, std::shared_ptr<Model>> models;
	std::map<std::string, std::shared_ptr<GLUtils::Program>> shaders;

	std::shared_ptr<GLUtils::CubeMap> diffuse_cubemap;
	std::shared_ptr<GLUtils::VBO<GL_ARRAY_BUFFER> > cube_vertices, cube_normals;

	// we make the quad vbo without help from Program.hpp
	// this is just like the code for triangle primitives in lab_01_solution
	// and that you will find inside Program.hpp now
	GLuint debugview;

	// FBO screenshot
	std::shared_ptr<ScreenshotFBO> screenshot_fbo;

	float zoom;
	Timer fps_timer;
	VirtualTrackball cam_trackball;

	

	POV_entity camera, light, current_POV;

	std::shared_ptr<Model> bunny_model, cube_model;
	std::shared_ptr<GLUtils::Program> cube_program, debugview_program;
	std::shared_ptr<GLUtils::ShadowProgram> shadow_program;

	glm::mat4 bunny_model_matrix; // TODO should be in a struct with the bunny_model mesh
	glm::mat4 cube_model_matrix;

};
 
#endif // _GAMEMANAGER_H_

#include "GameManager.h"
#include "GeometryManager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include <stdexcept>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>

using std::cerr;
using std::endl;
using GLUtils::VBO;
using GLUtils::Program;
using GLUtils::readFile;

const float GameManager::cube_vertices_data[] = {
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,

	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,

	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
};

const float GameManager::cube_normals_data[] = {
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,
	0.0f, 0.0f, -1.0f,

	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

void GameManager::write_coordinates(std::stringstream& ss, const float* coords, const int length,
                                    const std::string prefix, const int step){
	std::cout << "length of '" << prefix << "' coords: " << length << '\n';

	for (int i = 0; i < length; i += step){
		ss << prefix << " ";
		for (int j = 0; j < step; j++){
			ss << coords[ i + j ] << " ";
		}
		ss << '\n';
	}
}

void GameManager::makeCubeModel(){
	std::stringstream file_content;
	std::cout << "size of '" << "v" << "' coords: " << sizeof(cube_vertices_data) << '\n';
	const int v_length = sizeof(cube_vertices_data) / sizeof(cube_vertices_data[0]);
	write_coordinates(file_content, cube_vertices_data, v_length, "v", 3);
	file_content << '\n';
	const int vn_length = sizeof(cube_vertices_data) / sizeof(cube_vertices_data[0]);

	write_coordinates(file_content, cube_normals_data, vn_length, "vn", 3);

	std::ofstream ofs;
	ofs.open("models/cube.obj", std::ofstream::out | std::ofstream::trunc);
	const std::string content_str = file_content.str();
	ofs << content_str;
	ofs.close();
}

GameManager::GameManager(){
	fps_timer.restart();
	showDebugView = false;
	zoom = 1;
	near_plane = 0.5f;
	far_plane = 30.0f;
	fovy = 45.0f;
	makeCubeModel();
}


GameManager::~GameManager(){}

void GameManager::init(){
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		std::stringstream err;
		err << "Could not initialize SDL: " << SDL_GetError();
		THROW_EXCEPTION(err.str());
	}
	atexit(SDL_Quit);

	ilInit();
	iluInit();

	createOpenGLContext();
	setOpenGLStates();
	createMatrices();
	createSimpleProgram();
	createVAO();
	init_shadowFBO();
}

void GameManager::createOpenGLContext(){
	//Set OpenGL major an minor versions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Use double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Use framebuffer with 16 bit depth buffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Use framebuffer with 8 bit for red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); // Use framebuffer with 8 bit for green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); // Use framebuffer with 8 bit for blue
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // Use framebuffer with 8 bit for alpha
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// Initalize video
	main_window = SDL_CreateWindow("Westerdals - PG6200 Reworked Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                               window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!main_window){
		THROW_EXCEPTION("SDL_CreateWindow failed");
	}

	//Create OpenGL context
	main_context = SDL_GL_CreateContext(main_window);

	// Init glew
	// glewExperimental is required in openGL 3.3 
	// to create forward compatible contexts 
	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK){
		std::stringstream err;
		err << "Error initializing GLEW: " << glewGetErrorString(glewErr);
		THROW_EXCEPTION(err.str());
	}

	// Unfortunately glewInit generates an OpenGL error, but does what it's
	// supposed to (setting function pointers for core functionality).
	// Lets do the ugly thing of swallowing the error....
	glGetError();

	cam_trackball.setWindowSize(window_width, window_height);
}

void GameManager::setOpenGLStates(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.5, 1.0);
	glViewport(0, 0, window_width, window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameManager::createMatrices(){
	bunny_model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(3));
	cube_model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(far_plane * 0.75f));

	camera.projection = glm::perspective(fovy / zoom, window_width / (float)window_height, near_plane, far_plane);
	camera.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));

	light.position = glm::vec3(10, 0, 0);
	light.projection = glm::perspective(glm::half_pi<float>(), 1.0f, near_plane, far_plane);
	light.view = glm::lookAt(light.position, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
}

void GameManager::createSimpleProgram(){

	std::string fs_src;
	std::string vs_src;


	fs_src = readFile("shaders/fbo.frag");
	vs_src = readFile("shaders/fbo.vert");

	//	debugview_program.reset(new Program(vs_src, fs_src));
	debugview_program = std::make_shared<Program>(vs_src, fs_src);

	fs_src = readFile("shaders/cube_map.frag");
	vs_src = readFile("shaders/cube_map.vert");
	// For this one we will also add a geometry shader!
	// we will need to do some primitive offsetting when we add shadows
	std::string gs_src = readFile("shaders/cube_map.geom");

	// alternativly to a separate variable to shader collections we could organize them into a map
	//shaders.insert(std::make_pair("cube_shaders", new Program(vs_src, fs_src)));
	cube_program.reset(new Program(vs_src, gs_src, fs_src));

	cube_program->use();
	diffuse_cubemap.reset(new GLUtils::CubeMap("cubemaps/diffuse/", "jpg"));
	glProgramUniform1i(cube_program->name, cube_program->getUniform("cubemap"), 0);
	cube_program->disuse();

	//Compile shaders, attach to blinn_phong_program object, and link
	// blinn_phong_program.reset(new Program(readFile("shaders/basic_phong.vert"), readFile("shaders/basic_phong.frag")));

	/// create shadow program 
	fs_src = GLUtils::readFile("shaders/shadow.frag");
	vs_src = GLUtils::readFile("shaders/shadow.vert");
	shadow_program.reset(new Program(vs_src, fs_src));
	shadow_program->use();
	shadow_program->disuse();

}

void GameManager::createVAO(){
	// We wan two VAO pointers, we tell OpenGL where it can start counting (to two)
	// look inside the header to alter the size of our vao array.
	glGenVertexArrays(2, &main_scene_vao[0]);
	glBindVertexArray(main_scene_vao[0]);
	CHECK_GL_ERROR();

	// Seperate VBOs
	bunny_model.reset(new Model("models/bunny.obj", false));

	bunny_model->getVertices()->bind();
	cube_program->setAttributePointer("position", 3);
	CHECK_GL_ERROR();
	shadow_program->setAttributePointer("position", 3);
	CHECK_GL_ERROR();

	bunny_model->getNormals()->bind();
	cube_program->setAttributePointer("normal", 3);
	CHECK_GL_ERROR();


	// Setting up cube VBO data with its own VAO reference
	glBindVertexArray(main_scene_vao[1]);
	cube_vertices.reset(new VBO<GL_ARRAY_BUFFER>(cube_vertices_data, sizeof(cube_vertices_data)));
	cube_normals.reset(new VBO<GL_ARRAY_BUFFER>(cube_normals_data, sizeof(cube_normals_data)));
	//	cube_model.reset(new Model("models/cube.obj"), false);

	cube_vertices->bind();
	//	cube_model->getVertices()->bind();
	cube_program->setAttributePointer("position", 3);
	CHECK_GL_ERROR();

	cube_normals->bind();
	//	cube_model->getNormals()->bind();
	cube_program->setAttributePointer("normal", 3);
	CHECK_GL_ERROR();

	bunny_model->getVertices()->unbind(); //Unbinds both vertices and normals
	//	cube_model->getNormals()->unbind();
	CHECK_GL_ERROR();

	glBindVertexArray(0);

	initDebugView();
	screenshot_fbo.reset(new ScreenshotFBO(1024, 1024));

	// Interleaved VBOs
	/*
	GLint k = 9 * sizeof(float); // stride size

	bunny_model->getVertices()->bind();
	blinn_phong_program->setAttributePointer("position", 3, GL_FLOAT, GL_FALSE, k, 0);
	CHECK_GL_ERROR();

	blinn_phong_program->setAttributePointer("normal", 3, GL_FLOAT, GL_FALSE, k, reinterpret_cast<void *>(3 * sizeof(float)));
	CHECK_GL_ERROR();
	*/
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void GameManager::init_shadowFBO(){
	int LoD = 0;
	glGenFramebuffers(1, &depth_fbo);

	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);

	// allocate storage
	glTexImage2D(GL_TEXTURE_2D, LoD, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
	             nullptr);

	// filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*// depth comparison mode: less or equal will be shown;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
*/
	// wrapping
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// Now the Frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);

	// Pass the depth texture in
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);
	// no color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	auto fboLoaded = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboLoaded != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "FrameBuffer not loaded correctly" << std::endl;
	}
}

void GameManager::shadow_map_pass(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, depth_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	// we offset the view by the light's position
	glm::mat4 light_as_camera_view = glm::translate(light.view, -light.position);

	// render bunny
	glBindVertexArray(main_scene_vao[0]);
	bunny_model->render_for_shadow(shadow_program, light_as_camera_view, bunny_model_matrix, light.projection);
	// render cube
	glBindVertexArray(main_scene_vao[1]);
	render_cube_map_shadows(light_as_camera_view);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameManager::render_cube_map_shadows(const glm::mat4 view) const{
	mat4 light_mvp = light.projection * view * cube_model_matrix;

	shadow_program->use();
	const auto loc = shadow_program->getUniform("light_transform");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(light_mvp));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shadow_program->disuse();
}

void GameManager::shadow_render(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLuint shadow_map_loc = shadow_program->getUniform("shadow_map");
	glUniform1i(shadow_map_loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	//...
}


void GameManager::initDebugView(){
	glGenVertexArrays(1, &debugview_vao);

	glBindVertexArray(debugview_vao);

	// an example of quads instead of triangles
	static float positions[8] = {
		-1.0, 1.0,
		-1.0, -1.0,
		1.0, 1.0,
		1.0, -1.0
	};

	glGenBuffers(1, &debugview);
	glBindBuffer(GL_ARRAY_BUFFER, debugview);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &positions[0], GL_STATIC_DRAW);

	debugview_program->setAttributePointer("position", 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void GameManager::renderDebugView(){
	glViewport(0, 0, window_width, window_height);
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(debugview_vao);
	debugview_program->use();

	glUniform1i(debugview_program->getUniform("texture"), 0);
	glBindTexture(GL_TEXTURE_2D, screenshot_fbo->getTexture());

	// this is independent of the transformations to the world
	// we are talking about window space
	glm::mat3 transform = glm::mat3(glm::vec3(0.5, 0.0, 0.0), glm::vec3(0.0, 0.5, 0.0), glm::vec3(-0.5, -0.5, 0.5));

	glProgramUniformMatrix3fv(debugview_program->name, debugview_program->getUniform("transform"), 1, 0,
	                          glm::value_ptr(transform));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	debugview_program->disuse();
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}

void GameManager::renderCubeMap(glm::mat4 view){

	cube_program->use();

	diffuse_cubemap->bindTexture(GL_TEXTURE0);

	glBindVertexArray(main_scene_vao[1]);

	glm::mat4 model_view_mat = view * cube_model_matrix;

	glm::mat4 model_mat_inverse = glm::inverse(cube_model_matrix);
	glm::mat4 model_view_mat_inverse = glm::inverse(model_view_mat);
	glm::mat3 normal_mat = glm::transpose(glm::mat3(model_view_mat_inverse));

	glm::vec3 light_pos = glm::mat3(model_mat_inverse) * light.position / model_mat_inverse[3].w;
	glm::vec3 camera_pos = glm::vec3(model_view_mat_inverse[3] / model_view_mat_inverse[3].w);


	glUniform3fv(cube_program->getUniform("colour"), 1, glm::value_ptr(glm::vec3(1.0f, 0.8f, 0.8f)));

	glUniform3fv(cube_program->getUniform("light_position"), 1, glm::value_ptr(light_pos));
	glUniform3fv(cube_program->getUniform("camera_position"), 1, glm::value_ptr(camera_pos));

	glUniformMatrix4fv(cube_program->getUniform("model_view_mat"), 1, 0, glm::value_ptr(model_view_mat));
	glUniformMatrix4fv(cube_program->getUniform("proj_mat"), 1, 0, glm::value_ptr(camera.projection));
	glUniformMatrix3fv(cube_program->getUniform("normal_mat"), 1, 0, glm::value_ptr(normal_mat));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GameManager::render(){
	float elapsed = fps_timer.elapsedAndRestart();

	//	glm::mat4 rotation = glm::rotate(elapsed*2.f, glm::vec3( 0.0f, 1.0f, 0.0f));
	glm::mat4 rotation = glm::rotate(.002f, glm::vec3(0.0f, 1.0f, 0.0f));
	light.position = glm::mat3(rotation) * light.position;
	light.view = glm::lookAt(light.position, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 view = camera.view * cam_trackball.getTransform();

	if (showDebugView){
		// Render to FBO and set the viewport to cover the pixels in the FBO texture
		screenshot_fbo->bind();
		glViewport(0, 0, screenshot_fbo->getWidth(), screenshot_fbo->getHeight());
	}
	else{
		// Default: to window rendering
		glViewport(0, 0, window_width, window_height);
		glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	}

	//	shadow_map_pass();
	//	shadow_render();

	//Clear screen, and set the correct blinn_phong_program
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderCubeMap(view);


	//Render geometry
	glBindVertexArray(main_scene_vao[0]);
	switch (render_mode_enum){
//		case RENDERMODE_WIREFRAME:
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			glUniform1i(cube_program->getUniform("lighting"), 0);
//			break;
		case RENDERMODE_HIDDEN_LINE:
			//first, render filled polygons with an offset in negative z-direction
			glCullFace(GL_BACK);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.1f, 4.0f);
			//Render geometry to be offset here
			bunny_model->renderMeshRecursive(cube_program, view, bunny_model_matrix, camera.projection, light.position);
			glDisable(GL_POLYGON_OFFSET_FILL);

			//then, render wireframe, without lighting
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glUniform1i(cube_program->getUniform("lighting"), 0);
			break;
		case RENDERMODE_FLAT:
			// TODO
			break;
//		case RENDERMODE_PHONG:
//			glCullFace(GL_BACK);
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//			break;
		default:
			THROW_EXCEPTION("Rendermode not supported");
			break;
	}

	bunny_model->renderMeshRecursive(cube_program, view, bunny_model_matrix, camera.projection, light.position);
//	renderMeshRecursive(bunny_model->getMesh(), cube_program, view, bunny_model_matrix, camera.projection, light.position);

	if (showDebugView)
		renderDebugView();

	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void GameManager::zoomIn(){
	zoom *= 1.1f;
	camera.projection = glm::perspective(fovy / zoom,
	                                     window_width / (float)window_height, near_plane, far_plane);
}

void GameManager::zoomOut(){
	zoom = max(zoom*0.9f, 0.5f);
	camera.projection = glm::perspective(fovy / zoom,
	                                     window_width / (float)window_height, near_plane, far_plane);
}

void GameManager::play(){
	bool doExit = false;
	//SDL main loop
	while (!doExit){
		SDL_Event event;
		while (SDL_PollEvent(&event)){
			// poll for pending events
			switch (event.type){
				case SDL_MOUSEBUTTONDOWN:
					cam_trackball.rotateBegin(event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEBUTTONUP:
					cam_trackball.rotateEnd(event.motion.x, event.motion.y);
					break;
				case SDL_MOUSEMOTION:
					cam_trackball.rotate(event.motion.x, event.motion.y, zoom);
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym){
						case SDLK_ESCAPE:
							doExit = true;
							break;
						case SDLK_q:
							if (event.key.keysym.mod & KMOD_CTRL) doExit = true; //Ctrl+q
							break;
						case SDLK_m:
							showDebugView = !showDebugView;
							break;
						case SDLK_p:
							screenshot();
							break;
						case SDLK_RIGHT:
							camera.view = glm::translate(camera.view, glm::vec3(-0.1, 0.0, 0.0));
							break;
						case SDLK_LEFT:
							camera.view = glm::translate(camera.view, glm::vec3(0.1, 0.0, 0.0));
							break;
						case SDLK_UP:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, -0.1, 0.0));
							break;
						case SDLK_DOWN:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, 0.1, 0.0));
							break;
						case SDLK_w:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, 0.0, 0.1));
							break;
						case SDLK_s:
							camera.view = glm::translate(camera.view, glm::vec3(0.0, 0.0, -0.1));
							break;
						case SDLK_PLUS:
							zoomIn();
							break;
						case SDLK_MINUS:
							zoomOut();
							break;
						case SDLK_1:
							render_mode_enum = RENDERMODE_FLAT;
							break;
						case SDLK_2:
							render_mode_enum = RENDERMODE_PHONG;
							break;
						case SDLK_3:
							render_mode_enum = RENDERMODE_WIREFRAME;
							break;
						case SDLK_4:
							render_mode_enum = RENDERMODE_HIDDEN_LINE;
							break;
					}
					break;
				case SDL_QUIT: //e.g., user clicks the upper right x
					doExit = true;
					break;
			}
		}

		//Render, and swap front and back buffers
		render();
		SDL_GL_SwapWindow(main_window);
	}
	quit();
}

void GameManager::quit(){
	std::cout << "Bye bye..." << std::endl;
}

void GameManager::screenshot(){
	// set a unique filename for each new screenshot (only unique per application run!)
	std::stringstream filename_stream;
	filename_stream << "screenshot_" << screenshot_number << ".png";
	std::string filename = filename_stream.str();
	++screenshot_number;

	int width = screenshot_fbo->getWidth();
	int height = screenshot_fbo->getHeight();

	// need to store the data on the CPU before writing to file
	std::vector<unsigned char> screenshot_data;
	screenshot_data.resize(width * height * 4);

	// read pixels from FBO
	screenshot_fbo->bind();
	glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, (void*)screenshot_data.data());
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	CHECK_GL_ERROR();

	// write to file using DevIL
	ilEnable(IL_FILE_OVERWRITE);
	ILuint ImageName;
	ilGenImages(1, &ImageName); // Grab a new image name.
	ilBindImage(ImageName);

	if (!ilTexImage(width, height, 1, 4, GL_BGRA, GL_UNSIGNED_BYTE, (void*)screenshot_data.data())){
		ILenum e;
		std::stringstream error;
		while ((e = ilGetError()) != IL_NO_ERROR){
			error << e << ": " << iluErrorString(e) << std::endl;
		}
		ilDeleteImages(1, &ImageName); // Delete the image name. 
		throw std::runtime_error(error.str());
	}

	ilSave(IL_PNG, filename.c_str());
	ilDeleteImages(1, &ImageName); // Delete the image name. 

	ilDisable(IL_FILE_OVERWRITE);
	std::cout << "Saved screenshot!" << std::endl;
}

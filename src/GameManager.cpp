#include "GameManager.h"
#include "GeometryManager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include <stdexcept>


using std::cerr;
using std::endl;
using GLUtils::VBO;
using GLUtils::Program;
using GLUtils::ShadowProgram;
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

void debug_mat_content(glm::mat4& mat){
	auto l = mat.length();
	std::cout << "[\n";
	for (int i = 0; i < l; i++){
		for (int j = 0; j < l; j++){
			std::cout << mat[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << " ]" << std::endl;
}


GameManager::GameManager(){
	fps_timer.restart();
	showDebugView = false;
	zoom = 1;
	near_plane = 0.5f;
	far_plane = 30.0f;
	fovy = 45.0f;
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
	CHECK_GL_ERROR();
	createVAO();
	CHECK_GL_ERROR();
	init_shadowFBO();
	CHECK_GL_ERROR();

}

void GameManager::printDebug(){
	std::cout << "Fovy / zoom : " << fovy << " / " << zoom << '\n';
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

	//enabling new kind of debugging:
	glEnable(GL_DEBUG_OUTPUT);

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

	// must be called AFTER glewInit() is called
	glDebugMessageCallback((GLDEBUGPROC)GLUtils::DebugOutput::myCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	// Unfortunately glewInit generates an OpenGL error, but does what it's
	// supposed to (setting function pointers for core functionality).
	// Lets do the ugly thing of swallowing the error....
	glGetError();

	cam_trackball.setWindowSize(window_width, window_height);
}

void GameManager::setOpenGLStates(){

	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
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

	scale_bias_matrix = glm::scale(glm::translate(mat4(1.0f), glm::vec3(0.5f)), glm::vec3(0.5f));
	debug_mat_content(scale_bias_matrix);
}

void GameManager::createSimpleProgram(){

	/// create shadow program 
	shadow_program.reset(new ShadowProgram());
	shadow_program->init();


	std::string fs_src = readFile("shaders/cube_map.frag");
	std::string vs_src = readFile("shaders/cube_map.vert");
	std::string gs_src = readFile("shaders/cube_map.geom");

	cube_program.reset(new Program(vs_src, gs_src, fs_src));
	cube_program->use();
	diffuse_cubemap.reset(new GLUtils::CubeMap("cubemaps/diffuse/", "jpg"));
	glProgramUniform1i(cube_program->name, cube_program->getUniform("cubemap"), 1);
	cube_program->disuse();

}

void GameManager::createVAO(){
	glGenVertexArrays(2, main_scene_vao);
	glBindVertexArray(main_scene_vao[BUNNY]);
	CHECK_GL_ERROR();

	// under the hood, the bunny_model constructor creates a VBO 
	// (actual storage in OpenGL) for the various data concerning the bunny, 
	// depending on the WaveFront object (.obj) file structure.
	// In this case, a VBO will be made to load vertices and face normals
	// and those will be connected to main_scene_vao[BUNNY], since this is the one
	// that was last bound with glBindVertexArray
	bunny_model.reset(new Model("models/bunny.obj", false));

	bunny_model->getVertices()->bind();
	shadow_program->setAttributePointer("position", 3);
	CHECK_GL_ERROR();
	cube_program->setAttributePointer("position", 3);
	CHECK_GL_ERROR();


	bunny_model->getNormals()->bind();
	cube_program->setAttributePointer("normal", 3);
	CHECK_GL_ERROR();


	// Setting up cube VBO data with its own VAO reference
	glBindVertexArray(main_scene_vao[CUBE]);
	cube_vertices.reset(new VBO<GL_ARRAY_BUFFER>(cube_vertices_data, sizeof(cube_vertices_data)));
	cube_normals.reset(new VBO<GL_ARRAY_BUFFER>(cube_normals_data, sizeof(cube_normals_data)));

	cube_vertices->bind();
	cube_program->setAttributePointer("position", 3);
	CHECK_GL_ERROR();
	shadow_program->setAttributePointer("position", 3);
	CHECK_GL_ERROR();

	cube_normals->bind();
	cube_program->setAttributePointer("normal", 3);
	CHECK_GL_ERROR();

	bunny_model->getVertices()->unbind(); //Unbinds both vertices and normals
	CHECK_GL_ERROR();

	glBindVertexArray(0);

	//	initDebugView();
	//	screenshot_fbo.reset(new ScreenshotFBO(1024, 1024));

}

void GameManager::init_shadowFBO(){
	int LoD = 0;
	depth_fbo_width = window_width;
	depth_fbo_height = window_height;
	glGenTextures(1, &depth_texture);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	glTexImage2D(GL_TEXTURE_2D, LoD, GL_DEPTH_COMPONENT, depth_fbo_width, depth_fbo_height, 0, GL_DEPTH_COMPONENT,
	             GL_FLOAT,
	             nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// Now the Frame buffer
	glGenFramebuffers(1, &depth_fbo);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameManager::initRenderToShadowFBO(){
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
	glViewport(0, 0, depth_fbo_width, depth_fbo_height);
	CHECK_GL_ERROR();
	//		glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	CHECK_GL_ERROR();
}

void GameManager::render_cubemap_depth(const glm::mat4& view){

	shadow_program->use();
	const auto loc = shadow_program->getUniform("model_view_projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(cube_depthMVP));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shadow_program->disuse();
}

void GameManager::render_bunny_depth(MeshPart& mesh,
                                     const std::shared_ptr<Program>& program,
                                     const mat4& view_matrix,
                                     const mat4& model_matrix,
                                     const mat4& projection_matrix){

	/*const mat4 meshpart_model_matrix = model_matrix * mesh.transform;
	mat4 light_mvp = projection_matrix * view_matrix * meshpart_model_matrix;*/
	const mat4 meshpart_model_matrix = model_matrix * mesh.transform;
	mat4 light_mvp = mesh.transform * projection_matrix * view_matrix * model_matrix;

	program->use();
	const auto loc = program->getUniform("model_view_projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(light_mvp));
	CHECK_GL_ERRORS();
	glDrawArrays(GL_TRIANGLES, mesh.first, mesh.count);
	CHECK_GL_ERRORS();
	for (int i = 0; i < (int)mesh.children.size(); ++i){
		render_bunny_depth(mesh.children.at(i), program, view_matrix, meshpart_model_matrix, projection_matrix);
	}
	program->disuse();
}


void GameManager::renderCubeMap(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& shadow_MVP){

	glBindVertexArray(main_scene_vao[CUBE]);
	CHECK_GL_ERROR();

	glm::vec3 colour = glm::vec3(1.0f, 0.8f, 0.8f);
	glm::mat4 model_view_mat = view * cube_model_matrix;

	glm::mat4 model_mat_inverse = glm::inverse(cube_model_matrix);
	glm::mat4 model_view_mat_inverse = glm::inverse(model_view_mat);

	glm::vec3 light_pos = glm::mat3(model_mat_inverse) * light.position / model_mat_inverse[3].w;
	glm::vec3 camera_pos = glm::vec3(model_view_mat_inverse[3] / model_view_mat_inverse[3].w);


	cube_program->use();

	glUniform3fv(cube_program->getUniform("colour"), 1, glm::value_ptr(colour));
	glUniform3fv(cube_program->getUniform("light_position"), 1, glm::value_ptr(light_pos));
	glUniform3fv(cube_program->getUniform("camera_position"), 1, glm::value_ptr(camera_pos));

	glUniformMatrix4fv(cube_program->getUniform("model_view_mat"), 1, 0, glm::value_ptr(model_view_mat));
	glUniformMatrix4fv(cube_program->getUniform("proj_mat"), 1, 0, glm::value_ptr(projection));
	CHECK_GL_ERROR();

	glUniformMatrix4fv(cube_program->getUniform("shadow_MVP"), 1, 0, glm::value_ptr(shadow_MVP));
	CHECK_GL_ERROR();

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	cube_program->disuse();
	CHECK_GL_ERROR();

}

void GameManager::renderMeshRecursive(
	MeshPart& mesh,
	const std::shared_ptr<Program>& program,
	const mat4& view_matrix,
	const mat4& model_matrix,
	const mat4& shadow_MVP,
	mat4& projection_matrix,
	glm::vec3 light_position
){
	//Create modelview matrix
	const mat4 meshpart_model_matrix = model_matrix * mesh.transform;
	mat4 model_view_mat = view_matrix * meshpart_model_matrix;
	mat4 model_mat_inverse = inverse(meshpart_model_matrix);
	mat4 model_view_mat_inverse = inverse(model_view_mat);

	glm::vec3 light_pos = glm::mat3(model_mat_inverse) * light_position / model_mat_inverse[3].w;
	glm::vec3 camera_pos = glm::vec3(model_view_mat_inverse[3] / model_view_mat_inverse[3].w);


	program->use();

	glUniformMatrix4fv(program->getUniform("shadow_MVP"), 1, 0, glm::value_ptr(shadow_MVP));
	glUniformMatrix4fv(program->getUniform("model_view_mat"), 1, 0, value_ptr(model_view_mat));
	glUniformMatrix4fv(program->getUniform("proj_mat"), 1, 0, glm::value_ptr(projection_matrix));
	CHECK_GL_ERROR();

	glUniform3fv(program->getUniform("colour"), 1, value_ptr(glm::vec3(.0f, 1.f, .8f)));
	glUniform3fv(program->getUniform("light_position"), 1, value_ptr(light_pos));
	glUniform3fv(program->getUniform("camera_position"), 1, value_ptr(camera_pos));
	CHECK_GL_ERROR();

	glDrawArrays(GL_TRIANGLES, mesh.first, mesh.count);
	CHECK_GL_ERROR();
	for (int i = 0; i < (int)mesh.children.size(); ++i)
		renderMeshRecursive(mesh.children.at(i), program, view_matrix, meshpart_model_matrix, shadow_MVP,
		                    projection_matrix,
		                    light_position);

	program->disuse();
}

void GameManager::render_bunny_shadow_recursive(MeshPart& mesh,
                                                const std::shared_ptr<Program>& program,
                                                const mat4& view_matrix,
                                                const mat4& model_matrix,
                                                mat4& projection_matrix) const{

	const mat4 meshpart_model_matrix = model_matrix * mesh.transform;
	mat4 light_mvp = projection_matrix * view_matrix * meshpart_model_matrix;

	program->use();
	const auto loc = program->getUniform("transform");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(light_mvp));

	glDrawArrays(GL_TRIANGLES, mesh.first, mesh.count);
	for (int i = 0; i < (int)mesh.children.size(); ++i){
		render_bunny_shadow_recursive(mesh.children.at(i), program, view_matrix, meshpart_model_matrix, projection_matrix);
	}
	program->disuse();
}

void GameManager::render(){
	const float elapsed = fps_timer.elapsedAndRestart();

	glm::mat4 rotation = glm::rotate(elapsed * 1.f, glm::vec3(0.0f, 1.0f, 0.0f));
	light.position = glm::mat3(rotation) * light.position;
	light.view = glm::lookAt(light.position, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 view = camera.view * cam_trackball.getTransform();

	cube_depthMVP = light.projection * light.view * cube_model_matrix;
	bunny_depthMVP = light.projection * light.view * bunny_model_matrix;

	glm::mat4 cube_depthBiasMVP = scale_bias_matrix * cube_depthMVP;
	glm::mat4 bunny_depthBiasMVP = scale_bias_matrix * bunny_depthMVP;


	///******************* R T T ******************************

	// ******** prepare shadow FBO + "shrink" viewport to the FBO (texture) size + clear depth
	initRenderToShadowFBO();
	// ******** render bunny
	glBindVertexArray(main_scene_vao[BUNNY]);
	render_bunny_depth(bunny_model->getMesh(), shadow_program, light.view, bunny_model_matrix, camera.projection);

	CHECK_GL_ERROR();
	// ******* render cube
//		glBindVertexArray(main_scene_vao[CUBE]);
//		render_cubemap_depth(light.view);
//		CHECK_GL_ERROR();

	///******************* FINAL RENDER PASS ************************************
	// ******* prepare actual render-to-screen FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //go back to main framebuffer
	glViewport(0, 0, window_width, window_height); //blow viewport back up to window size
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color + depth

	// ******* render cube with shadows
	cube_program->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depth_texture);
	CHECK_GL_ERROR();
	glUniform1i(cube_program->getUniform("shadow_map"), 0);
	CHECK_GL_ERROR();
	diffuse_cubemap->bindTexture(GL_TEXTURE1);
	CHECK_GL_ERROR();

	renderCubeMap(view, camera.projection, cube_depthBiasMVP);
	CHECK_GL_ERROR();
	glBindVertexArray(main_scene_vao[BUNNY]);

	renderMeshRecursive(bunny_model->getMesh(), cube_program, view, bunny_model_matrix, bunny_depthBiasMVP,
	                    camera.projection, light.position);
	glBindVertexArray(0);
	CHECK_GL_ERROR();
	//	diffuse_cubemap->unbindTexture();


}


void GameManager::zoomIn(){
	//	zoom *= 1.1f;
	zoom += 1e-3f;
	camera.projection = glm::perspective(fovy / zoom,
	                                     window_width / (float)window_height, near_plane, far_plane);
}

void GameManager::zoomOut(){
	//	zoom = max(zoom*0.9f, 0.5f);
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
						case SDLK_d:
							printDebug();
							break;
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


void GameManager::initDebugView(){
	glGenVertexArrays(1, &debugview_vao); // create an array that represents the whole debugview object

	glBindVertexArray(debugview_vao); // enable it

	// an example of quads instead of triangles
	static float positions[8] = {
		-1.0, 1.0,
		-1.0, -1.0,
		1.0, 1.0,
		1.0, -1.0
	};

	glGenBuffers(1, &debugview);
	// create a name in OpenGL for the position coords and save it locally in "GLuint debugview"
	glBindBuffer(GL_ARRAY_BUFFER, debugview); // activate a storage for that name
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &positions[0], GL_STATIC_DRAW); // fill the storage with data

	debugview_program->setAttributePointer("position", 2);
	// link the data to a variable in the shader program

	glBindVertexArray(0); // stop modifying the debugview mesh / data
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GameManager::renderDebugView(){
	glViewport(0, 0, window_width, window_height);
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	glBindVertexArray(debugview_vao);
	debugview_program->use();

	glUniform1i(debugview_program->getUniform("texture"), 0);
	glBindTexture(GL_TEXTURE_2D, screenshot_fbo->getTexture());

	// this is independent of the transformations to the world
	// we are talking about window space
	glm::mat3 transform = glm::mat3(
		glm::vec3(0.5, 0.0, 0.0),
		glm::vec3(0.0, 0.5, 0.0),
		glm::vec3(-0.5, -0.5, 0.5));
	// for debug
	transform = glm::mat4(1.f);

	glProgramUniformMatrix3fv(debugview_program->name, debugview_program->getUniform("transform"), 1, 0,
	                          glm::value_ptr(transform));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	debugview_program->disuse();
	glBindVertexArray(0);
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

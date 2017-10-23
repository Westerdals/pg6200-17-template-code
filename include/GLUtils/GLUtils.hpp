#ifndef _GLUTILS_HPP__
#define _GLUTILS_HPP__

#include <cstdlib>
#include <sstream>
#include <vector>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <glm/glm.hpp>

#include <GL/glew.h>


#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define CHECK_GL_ERRORS() GLUtils::checkGLErrors(__FILE__, __LINE__)
#define CHECK_GL_FBO_COMPLETENESS() GLUtils::checkGLFBOCompleteness(__FILE__, __LINE__)

namespace GLUtils {

inline void checkGLErrors(const char* file, unsigned int line) {
	GLenum err = glGetError(); 
    if( err != GL_NO_ERROR ) { 
		std::stringstream log; 
		log << file << '@' << line << ": OpenGL error:" 
             << std::hex << err << " " << gluErrorString(err); 
			 throw std::runtime_error(log.str()); 
    } 
}

inline void checkGLFBOCompleteness(const char* file, unsigned int line) {
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE) {
		std::stringstream log; 
		log << file << '@' << line << ": FBO incomplete error:" 
             << std::hex << err << " " << gluErrorString(err); 
			 throw std::runtime_error(log.str()); 
	}
}

// Just a function I made to help with debuging!
#ifdef _DEBUG
inline std::string mat4ToString(glm::mat4 matrix)
{
	std::stringstream ss;
	ss << std::setfill(' ') << std::setw(2) << matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << " " << matrix[3][0] << "\n"
		<< matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << " " << matrix[3][1] << "\n"
		<< matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << " " << matrix[3][2] << "\n"
		<< matrix[0][3] << " " << matrix[1][3] << " " << matrix[2][3] << " " << matrix[3][3] << "\n";
	return ss.str();
}
#endif

}; //Namespace GLUtils

#include "GLUtils/Program.hpp"
#include "GLUtils/BO.hpp"
#include "GLUtils/CubeMap.hpp"

#endif
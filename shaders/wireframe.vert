#version 150

uniform mat4 wireframe_projection_matrix;

in vec3 position;

void main(){
	gl_Position = wireframe_projection_matrix * vec4(position, 1.0);
}
#version 150
uniform mat4 light_transform;
in vec3 position;

in vec2 in_depth_texture;
out vec2 ex_depth_texture;

void main(){
	gl_Position = light_transform * vec4(position, 1.0f);
	ex_depth_texture = in_depth_texture;
}
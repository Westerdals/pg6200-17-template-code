#version 150

uniform mat4 shadow_light_transform;

in vec3 position;

void main() {
	gl_Position = shadow_light_transform * vec4(position, 1.0);
}

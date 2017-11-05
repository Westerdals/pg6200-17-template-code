#version 150

uniform mat4 mv_mat;
uniform mat4 proj_mat;
uniform mat3 normal_mat;
uniform vec3 camera_position;
uniform vec3 light_position;

in vec3 position;
in vec3 normal;

out vec3 v;
out vec3 l;
out vec3 n;
out vec3 cube_tex_coord;

void main() {
	vec4 pos = mv_mat * vec4(position, 1.f);
	gl_Position = proj_mat * pos;

	v = camera_position;
	n = normal_mat * normal;
	l = light_position - pos.xyz;
	v = -pos.xyz;
	cube_tex_coord = position;
}

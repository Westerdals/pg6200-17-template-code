#version 150

uniform mat4 phong_modelviewprojection_matrix;
uniform mat4 phong_modelview_matrix_inverse;

uniform vec3 phong_light_pos;
uniform mat4 phong_light_transform;

in vec3 position;
in vec3 normal;

out vec4 crd;
out vec3 cube_map_coord;

smooth out vec3 g_v;
smooth out vec3 g_l;
smooth out vec3 g_n;

void main() {	
	float homogeneous_divide = (1.0f/phong_modelview_matrix_inverse[3].w);
	vec3 cam_pos_world = phong_modelview_matrix_inverse[3].xyz*homogeneous_divide;

	g_v = normalize(cam_pos_world - position);
	g_l = normalize(phong_light_pos - position);
	g_n = normalize(normal);

	crd = phong_light_transform * vec4(position, 1.0);

	cube_map_coord = position.xyz;

	gl_Position = phong_modelviewprojection_matrix * vec4(position, 1.0);
}

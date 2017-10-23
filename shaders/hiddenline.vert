#version 150

uniform mat4 hiddenline_projection_matrix;
uniform mat4 hiddenline_modelview_matrix_inverse;
uniform mat4 hiddenline_light_transform;
uniform vec3 hiddenline_light_pos;

in vec3 position;
in vec3 normal;

smooth out vec3 g_v;
smooth out vec3 g_l;
smooth out vec3 g_n;

out vec4 crd;
out vec3 cube_map_coord;

void main(){
	float homogeneous_divide = (1.0f/hiddenline_modelview_matrix_inverse[3].w);
	vec3 cam_pos_world = hiddenline_modelview_matrix_inverse[3].xyz*homogeneous_divide;

	g_v = normalize(cam_pos_world - position);
	g_l = normalize(hiddenline_light_pos - position);
	g_n = normalize(normal);

	crd = hiddenline_light_transform * vec4(position, 1.0);

	cube_map_coord = position.xyz;

	gl_Position = hiddenline_projection_matrix * vec4(position, 1.0);
}
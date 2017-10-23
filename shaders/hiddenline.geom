#version 150
 

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

smooth in vec3 g_n[3];
smooth in vec3 g_v[3];
smooth in vec3 g_l[3];

in vec4 crd[3];
in vec3 cube_map_coord[3];


out vec4 texture_coord;
out vec3 diffuse_cube_coord;

smooth out vec3 f_n;
smooth out vec3 f_v;
smooth out vec3 f_l;

smooth out vec3 bary;

void main(){
	bary = vec3(1.0, 0.0, 0.0);

	gl_Position = gl_in[0].gl_Position;

	f_n = g_n[0];
	f_v = g_v[0];
	f_l = g_l[0];

	texture_coord = crd[0];
	diffuse_cube_coord = cube_map_coord[0];

	EmitVertex();

	bary = vec3(0.0, 1.0, 0.0);

	gl_Position = gl_in[1].gl_Position;

	f_n = g_n[1];
	f_v = g_v[1];
	f_l = g_l[1];

	texture_coord = crd[1];
	diffuse_cube_coord = cube_map_coord[1];

	
	EmitVertex();

	bary = vec3(0.0, 0.0, 1.0);

	gl_Position = gl_in[2].gl_Position;

	f_n = g_n[2];
	f_v = g_v[2];
	f_l = g_l[2];

	texture_coord = crd[2];
	diffuse_cube_coord = cube_map_coord[2];

	
	EmitVertex();

	EndPrimitive();
}


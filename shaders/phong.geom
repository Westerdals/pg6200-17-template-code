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
 
void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		f_n = g_n[i];
		f_v = g_v[i];
		f_l = g_l[i];

		texture_coord = crd[i];

		diffuse_cube_coord = cube_map_coord[i];
		
		gl_Position =  gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
#version 150
 
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 cube_tex_coord[3];
in vec3 v[3];
in vec3 l[3];
in vec3 n[3];

out vec3 cube_map_coord;
out vec3 view;
out vec3 light;
out vec3 normal;

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		cube_map_coord = cube_tex_coord[i];
		view = v[i];
		light = l[i];
		normal = n[i];
		
		gl_Position =  gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
#version 150

uniform vec3 phong_color;
uniform sampler2D phong_shadow_map;

uniform samplerCube phong_cube_map;

smooth in vec3 f_n;
smooth in vec3 f_v;
smooth in vec3 f_l;

in vec4 texture_coord; 
in vec3 diffuse_cube_coord;

out vec4 out_color;


void main() {
	

	vec3 l = normalize(f_l);
    vec3 h = normalize(normalize(f_v)+l);
    vec3 n = normalize(f_n);
	
	vec4 cube_map_color = texture(phong_cube_map, diffuse_cube_coord) * dot(n, l);

	float spec = pow(max(0.0f, dot(n, h)), 128.0f);

	vec3 pre_shadow_color = cube_map_color.xyz * phong_color + spec;

	float bias = 0.005f;	

	bool shadow = false;
	if(texture_coord.x > 0 && texture_coord.x < texture_coord.w &&
	 texture_coord.y > 0 && texture_coord.y < texture_coord.w)
	{
		shadow = textureProj(phong_shadow_map, texture_coord).z < (texture_coord.z - bias)/texture_coord.w;
	}

	if(shadow)
	{
		out_color = vec4(0.0, 0.0, 0.0, 1.0f);
	} else
	{
		out_color = vec4(pre_shadow_color, 1.0f);
	}
}
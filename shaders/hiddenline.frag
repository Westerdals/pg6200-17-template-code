#version 150

uniform vec3 hiddenline_color;
uniform sampler2D hiddenline_shadow_map;

uniform samplerCube hiddenline_cube_map;

smooth in vec3 f_n;
smooth in vec3 f_v;
smooth in vec3 f_l;

in vec4 texture_coord; 
in vec3 diffuse_cube_coord;

out vec4 out_color;

smooth in vec3 bary;

// Creates an anti-aliased line
float amplify(float d, float scale, float offset){
	d = scale * d + offset;
	d = clamp(d, 0, 1);
	d = 1 - exp2(-2*d*d);
	return d;
}

void main() {
	
	bool shadow = false;

	vec3 l = normalize(f_l);
    vec3 h = normalize(normalize(f_v)+l);
    vec3 n = normalize(f_n);

    float spec = pow(max(0.0f, dot(n, h)), 128.0f);

	vec4 cube_map_color = texture(hiddenline_cube_map, diffuse_cube_coord) * dot(n, l);

	vec3 pre_shadow_color = cube_map_color.xyz * hiddenline_color + spec;

	float min_bary = min(min(bary.x, bary.y), bary.z);

	pre_shadow_color = amplify(min_bary, 40, -0.5)*pre_shadow_color;

	float bias = 0.005f;	

	if(texture_coord.x >= 0 && texture_coord.x < texture_coord.w && texture_coord.y >= 0 && texture_coord.y < texture_coord.w)
	{
		shadow = (textureProj(hiddenline_shadow_map, texture_coord).z < (texture_coord.z - bias)/texture_coord.w);
	}

	if(shadow)
	{
		out_color = vec4(0.0, 0.0, 0.0, 1.0f);
	} else
	{
		out_color = vec4(pre_shadow_color, 1.0);
	}
}



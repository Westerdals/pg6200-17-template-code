#version 150
in vec2 ex_depth_texture;
uniform sampler2D shadow_map;

out vec4 out_color;

void main(){
	float Depth = texture(shadow_map, ex_depth_texture).x;                               
    Depth = 1.0 - (1.0 - Depth) * 25.0;
	out_color = vec4(Depth);
}
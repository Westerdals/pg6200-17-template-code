#version 150

uniform sampler2D texture;

in vec2 tex_coord;

out vec4 res_colour;

void main(){
	vec4 colour = texture2D(texture, tex_coord.xy);
	res_colour = vec4(colour.xyz, 0.8f);
}
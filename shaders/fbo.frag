#version 150

uniform sampler2D texture;

in vec2 tex_coord;

out vec4 res_colour;

void main(){
	//vec4 colour = texture2D(texture, tex_coord.xy);
	float colour = texture2D(texture, tex_coord.xy).x;
	colour = 1.0 - (1.0 - colour) * 15.0;
	//res_colour = vec4(colour.xyz, 0.8f);
	//res_colour = vec4(vec3(colour.x), 0.8f);
	
	//res_colour = vec4(vec3(colour.x), 1.0f);
	res_colour = vec4(colour);
}
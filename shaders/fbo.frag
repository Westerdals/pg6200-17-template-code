#version 150

uniform sampler2D fbo_texture;

in vec2 ex_texcoord;

out vec4 out_color;

void main(){
	vec3 color = vec3(texture2D(fbo_texture, ex_texcoord.xy).rrr);
	out_color = vec4(color, 1.0f);
	out_color.a = 0.8f;
}
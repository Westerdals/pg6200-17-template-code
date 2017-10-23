#version 150

uniform mat3 fbo_transform;

in vec2 in_Position;

out vec2 ex_texcoord;

void main(){
	vec2 new_pos = (fbo_transform * vec3(in_Position, 1.0)).xy;

	gl_Position = vec4(new_pos.x, new_pos.y, 0.5, 1.0);;

	ex_texcoord = 0.5*in_Position + vec2(0.5);
}
#version 150

uniform mat3 transform;

in vec2 position;

out vec2 tex_coord;

void main(){
	vec2 new_pos = (transform * vec3(position, 1.0)).xy;

	//gl_Position = vec4(new_pos.x, new_pos.y, 0.5, 1.0);;
	gl_Position = vec4(new_pos.x, new_pos.y, 0.5, 1.0);;

	tex_coord = 0.5*position + vec2(0.5);
}
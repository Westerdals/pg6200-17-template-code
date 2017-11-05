#version 130

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec3 light_position;

in  vec3 position;
in  vec3 normal;
out vec3 ex_Color;
out vec3 ex_Normal;
out vec3 ex_View;
out vec3 ex_Light;

void main() {
	vec4 pos = modelview_matrix * vec4(position, 1.0);
	gl_Position = projection_matrix * pos;
	ex_Color = vec3(0.5f, 0.5f, 1.0f);
	ex_Normal = normal_matrix * normal;
	ex_View = -pos.rgb;
	ex_Light = light_position - pos.rgb;
}
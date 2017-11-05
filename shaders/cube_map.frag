#version 150

uniform samplerCube cubemap;
uniform vec3 colour;

in vec3 cube_map_coord;
in vec3 view;
in vec3 light;
in vec3 normal;

out vec4 res_colour;

void main() {
	vec3 l = normalize(light);
    vec3 h = normalize(normalize(view) + light);
    vec3 n = normalize(normal);

	float spec = pow(max(0.0f, dot(h, n)), 128.0f);
	vec4 diffuse = texture(cubemap, cube_map_coord) * dot(l, n);

    res_colour = diffuse * vec4(colour + vec3(spec), 1.f);
}

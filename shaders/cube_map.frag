#version 150

uniform samplerCube cubemap;
uniform vec3 colour;

in vec3 cube_map_coord;
in vec3 view;
in vec3 light;
in vec3 normal;


void main() {
	vec3 l = normalize(light);
    vec3 h = normalize(normalize(view) + l);
    vec3 n = normalize(normal);

	float spec = pow(max(0.f, dot(h, n)), 128.f);
	vec4 diffuse = texture(cubemap, cube_map_coord) * dot(l, n);

	gl_FragColor = diffuse * vec4(colour, 1.f) + vec4(spec);
}

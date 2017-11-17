#version 150

uniform samplerCube cubemap;
uniform sampler2DShadow shadow_map;
//uniform sampler2D shadow_map;
uniform sampler2D gShadowMap;                                                       

uniform vec3 colour;

in vec3 cube_map_coord;
in vec3 view;
in vec3 light;
in vec3 normal;

in vec4 shadow_coord;

void main() {
	vec3 l = normalize(light);
    vec3 h = normalize(normalize(view) + l);
    vec3 n = normalize(normal);

	float spec = pow(max(0.f, dot(h, n)), 128.f);
	vec4 diffuse = texture(cubemap, cube_map_coord) * dot(l, n);
	float f = textureProj(shadow_map, shadow_coord);
//	gl_FragColor = vec4( vec3(f * (diffuse * vec3(colour) + vec3(spec))), 1.0f );

	gl_FragColor = f*diffuse * vec4(colour, 1.f) + vec4(spec);
}

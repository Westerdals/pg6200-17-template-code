#version 330

uniform samplerCube cubemap;
uniform sampler2DShadow shadow_map;
//uniform sampler2D shadow_map;
uniform sampler2D gShadowMap;                                                       

uniform vec3 colour;

in vec3 cube_map_coord;
in vec3 view;
in vec3 light;
in vec3 normal;

in vec4 lightSpaceCoords;

void main() {
	vec3 l = normalize(light);
	vec3 h = normalize(normalize(view) + l);
	vec3 n = normalize(normal);

	float spec = pow(max(0.f, dot(h, n)), 128.f);
	vec4 diffuse = texture(cubemap, cube_map_coord) * dot(l, n);
	
	float f = 1.0f;
	float depth = textureProj(shadow_map, lightSpaceCoords);
	if(depth < (lightSpaceCoords.z / lightSpaceCoords.w))
	{
		f = 0.5f;
	}
//	gl_FragColor = vec4( f * vec3((diffuse * vec3(colour) + vec3(spec))), 1.0f );

	gl_FragColor = vec4(f*vec3(diffuse * colour), 1.f) + vec4(f*vec3(spec), 1.0f);
	
}

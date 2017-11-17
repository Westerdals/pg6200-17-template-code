#version 150

//out vec4 out_color;
//out float fragmentDepth;
out vec4 fragmentDepth;

void main(){
	//float Depth = texture(shadow_map, ex_depth_texture).x;                               
    //Depth = 1.0 - (1.0 - Depth) * 25.0;
	//out_color = vec4(Depth);
	//out_color = vec4(1.0);
	fragmentDepth = vec4(vec3(1.0 - (1.0f - gl_FragCoord.z)*5.0), 1.f);
	//fragmentDepth = gl_FragCoord.z;
}
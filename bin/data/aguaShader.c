#version 430 compatibility
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform float iTime;

#define N (256)
#define WIDTH (1024)
#define HEIGHT (768)

#define TAU 6.28318530718

#define TILING_FACTOR 1.0
#define MAX_ITER 8

float waterHighlight(vec2 p, float time, float foaminess)
{
    vec2 i = vec2(p);
    float c = 0.0;
    float foaminess_factor = mix(1.0, 6.0, foaminess);
	float inten = .005 * foaminess_factor;

	for (int n = 0; n < MAX_ITER; n++) 
	{
		float t = time * (1.0 - (3.5 / float(n+1)));
		i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
		c += 1.0/length(vec2(p.x / (sin(i.x+t)),p.y / (cos(i.y+t))));
	}
	c = 0.2 + c / (inten * float(MAX_ITER));
	c = 1.17-pow(c, 1.4);
    c = pow(abs(c), 8.0);
	return c / sqrt(foaminess_factor);
}

float rayStrength(vec2 raySource, vec2 rayRefDirection, vec2 coord, float seedA, float seedB, float speed)
{
	vec2 sourceToCoord = coord - raySource;
	float cosAngle = dot(normalize(sourceToCoord), rayRefDirection);
	
	return clamp(
		(0.45 + 0.15 * sin(cosAngle * seedA + iTime * speed)) +
		(0.3 + 0.2 * cos(-cosAngle * seedB + iTime * speed)),
		0.0, 1.0) *
		clamp((WIDTH - length(sourceToCoord)) / WIDTH, 0.5, 1.0);
}

void main()
{
	vec2 coord = gl_TexCoord[0].st;
	
	
	// Set the parameters of the sun rays
	vec2 rayPos1 = vec2(WIDTH * 0.3, HEIGHT * -0.2);
	vec2 rayRefDir1 = normalize(vec2(1.0, -0.116));
	float raySeedA1 = 36.2214;
	float raySeedB1 = 21.11349;
	float raySpeed1 = 1.5;
	
	vec2 rayPos2 = vec2(WIDTH * sin(iTime/100), HEIGHT * -0.6);
	vec2 rayRefDir2 = normalize(vec2(1.0, 0.241));
	const float raySeedA2 = 22.39910;
	const float raySeedB2 = 18.0234;
	const float raySpeed2 = 1.1;
	
	// Calculate the colour of the sun rays on the current fragment
	vec4 rays1 =
		vec4(1.0, 1.0, 1.0, 1.0) *
		rayStrength(rayPos1, rayRefDir1, coord, raySeedA1, raySeedB1, raySpeed1);
	 
	vec4 rays2 =
		vec4(1.0, 1.0, 1.0, 1.0) *
		rayStrength(rayPos2, rayRefDir2, coord, raySeedA2, raySeedB2, raySpeed2);
	
	gl_FragColor = rays1 * 0.5 + rays2 * 0.4;
	
	// Attenuate brightness towards the bottom, simulating light-loss due to depth.
	// Give the whole thing a blue-green tinge as well.
	float brightness = 1.0 - (coord.y / HEIGHT);
	gl_FragColor.r *= 0.1 + (brightness * 0.8);
	gl_FragColor.g *= 0.3 + (brightness * 0.6);
	gl_FragColor.b *= 0.7 + (brightness * 0.5);



        // Desenha reflexos na agua
	float time = iTime * 0.1;
	vec2 uv = coord.xy / vec2( HEIGHT, WIDTH );
	vec2 uv_square = vec2(uv.x * WIDTH / HEIGHT, uv.y);

        float foaminess = smoothstep(0.4, 1.8, .6);
        float clearness = 0.1 + 0.9*smoothstep(0.1, 1., .8);

        vec2 p = mod(uv_square*TAU*TILING_FACTOR, TAU)-250.0;

        float c = waterHighlight(p, time, foaminess);

        vec3 water_color = vec3(0.1, 0.3, 0.5);
        vec3 color = vec3(c);
        color = clamp(color + water_color, 0.0, 1.0);

        color = mix(water_color, color, clearness);

        gl_FragColor = ( gl_FragColor + vec4(color, 1.0) ) / 1.2;
}




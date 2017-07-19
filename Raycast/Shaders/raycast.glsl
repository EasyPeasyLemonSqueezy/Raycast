#version 430 core

layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform image2D image;

layout (std430, binding = 1) buffer data
{
	vec4 colors[];
};

uniform vec3 eye;
uniform float distance;
uniform vec3 normal;
uniform float monitor;
uniform ivec2 screen;
uniform ivec3 size;
uniform vec3 d;
uniform vec3 min;
uniform vec3 max;
uniform mat4 rot;


void main()
{
	const vec2 pixel = gl_GlobalInvocationID.xy - vec2(screen.x / 2, screen.y / 2);

	vec3 start = -eye * distance;
	vec3 mon = eye * (monitor - distance) + vec3(pixel, monitor);
	vec3 ray = normalize(mon - start);

	float t = 0;
	float tn = 0;

	float denom = dot(-normal, ray);
	if (denom > 1e-6) {
		vec3 p = min - start;
		t = dot(p, normal) / denom;

		vec3 pn = vec3(min.x, min.y, min.z + d.z) - start;
		tn = dot(pn, normal) / denom;

		// ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
		// imageStore(image, pixelCoords, vec4(0.0, 1.0, 0.0, 1.0));
	}

	vec3 first = start * t;
	vec3 delta = start * tn - first;

	if (first.x > min.x && first.x < max.x && first.y > min.y && first.y < max.y)
	{
		ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
		imageStore(image, pixelCoords, vec4(0.0, 1.0, 0.0, 1.0));
	}

	vec3 color = vec3(0);
	float alpha = 0;

	int layer;
	int find = 0;
	for (ray = first, layer = 0; layer < size.z; ++layer, ray += delta)
	{
		const int xi = int(round((ray.x - min.x) / d.x));
		if (xi >= size.x || xi < 0) {
			continue;
		}

		const int yi = int(round((ray.y - min.y) / d.y));
		if (yi >= size.y || yi < 0) {
			continue;
		}

		const int zi = int(round((ray.z - min.z) / d.z));
		if (zi >= size.z || zi < 0) {
			continue;
		}

		const int i = (zi * size.x * size.y) + (yi * size.x) + xi;

		const vec4 c = colors[i];

		const float da = (1 - alpha) * c.a;
		color += da * c.xyz;
		alpha += da;

		find = 1;
		if (alpha > 0.999) {
			break;
		}
	}
	if (find == 1) {
		color = vec3(1, 1, 1);
	}

	// ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
	// imageStore(image, pixelCoords, vec4(color, 1.0));
}

#version 430 core

layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform image2D image;

layout(std430, binding = 1) buffer opacityData
{
	float opacities[];
};

layout(std430, binding = 2) buffer hueData
{
	float hues[];
};

const float eye_position = -500.0f;
const float monitor_position = 0.0f;

uniform int x;
uniform int y;
uniform int z;
uniform vec3 d;
uniform vec3 min;
uniform vec3 max;

struct color
{
	float opacity;
	float hue;
};

void main()
{
	color points[17];

	float to_volume = min.z - eye_position;
	float to_monitor = monitor_position - eye_position;

	vec2 pixel = gl_GlobalInvocationID.xy - vec2(640, 512);
	vec3 ray = vec3(pixel.x, pixel.y, to_monitor);
	ray *= to_volume / to_monitor;

	const vec3 delta = ray * (d.z / to_volume);
	ray.z = min.z;

	int layer;
	for (layer = 0; layer < z; ++layer, ray += delta)
	{
		int xi = int(round((ray.x - min.x) / d.x));
		int yi = int(round((ray.y - min.y) / d.y));
		int zi = int(round((ray.z - min.z) / d.z));

		if (xi >= x || xi < 0 ||
			yi >= y || yi < 0 ||
			zi >= z || zi < 0)
		{
			break;
		}

		const int i = (zi * x * y) + (yi * x) + xi;
		
		color c = color(opacities[i], hues[i]);
		points[layer] = c;

		if (c.opacity == 1)
		{
			break;
		}
	}

	float hue = 0;
	for (int i = 0; i < layer; i++)
	{
		hue = points[i].hue * points[i].opacity + hue * (1 - points[i].opacity);
	}

	ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
	imageStore(image, pixelCoords, vec4(hue, hue, hue, 1.0));
}
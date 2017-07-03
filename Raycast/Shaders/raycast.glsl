#version 430 core

layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform image2D image;

layout (std430, binding = 1) buffer data
{
	vec4 colors[];
};

uniform float eyePosition;
uniform float monitorPosition;
uniform ivec2 screen;
uniform ivec3 size;
uniform vec3 d;
uniform vec3 min;
uniform vec3 max;

void main()
{
	const float to_volume = min.z - eyePosition;
	const float to_monitor = monitorPosition - eyePosition;

	const vec2 pixel = gl_GlobalInvocationID.xy - vec2(screen.x / 2, screen.y / 2);
	
	vec3 ray = vec3(pixel.x, pixel.y, to_monitor);
	ray *= to_volume / to_monitor;

	const vec3 delta = ray * (d.z / to_volume);
	
	ray.z = min.z;

	vec3 color = vec3(0.0, 0.0, 0.0);

	int layer;
	for (layer = 0; layer < size.z; ++layer, ray += delta)
	{
		int xi = int(round((ray.x - min.x) / d.x));
		int yi = int(round((ray.y - min.y) / d.y));
		int zi = int(round((ray.z - min.z) / d.z));

		if (xi >= size.x || xi < 0 ||
			yi >= size.y || yi < 0 ||
			zi >= size.z || zi < 0)
		{
			break;
		}

		const int i = (zi * size.x * size.y) + (yi * size.x) + xi;
		
		vec4 c = colors[i];
		color = c.a * c.rgb + color.rgb * (1.0 - c.a);

		if (c.a == 1)
		{
			break;
		}
	}

	const int i = (15 * size.x * size.y) + (int(gl_GlobalInvocationID.y) * size.x) + int(gl_GlobalInvocationID.x);

	ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
	imageStore(image, pixelCoords, colors[i]);
}
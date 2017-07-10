#version 430 core

layout(local_size_x = 1, local_size_y = 1) in;

layout(rgba32f, binding = 0) uniform image2D image;

layout (std430, binding = 1) buffer data
{
	vec4 colors[];
};

uniform vec3 eyePosition;
uniform float eyeDistance;
uniform float monitorDistance;
uniform ivec2 screen;
uniform ivec3 size;
uniform vec3 d;
uniform vec3 min;
uniform vec3 max;
uniform mat4 rotation;

float intersectCube(vec3 eye, vec3 ray)
{
	float tNear = -1000000.0;
	float tFar = 1000000.0;
	float t1, t2;
	float temp;
	float tCube;

	bool intersects = true;
	for (int i = 0; i < 3; i++)
	{
		if (ray[i] == 0)
		{
			if (eye[i] < min[i] || eye[i] > max[i])
			{
				intersects = false;
			}
		}
		else
		{
			t1 = (min[i] - eye[i]) / ray[i];
			t2 = (max[i] - eye[i]) / ray[i];

			if (t1 > t2)
			{
				temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if (t1 > tNear)
				tNear = t1;

			if (t2 < tFar)
				tFar = t2;

			if (tNear > tFar)
				intersects = false;

			if (tFar < 0)
				intersects = false;
		}
	}

	if (intersects == false)
		tCube = -1;
	else
		tCube = tNear;

	return tCube;
}

void main()
{
	const float to_volume = max.z - eyeDistance;
	const float to_monitor = monitorDistance - eyeDistance;

	const vec2 pixel = gl_GlobalInvocationID.xy - vec2(screen.x / 2, screen.y / 2);

	vec3 ray = vec3(pixel.x, pixel.y, to_monitor);
	ray *= to_volume / to_monitor;
	ray.xyz = -(vec4(ray, 1.0) * rotation).xyz;

	float t = intersectCube(eyePosition, ray);

	const vec3 delta = ray * (d.z / to_volume);

	vec3 color = vec3(0.0, 0.0, 0.0);

	int layer;
	for (ray = eyePosition + t * ray, layer = size.z - 1; layer >= 0; --layer, ray += delta)
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

		const int i = (zi * size.x * size.y) + (yi * size.x) + xi;

		vec4 c = colors[i];
		color = c.a * c.rgb + color.rgb * (1.0 - c.a);
	}

	ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
	imageStore(image, pixelCoords, vec4(color, 1.0));
}

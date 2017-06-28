#pragma once
#include <stack>

typedef struct
{
	float hue;
	float opacity;
} color;

float blend(std::stack<color>);
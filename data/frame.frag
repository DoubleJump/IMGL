#version 330

uniform sampler2D diffuse;

in vec2 uv;

out vec4 result;

void main()
{
	result = texture(diffuse, uv);
}
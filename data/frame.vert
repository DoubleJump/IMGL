#version 330

layout(location = 0) in vec3 _position;
layout(location = 1) in vec2 _uv;

out vec2 uv;

void main()
{
	uv = _uv;
	gl_Position = vec4(_position, 1.0);
}
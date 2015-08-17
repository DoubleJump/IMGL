#version 330

layout(location = 0) in vec3 _position;
layout(location = 1) in vec2 _uv;
layout(location = 3) in vec4 _color;

out vec2 uv;
out vec4 color;

void main()
{
	uv = _uv;
	color = _color;
	gl_Position = vec4(_position, 1.0);
}
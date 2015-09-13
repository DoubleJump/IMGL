#VERTEX

#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

out vec2 _uv;

void main()
{
	_uv = uv;
	gl_Position = vec4(position, 1.0);
}

#FRAGMENT

#version 330

uniform sampler2D diffuse;
in vec2 _uv;
out vec4 result;
void main()
{
	result = texture(diffuse, _uv);
}
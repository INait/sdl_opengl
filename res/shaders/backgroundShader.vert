#version 330

layout(location=0) in vec3 in_position;
layout(location=2) in vec3 in_normal;
layout(location=8) in vec2 in_texcoord;

out vec2 v2f_texcoord;

uniform mat4 ModelViewProjectionMatrix;

void main()
{
    gl_Position = ModelViewProjectionMatrix * vec4(in_position, 1);
    v2f_texcoord = in_texcoord;
}
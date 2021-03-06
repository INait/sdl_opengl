#version 420

in vec2 v2f_texcoord;

layout (binding=0) uniform sampler2D imageSampler;

layout (location=0) out vec4 out_color;

void main()
{
    out_color = texture(imageSampler, v2f_texcoord);
}
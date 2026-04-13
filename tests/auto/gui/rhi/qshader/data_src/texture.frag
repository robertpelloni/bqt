#version 440

layout(location = 0) in vec2 bobui_TexCoord;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 bobui_Matrix;
    float opacity;
} ubuf;

layout(binding = 1) uniform sampler2D bobui_Texture;

void main()
{
    fragColor = texture(bobui_Texture, bobui_TexCoord) * ubuf.opacity;
}

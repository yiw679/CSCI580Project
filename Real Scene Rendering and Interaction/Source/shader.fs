#version 330 core

in vec4 outColor;
in vec2 TexCoord;

out vec4 OutColor;
uniform sampler2D OutTexture;
uniform float tileSize;

void main()
{
    OutColor = texture(OutTexture, fract(TexCoord.xy / vec2(tileSize, tileSize)));
}
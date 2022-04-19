#version 330 core

in vec4 outColor;
in vec2 TexCoord;

out vec4 OutColor;
uniform sampler2D OutTexture1;
uniform sampler2D OutTexture2;
uniform sampler2D OutTexture3;
uniform float tileSize;

void main() 
{
    vec2 tile = fract(TexCoord.xy / vec2(tileSize, tileSize));
    vec4 grass = texture(OutTexture1, tile);
    vec4 rock = texture(OutTexture2, tile);
    vec4 snow = texture(OutTexture3, tile);

    OutColor = grass * outColor.x + rock * outColor.y + snow * outColor.z;
}
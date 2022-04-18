#version 330 core

in vec4 v2f_positionW; // Position in world space.
in vec4 v2f_normalW; // Surface normal in world space.
in vec2 v2f_texcoord;
in vec4 outColor;
in vec2 TexCoord;

uniform vec4 EyePosW;   // Eye position in world space.

uniform vec4 LightPosW; // Light's position in world space.
uniform vec4 LightColor; // Light's diffuse and specular contribution.
uniform vec4 Ambient; // Global ambient contribution.
 
uniform vec4 MaterialEmissive;
uniform vec4 MaterialDiffuse;
uniform vec4 MaterialSpecular;
uniform float MaterialShininess;

out vec4 OutColor;
uniform sampler2D OutTexture;
uniform float tileSize;

void main()
{
    OutColor = texture(OutTexture, fract(TexCoord.xy / vec2(tileSize, tileSize)));

    // Compute the emissive term.
    vec4 Emissive = MaterialEmissive;
 
    // Compute the diffuse term.
    vec4 N = normalize( v2f_normalW );
    vec4 L = normalize( LightPosW - v2f_positionW );
    float NdotL = max( dot( N, L ), 0 );
    vec4 Diffuse =  NdotL * LightColor * MaterialDiffuse;
    
    // Compute the specular term.
    vec4 V = normalize( EyePosW - v2f_positionW );
    vec4 H = normalize( L + V );
    vec4 R = reflect( -L, N );
    float RdotV = max( dot( R, V ), 0 );
    float NdotH = max( dot( N, H ), 0 );
    vec4 Specular = pow( RdotV, MaterialShininess ) * LightColor * MaterialSpecular;
    
    OutColor = ( Emissive + Ambient + Diffuse + Specular ) * texture(OutTexture, fract(TexCoord.xy / vec2(tileSize, tileSize)));
}
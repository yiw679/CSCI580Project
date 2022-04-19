#version 330 core
#extension GL_ARB_separate_shader_objects: enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 v2f_positionW; // Position in world space.
out vec4 v2f_normalW; // Surface normal in world space.
out vec2 v2f_texcoord;
out vec4 outColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    outColor = aColor;
    TexCoord = aTexCoord;

    v2f_positionW = model * vec4(aPos, 1); 
    v2f_normalW = model * vec4(aNormal, 0);
    v2f_texcoord = aTexCoord;
}
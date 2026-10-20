#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix;
layout (location = 7) in float texId;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;
out float texIndex;

void main()
{
   gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
   TexCoords = aTexCoord;
   texIndex = texId;
}
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
   mat4 modelViewMat = view * model;

   // remove all rotations so quad faces camera
   // NOTE: GLSL is [column][row]

   // column 0
   modelViewMat[0][0] = 1;
   modelViewMat[0][1] = 0;
   modelViewMat[0][2] = 0;

   // column 1
   modelViewMat[1][0] = 0;
   modelViewMat[1][1] = 1;
   modelViewMat[1][2] = 0;

   // column 2
   modelViewMat[2][0] = 0;
   modelViewMat[2][1] = 0;
   modelViewMat[2][2] = 1;

   gl_Position = projection * modelViewMat * vec4(aPos, 1.0);
   TexCoord = aTexCoord;
}
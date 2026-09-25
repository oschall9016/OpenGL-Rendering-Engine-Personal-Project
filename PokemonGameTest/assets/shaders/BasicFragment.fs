#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform float x;
uniform float z;

void main()
{

    if (mod(x + z, 2.0) == 0.0)
        FragColor = vec4(1,0.5,0.2,1);
    else
        FragColor = vec4(0.7,0.7,0.7,0.7);
}
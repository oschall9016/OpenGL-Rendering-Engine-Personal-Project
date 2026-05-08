#include "Sprite_Billboard.h"
#include "Mesh.h"

float vertices[] =
{
	-0.5f,  0.5f, 0.0f, // top left 0
	 0.5f,  0.5f, 0.0f, // top right 1
	 0.5f, -0.5f, 0.0f, // bottom right 2
	-0.5f, -0.5f, 0.0f  // bottom left 3
};

unsigned int indices[] =
{
	 0, 1, 3,
	 1, 2, 3
};
/*
Sprite_Billboard::Sprite_Billboard() : Mesh()
{
	
}
*/


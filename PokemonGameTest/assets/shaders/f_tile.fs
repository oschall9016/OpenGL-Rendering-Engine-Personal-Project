#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D material_texture_diffuse1;

uniform float texIndex;

void main()
{
	vec2 uv = TexCoords;

	// size of texture sheet
	float sheetX = 20.0;
	float sheetY = 17.0;

	// seperate texIndex into x and y
	float xIndex = mod(texIndex, sheetX);
	float yIndex = floor(texIndex / sheetX); 

	// resize sheet to one texture
	uv.x /= sheetX; 
	uv.y /= sheetY;

	// translate to given texIndex
	uv.x += xIndex / sheetX;
	uv.y += (1.0 - yIndex) / sheetY; // swap rows to make 0,0 top left

	vec4 textureColor = texture(material_texture_diffuse1, uv);

	FragColor = textureColor;
}
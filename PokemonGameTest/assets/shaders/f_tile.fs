#version 330 core


in vec2 TexCoords;
in float texIndex;

out vec4 FragColor;

uniform sampler2D material_texture_diffuse1;

void main()
{
	vec2 uv = TexCoords;

	// size of texture sheet
	float sheetX = 40.0;
	float sheetY = 34.0;

	// seperate texIndex into x and y
	float xIndex = mod(texIndex, sheetX);
	float yIndex = floor(texIndex / sheetX); 

	// resize sheet to one texture
	uv.x /= sheetX; 
	uv.y /= sheetY;

	// translate to given texIndex
	uv.x += xIndex / sheetX;
	uv.y += yIndex / sheetY; 

	vec4 textureColor = texture(material_texture_diffuse1, uv);

	FragColor = textureColor;
}
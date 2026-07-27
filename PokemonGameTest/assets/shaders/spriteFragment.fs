#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D material_texture_diffuse1;

uniform float uvRow;
uniform float uvCol;

void main()
{
	//////// find sprite uv location in sprite sheet ////////
	vec2 uv = TexCoord;

	// size of sprite sheet
	float sheetCols = 12.0; 
	float sheetRows = 2.0;
	
	// location of desired sprite in sheet
	//float colIndex = 11.0;
	//float rowIndex = 0.0;

	uv.x /= sheetCols; // amount of sprites in a row
	uv.y /= sheetRows;  // amount of rows

	uv.x += uvCol / sheetCols;
	uv.y += (1.0 - uvRow) / sheetRows; // swap rows to make 0,0 top left

	vec4 textureColor = texture(material_texture_diffuse1, uv);

	// transparency
	if(textureColor.a < 0.1)
	{
		discard;
	}

	FragColor = textureColor;
}
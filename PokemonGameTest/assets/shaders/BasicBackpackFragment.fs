#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D material_texture_diffuse1;

void main()
{
	vec4 textureColor = texture(material_texture_diffuse1, TexCoord);
	// transparency
	if(textureColor.a < 0.1)
	{
		discard;
	}

	FragColor = textureColor;
}
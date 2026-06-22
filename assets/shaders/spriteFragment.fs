#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D material_texture_diffuse1;

void main()
{

	vec2 uv = TexCoord;
	uv.x /= 8.0;
	uv.y /= 3.0;
	uv.x += 64.0;

	vec4 textureColor = texture(material_texture_diffuse1, uv);

	if(textureColor.a < 0.1)
	{
		discard;
	}

	FragColor = textureColor;
}
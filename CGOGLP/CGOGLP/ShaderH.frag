#version 330 core

out vec4 FragColor;

in float Height;
in vec4 Color;
in vec2 TexCoords;

uniform sampler2D tex0;

void main()
{
    float h = (Height + 16)/32.0f;	// shift and scale the height into a grayscale value
    //FragColor = vec4(h, h, h, 1.0);
    //vec2 texCoord = vec2(1.0f,1.0f);
    //vec4 TexColor = texture(tex0,TexCoords);
    //FragColor = Color * TexColor;
    FragColor = texture(tex0,TexCoords);

}
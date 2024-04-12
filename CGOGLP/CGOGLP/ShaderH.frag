#version 330 core

out vec4 FragColor;

in float Height;
in vec3 Color;
in vec2 TexCoords;

uniform sampler2D tex0;

void main()
{
    float h = (Height + 16)/32.0f;	// shift and scale the height into a grayscale value
    //FragColor = vec4(h, h, h, 1.0);
    //vec2 texCoord = vec2(1.0f,1.0f);
    FragColor = texture(tex0,TexCoords);
}
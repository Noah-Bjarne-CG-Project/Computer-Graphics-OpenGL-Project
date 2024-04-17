#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
//layout (location = 1) in vec3 aColor

out float Height;
out vec3 Position;
out vec2 TexCoords;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Height = aPos.y;
    Position = (view * model * vec4(aPos, 1.0f)).xyz;
    //Color = aColor;
    float h = (Height + 16)/32.0f;     // shift and scale the height into a grayscale value
    Color = vec4(h,h,h,1.0f);
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
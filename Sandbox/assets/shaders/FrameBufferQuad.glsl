#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
} 


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) out vec4 color;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;


void main()
{ 
    color = texture(screenTexture, TexCoords);
}
#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = vec2((aPos.x + 1) / 2, (aPos.y + 1) / 2);
    
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
} 


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) out vec4 color;
  
in vec2 v_TexCoords;

uniform sampler2D u_ScreenTexture;


const float offset = 1.0 / 300.0;  

void main()
{ 
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1.0, -1.0, -1.0,
        -1.0,  9.0, -1.0,
        -1.0, -1.0, -1.0
    );

    //color = vec4(vec3(1.0 - texture(u_ScreenTexture, v_TexCoords)), 1.0);
    //color = vec4(v_TexCoords, 0.0, 1.0);

    //Black & White
    color = texture(u_ScreenTexture, v_TexCoords);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    vec3 col = vec3(average, average, average);

    //Edge detection
//    vec3 sampleTex[9];
//    for(int i = 0; i < 9; i++)
//    {
//        sampleTex[i] = vec3(texture(u_ScreenTexture, v_TexCoords.st + offsets[i]));
//    }
//    vec3 col = vec3(0.0);
//    for(int i = 0; i < 9; i++)
//        col += sampleTex[i] * kernel[i];

    if(v_TexCoords.x < 0.5)
        color = vec4(col, 1.0);
    else
        color = texture(u_ScreenTexture, v_TexCoords);
}
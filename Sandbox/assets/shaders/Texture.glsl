//Basic Texture Shader


#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMat;
uniform mat4 u_ModelMat;

out vec2 v_TexCoord;

void main() 
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjectionMat * u_ModelMat * vec4(a_Position, 1.0);
}


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

//Output
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//Output
	color = texture(u_Texture, v_TexCoord * 10) * u_Color;

}
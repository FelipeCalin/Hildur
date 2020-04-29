#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_ViewProjectionMat;
uniform mat4 u_ModelMat;


void main()
{
	//vertex shader
	gl_Position = u_ViewProjectionMat * u_ModelMat * vec4(a_Pos, 1.0);
}


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) out vec4 color;

uniform vec3 lightColor;


void main()
{

	color = vec4(lightColor, 1.0f);
	
}
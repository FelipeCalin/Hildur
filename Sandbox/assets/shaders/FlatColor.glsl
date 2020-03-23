// Flat Color Shader


#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjectionMat;
uniform mat4 u_ModelMat;

void main()
{
	gl_Position = u_ViewProjectionMat * u_ModelMat * vec4(a_Position, 1.0);
}


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

//Output
layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	//Output
	color = u_Color;
	
}
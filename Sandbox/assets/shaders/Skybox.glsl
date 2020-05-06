#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjectionMat;

out vec3 v_TexCoord;


void main()
{
	v_TexCoord = a_Position;

	vec4 position = u_ViewProjectionMat * vec4(a_Position, 1.0);
	gl_Position = position.xyww;
}


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

//Output
layout(location = 0) out vec4 color;

uniform samplerCube u_CubeMap;

in vec3 v_TexCoord;


void main()
{
	//Output
	color = texture(u_CubeMap, v_TexCoord);
	//color = vec4(0.5019f, 0.2902f, 0.7294f, 1.0f);
	
}
#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjectionMat;
uniform mat4 u_ModelMat;
uniform vec3 u_ViewPos;

out vec3 v_Pos;
out vec2 v_TexCoord;
out vec3 v_Normal;

out vec3 v_ViewPos;


void main()
{

	//Variables for fragment shader
	v_Pos = vec4(u_ModelMat * vec4(a_Pos, 1.0)).xyz;
	v_TexCoord = a_TexCoord;
	v_Normal = mat3(u_ModelMat) * a_Normal;

	v_ViewPos = u_ViewPos;

	//vertex shader
	gl_Position = u_ViewProjectionMat * u_ModelMat * vec4(a_Pos, 1.0);

}


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

//Output
layout(location = 0) out vec4 color;

float near = 0.1;
float far = 100.0;


float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //color = vec4(vec3(depth), 1.0);
    color = vec4(0.1, 0.5, 0.9, 1.0);
}
#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec3 u_ViewPos;


out vec3 v_Pos;
out vec3 v_ViewPos;
out vec2 v_TexCoord;
out vec3 v_Normal;


void main()
{

	//Variables for fragment shader
	v_Pos = vec4(u_Transform * vec4(a_Pos, 1.0)).xyz;
	v_ViewPos = u_ViewPos;
	v_TexCoord = a_TexCoord;
	v_Normal = mat3(u_Transform) * a_Normal;
	//v_Normal = a_Normal;


	//vertex shader
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);

}


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) out vec4 color;

in vec3 v_Pos;
in vec3 v_ViewPos;
in vec2 v_TexCoord;
in vec3 v_Normal;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture2;

uniform vec3 u_LightPos0;

void main()
{

	//Ambient light
	vec3 ambLight = vec3(0.2f, 0.2f, 0.2f);

	//Diffuse light
	vec3 PosToLightDirVec = normalize(v_Pos - u_LightPos0);
	vec3 diffuseCol = vec3(1.0f, 1.0f, 1.0f);
	float diffuse = clamp(dot(PosToLightDirVec, v_Normal), 0, 1);
	vec3 diffuseFinal = diffuseCol * diffuse;

	//Specular light
	vec3 lightToPosDirVec = normalize(u_LightPos0 - v_Pos);
	vec3 refectDirDirVec = normalize(reflect(lightToPosDirVec, normalize(v_Normal)));
	vec3 posToViewVecDirVec = normalize(v_Pos - v_ViewPos);
	float specularConstant = pow(max(dot(posToViewVecDirVec, refectDirDirVec), 0), 35);
	vec3 specularFinal = vec3(1.0f, 1.0f, 1.0f) * specularConstant;


	color = mix(texture(u_Texture, v_TexCoord), texture(u_Texture2, v_TexCoord), 1.0)
		* (vec4(ambLight, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));
	
}
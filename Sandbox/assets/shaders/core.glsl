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


//Material
struct Material {

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};


//Variables from vertex shader
in vec3 v_Pos;
in vec2 v_TexCoord;
in vec3 v_Normal;

in vec3 v_ViewPos;

//Uniforms
uniform Material material;

uniform sampler2D diffuseTex1;
uniform sampler2D diffuseTex2;
uniform sampler2D diffuseTex3;
uniform sampler2D specularTex1;
uniform sampler2D specularTex2;

uniform vec3 u_LightPos1;

//textures (diffuse, specular)
//uniform sampler2D u_Texture;
//uniform sampler2D u_Texture2;


void main()
{

	//Ambient light
	vec3 ambLight = material.ambient;

	//Diffuse light
	vec3 PosToLightDirVec = normalize(u_LightPos0 - v_Pos);
	vec3 diffuseCol = vec3(1.0f, 1.0f, 1.0f);
	float diffuse = clamp(dot(PosToLightDirVec, v_Normal), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	//Specular light
	vec3 lightToPosDirVec = normalize(v_Pos - u_LightPos0);
	vec3 refectDirDirVec = normalize(reflect(lightToPosDirVec, normalize(v_Normal)));
	vec3 posToViewVecDirVec = normalize(v_ViewPos - v_Pos);
	float specularConstant = pow(max(dot(posToViewVecDirVec, refectDirDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant;

	//Output
	color = mix(texture(diffuseTex, v_TexCoord), texture(specularTex, v_TexCoord), 1.0)
		* (vec4(ambLight, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));
	
}
#type vertex
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjectionMat;
uniform mat4 u_ModelMat;

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

	//vertex shader
	gl_Position = u_ViewProjectionMat * u_ModelMat * vec4(a_Pos, 1.0);

}


#type fragment
#version 140 core
#extension GL_ARB_explicit_attrib_location : require

//Output
layout(location = 0) out vec4 color;


// Material
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


// Light
struct DirectionalLight 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct PointLight 
{    
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;  
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic; 

    float cutOff;
    float outerCutOff;
};


// Variables from vertex shader
in vec3 v_Pos;
in vec2 v_TexCoord;
in vec3 v_Normal;


// Uniforms
uniform Material material;
#define MAX_NR_LIGHTS 5
uniform int nrDirectionalLights;
uniform DirectionalLight[MAX_NR_LIGHTS] u_DirLights;
uniform int nrPointLights;
uniform PointLight[MAX_NR_LIGHTS] u_PointLights;
uniform int nrSpotLights;
uniform SpotLight[MAX_NR_LIGHTS] u_SpotLights;


// Uniform Maps
uniform sampler2D albedoMap;
//uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
//uniform sampler2D aoMap;
//uniform sampler2D metallicMap;
uniform sampler2D emissionMap;


uniform vec3 u_ViewPos;


vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(albedoMap, v_TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(albedoMap, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(roughnessMap, v_TexCoord).r);
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient   * vec3(texture(albedoMap, v_TexCoord));
    vec3 diffuse  = light.diffuse   * material.diffuse  * (diff * vec3(texture(albedoMap, v_TexCoord)));
    vec3 specular = light.specular  * material.specular * (spec * vec3(texture(roughnessMap, v_TexCoord).r));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

    if(theta > light.cutOff) 
    {   
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        // specular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        // combine results
        vec3 ambient  = light.ambient  * vec3(texture(albedoMap, v_TexCoord));
        vec3 diffuse  = light.diffuse  * material.diffuse  * diff * vec3(texture(albedoMap, v_TexCoord));
        vec3 specular = light.specular * material.specular * spec * vec3(texture(roughnessMap, v_TexCoord).r);
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        return (ambient + diffuse + specular);
    }
    else 
    {
        vec3 result = light.ambient * vec3(texture(albedoMap, v_TexCoord));

        return result;
    }
}
 


void main()
{ 
    // properties
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_Pos);

    vec3 result = (material.ambient * vec3(texture(albedoMap, v_TexCoord))) + (vec3(texture(emissionMap, v_TexCoord)));


    // phase 1: Directional lighting
    for(int i = 0; i < nrDirectionalLights; i++)
    {
        result += CalcDirLight(u_DirLights[i], norm, viewDir);
    }

    // phase 2: Point lights
    for(int i = 0; i < nrPointLights; i++)
    {
        result += CalcPointLight(u_PointLights[i], norm, v_Pos, viewDir);
    }

    // phase 3: Spot light
    for(int i = 0; i < nrSpotLights; i++)
    {
        result += CalcSpotLight(u_SpotLights[0], norm, v_Pos, viewDir);   
    }

    color = vec4(result, 1.0);
    //color = vec4(CalcDirLight(u_DirLights[0], norm, viewDir), 1.0);
    //color = vec4(u_DirLights[0].diffuse, 1.0);
    //color = vec4(0.5019f, 0.2902f, 0.7294f, 1.0f);
    //color = vec4(v_Pos, 1.0);
    //color = vec4(vec3(gl_FragCoord.z), 1.0);
}
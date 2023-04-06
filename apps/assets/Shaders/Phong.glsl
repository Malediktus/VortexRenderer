#type vertex

#version 330 core

layout(location = 0) in vec3 v_in_Pos;
layout(location = 1) in vec3 v_in_Normal;
layout(location = 2) in vec2 v_in_TexCoords;

out vec2 v_out_TexCoords;
out vec3 v_out_Normal;
out vec3 v_out_FragPos;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main() {
    v_out_TexCoords = v_in_TexCoords;
    v_out_Normal = mat3(transpose(inverse(u_Model))) * v_in_Normal;
    v_out_FragPos = vec3(u_Model * vec4(v_in_Pos, 1.0));
    gl_Position = u_ViewProj * vec4(v_out_FragPos, 1.0);
}

#type fragment

#version 330 core

#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 4
#define NR_DIRECTIONAL_LIGHTS 4
#define NR_TEXTURE_DIFFUSE 4
#define NR_TEXTURE_SPECULAR 4

struct Material {
    sampler2D texture_diffuse[NR_TEXTURE_DIFFUSE];
    sampler2D texture_specular[NR_TEXTURE_SPECULAR];
    float shininess;

    int NumDiffuseTextures;
    int NumSpecularTextures;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLights[NR_SPOT_LIGHTS];
uniform DirectionalLight u_DirectionalLights[NR_DIRECTIONAL_LIGHTS];
uniform int u_NumPointLights;
uniform int u_NumSpotLights;
uniform int u_NumDirectionalLights;
uniform Material u_Material;
uniform vec3 u_ViewPos;

in vec2 v_out_TexCoords;
in vec3 v_out_Normal;
in vec3 v_out_FragPos;
out vec4 FragColor;

vec4 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(v_out_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_out_FragPos);
    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < u_NumPointLights; i++)
        result += CalcPointLight(u_PointLights[i], norm, v_out_FragPos, viewDir);
    for (int i = 0; i < u_NumSpotLights; i++)
        result += CalcSpotLight(u_SpotLights[i], norm, v_out_FragPos, viewDir);
    for (int i = 0; i < u_NumDirectionalLights; i++)
        result += CalcDirLight(u_DirectionalLights[i], norm, viewDir);

    FragColor = result;
}

vec4 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // combine results
    vec4 ambient = vec4(light.ambient, 1.0);
    for (int i = 0; i < u_Material.NumDiffuseTextures; i++)
        ambient *= texture(u_Material.texture_diffuse[i], v_out_TexCoords);

    vec4 diffuse = vec4(light.diffuse * diff, 1.0);
    for (int i = 0; i < u_Material.NumDiffuseTextures; i++)
        diffuse *= texture(u_Material.texture_diffuse[i], v_out_TexCoords);

    vec4 specular = vec4(light.specular * spec, 1.0);
    for (int i = 0; i < u_Material.NumSpecularTextures; i++)
        specular *= texture(u_Material.texture_specular[i], v_out_TexCoords);

    return (ambient + diffuse + specular);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec4 ambient = vec4(light.ambient, 1.0);
    for (int i = 0; i < u_Material.NumDiffuseTextures; i++)
        ambient *= texture(u_Material.texture_diffuse[i], v_out_TexCoords);

    vec4 diffuse = vec4(light.diffuse * diff, 1.0);
    for (int i = 0; i < u_Material.NumDiffuseTextures; i++)
        diffuse *= texture(u_Material.texture_diffuse[i], v_out_TexCoords);

    vec4 specular = vec4(light.specular * spec, 1.0);
    for (int i = 0; i < u_Material.NumSpecularTextures; i++)
        specular *= texture(u_Material.texture_specular[i], v_out_TexCoords);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec4 ambient = vec4(light.ambient, 1.0);
    for (int i = 0; i < u_Material.NumDiffuseTextures; i++)
        ambient *= texture(u_Material.texture_diffuse[i], v_out_TexCoords);

    vec4 diffuse = vec4(light.diffuse * diff, 1.0);
    for (int i = 0; i < u_Material.NumDiffuseTextures; i++)
        diffuse *= texture(u_Material.texture_diffuse[i], v_out_TexCoords);

    vec4 specular = vec4(light.specular * spec, 1.0);
    for (int i = 0; i < u_Material.NumSpecularTextures; i++)
        specular *= texture(u_Material.texture_specular[i], v_out_TexCoords);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

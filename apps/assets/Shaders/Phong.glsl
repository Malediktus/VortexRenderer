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

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
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

#define NR_POINT_LIGHTS 4

uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform DirectionalLight u_DirectionalLight;
uniform SpotLight u_SpotLight;
uniform Material u_Material;
uniform vec3 u_ViewPos;

in vec2 v_out_TexCoords;
in vec3 v_out_Normal;
in vec3 v_out_FragPos;
out vec4 FragColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(v_out_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_out_FragPos);
    vec3 result = CalcDirLight(u_DirectionalLight, norm, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(u_PointLights[i], norm, v_out_FragPos, viewDir);
    result += CalcSpotLight(u_SpotLight, norm, v_out_FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_out_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_out_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_out_TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_out_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_out_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_out_TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_out_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_out_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_out_TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

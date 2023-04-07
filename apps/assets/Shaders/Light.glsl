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

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_ViewPos;
uniform int u_NumPointLights;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];

in vec2 v_out_TexCoords;
in vec3 v_out_Normal;
in vec3 v_out_FragPos;
out vec4 FragColor;

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(v_out_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_out_FragPos);
    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < u_NumPointLights; i++)
        result += CalcPointLight(u_PointLights[i], norm, v_out_FragPos, viewDir);

    FragColor = result;
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec4 ambient = vec4(light.ambient, 1.0);
    vec4 diffuse = vec4(light.diffuse * diff, 1.0);
    vec4 specular = vec4(light.specular * spec, 1.0);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

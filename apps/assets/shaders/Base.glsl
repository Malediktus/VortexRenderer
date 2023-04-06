#type vertex

#version 330 core

layout(location = 0) in vec3 v_in_Pos;
layout(location = 0) in vec3 v_in_Normals;
layout(location = 0) in vec2 v_in_TexCoords;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(v_in_Pos, 1.0);
}

#type fragment

#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

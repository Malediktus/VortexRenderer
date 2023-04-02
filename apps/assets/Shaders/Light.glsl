#type vertex

#version 330 core

layout(location = 0) in vec3 v_in_Pos;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main() {
    gl_Position = u_ViewProj * u_Model * vec4(v_in_Pos, 1.0f);
}

#type fragment

#version 330 core

uniform vec3 u_Color;

out vec4 FragColor;

void main() {
    FragColor = vec4(u_Color, 1.0f);
}

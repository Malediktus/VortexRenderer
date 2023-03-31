#type vertex

#version 330 core

layout(location = 0) in vec3 v_in_Pos;
layout(location = 1) in vec2 v_in_TexCoords;

out vec2 v_out_TexCoords;

uniform mat4 u_ViewProj;
uniform mat4 u_Model;

void main() {
    v_out_TexCoords = v_in_TexCoords;
    gl_Position = vec4(v_in_Pos, 1.0f) * u_Model * u_ViewProj;
}

#type fragment

#version 330 core

in vec2 v_out_TexCoords;
out vec4 FragColor;

uniform sampler2D u_TextureId;

void main() {
    FragColor = texture(u_TextureId, v_out_TexCoords);
}

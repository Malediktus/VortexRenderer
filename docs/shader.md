# Documentation for Vortex shaders {#shader}

Vortex shader are just glsl with one extra feature:  
They are a single file only. The vertex shader gets introduced by '#type vertex' and the fragment shader by '#type fragment'.
For now only vertex- and fragment shader are supported, but this will change.

Example Base shader:  
Base.glsl:   
```
#type vertex

#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uViewProj;

void main() {
    gl_Position = vec4(aPos, 1.0f) * uViewProj;
}

#type fragment

#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
```

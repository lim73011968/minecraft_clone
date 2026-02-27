#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;
layout(location=2) in vec2 textureCordinates;
layout(location=3) in float textureID;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Perspective;
uniform mat4 u_ViewMatrix;

out vec3 v_vertexColors;
out vec2 v_texture_placement;
out float v_texture_ID;

void main()
{
    v_vertexColors = vertexColors;

    vec4 newPosition = u_Perspective * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);
    v_texture_placement = vec2(textureCordinates.x, textureCordinates.y);
    v_texture_ID = textureID;
    gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);

};

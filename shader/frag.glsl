#version 410 core
in vec3 v_vertexColors;
in vec2 v_texture_placement;
in float v_texture_ID;

//out vec4 color;
out vec4 FragColor;

uniform sampler2D Dirt;
//this is uniform valuer for our texture
uniform sampler2D GrassBlock;
uniform sampler2D GrassBlockTop;
void main()
{
    //disable this for while cause we aint using this
    //color = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0f);
    if (v_texture_ID < 0.5) {
        FragColor = texture(Dirt, v_texture_placement);
    } else if (v_texture_ID < 1.5) {
        FragColor = texture(GrassBlock, v_texture_placement);
    } else {
        FragColor = texture(GrassBlockTop, v_texture_placement);
    }
};

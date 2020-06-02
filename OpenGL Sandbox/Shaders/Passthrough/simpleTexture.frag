#version 330 core

out vec4 color;

in vec2 TexCoords;

//uniform samplerCube renderedTexture;
uniform sampler2D renderedTexture;

void main()
{
    color = texture( renderedTexture, TexCoords);
    
    /*
    vec2 mapCoord = 2.0 * TexCoords - 1.0;
    ////// cubemap debug:
    color = texture( renderedTexture, vec3(1.0, mapCoord.xy));
    //color = texture( renderedTexture, vec3(-1.0, mapCoord.xy));
    //color = texture( renderedTexture, vec3(mapCoord.x, 1.0, mapCoord.y));
    //color = texture( renderedTexture, vec3(mapCoord.x, -1.0, mapCoord.y));
    //color = texture( renderedTexture, vec3(mapCoord.xy, 1.0));
    //color = texture( renderedTexture, vec3(mapCoord.xy, -1.0));
     */
}

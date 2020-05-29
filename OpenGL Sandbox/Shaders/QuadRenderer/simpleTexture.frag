#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D renderedTexture;

void main()
{
    color = texture( renderedTexture, TexCoords);
}


/*
out vec4 color;
in vec2 TexCoords;

uniform sampler2D renderedTexture;

const float aberration = 0.02;

vec4 desaturate(vec3 color, float factor)
{
    vec3 lum = vec3(0.299, 0.587, 0.114);
    vec3 gray = vec3(dot(lum, color));
    return vec4(mix(color, gray, factor), 1.0);
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec4 fragColor = texture( renderedTexture, TexCoords);
    vec2 uv = TexCoords;
    
    // Change aberration with mouse x
    float aberration = 0.1f;
    
    // Get the colors
    vec4 base_col       =     texture( renderedTexture, uv);
    vec2 dist             =     (uv - 0.5) * 2.0;
    vec2 abr_dist        =     length(uv - 0.5) * length(uv - 0.5) * clamp(aberration * dist, -0.1, 0.1);
         base_col.rgb    =     texture( renderedTexture, uv - abr_dist).rgb    * vec3(1.0, 0.0, 0.5) +
                              texture( renderedTexture, uv + abr_dist).rgb    * vec3(0.0, 1.0, 0.5);
 
    base_col = desaturate(base_col.rgb, 0.0);
    
    // Output to screen
    color = base_col;
}
*/

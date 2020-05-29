#version 330 core

in vec2 TexCoords;
out vec4 outColour;

uniform mat4 g_previousViewProjectionMatrix;
uniform mat4 g_ViewProjectionInverseMatrix;
uniform sampler2D renderedTexture;
uniform sampler2D depthTexture;

float numSamples = 5.0f;
void main()
{
    // Get the depth buffer value at this pixel.
    float zOverW = texture(depthTexture, TexCoords).r;
    // H is the viewport position at this pixel in the range -1 to 1.
    vec4 H = vec4(TexCoords.x * 2.0f - 1.0f, (1.0f - TexCoords.y) * 2.0f - 1.0f, zOverW, 1.0f);
    // Transform by the view-projection inverse.
    vec4 D = g_ViewProjectionInverseMatrix * H;
    // Divide by w to get the world position.
    vec4 worldPos = D / D.w;

    // Current viewport position
    vec4 currentPos = H;
    // Use the world position, and transform by the previous view-projection matrix.
    vec4 previousPos = g_previousViewProjectionMatrix * worldPos;
    // Convert to nonhomogeneous points [-1,1] by dividing by w.
    previousPos /= previousPos.w;
    // Use this frame's position and last frame's to compute the pixel velocity.
    vec2 velocity = (currentPos - previousPos).xy/2.f;

    // Get the initial color at this pixel.w
    vec4 color = texture(renderedTexture, TexCoords);
    vec2 texCoord = TexCoords;
    texCoord += velocity;
    for(int i = 1; i < numSamples; ++i, texCoord += velocity) {
        // Sample the color buffer along the velocity vector.
        vec4 currentColor = texture(renderedTexture, texCoord);
        // Add the current color to our color sum.
        color += currentColor;
    }
    // Average all of the samples to get the final blur color.
    
    if (TexCoords.x > 0.5) {
        outColour = texture(renderedTexture, TexCoords);
    } else {
        outColour = worldPos;//color / numSamples;
    }

}

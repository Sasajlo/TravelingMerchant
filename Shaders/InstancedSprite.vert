#version 450 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;
layout(location = 3) in mat4 modelMatrix; // Instance model matrix
layout(location = 7) in float whiten;     // Instance whiten value
layout(location = 8) in vec4 texCoords;   // Instance texture coordinates (left, bottom, right, top)

uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform bool enableSwaying;
uniform float swayAmount;
uniform float swaySpeed;

out vec2 TexCoord;
out float Whiten;

void main()
{
    vec3 finalPos = inPos;

    if (enableSwaying) {
        // Use texture coordinate Y as a normalized height factor
        float heightFactor = -inTexCoord.y;

        // Horizontal sway increases with height
        float sway = sin(time * swaySpeed) * swayAmount * heightFactor;
        finalPos.x += sway;
    }


    gl_Position = projection * view * modelMatrix * vec4(finalPos, 1.0);
    
    // Calculate final texture coordinates based on instance data
    // inTexCoord is (0,0) to (1,1) for the full quad
    // texCoords contains (left, bottom, right, top) for the sprite sheet region
    float texX = mix(texCoords.x, texCoords.z, inTexCoord.x);
    float texY = mix(texCoords.y, texCoords.w, 1.0 - inTexCoord.y);
    TexCoord = vec2(texX, texY);
    
    Whiten = whiten;
}
#version 450 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform bool enableSwaying;
uniform float swayAmount;
uniform float swaySpeed;

out vec2 TexCoord;

void main()
{
    vec3 finalPos = inPos;

    if (enableSwaying) {
        // Use texture coordinate Y as a normalized height factor
        float heightFactor = 1.0 - inTexCoord.y;

        // Horizontal sway increases with height
        float sway = sin(time * swaySpeed) * swayAmount * heightFactor;
        finalPos.x += sway;

        // Optional vertical sway
        float verticalSway = sin(time * swaySpeed * 0.7) 
                            * swayAmount * 0.3 
                            * heightFactor;
        finalPos.y += verticalSway;
    }

    gl_Position = projection * view * model * vec4(finalPos, 1.0);
    TexCoord = inTexCoord;
}
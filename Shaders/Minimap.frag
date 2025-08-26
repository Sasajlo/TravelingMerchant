#version 450 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D imageTex;
uniform vec4 tintColor;

void main()
{
    // Calculate distance from center
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(TexCoord, center);
    
    // Only render if inside circle
    if (dist > 0.5) {
        discard;
    }
    
    vec4 tex = texture(imageTex, TexCoord);
    FragColor = vec4(tex.rgb * tintColor.rgb, tex.a * tintColor.a);
}
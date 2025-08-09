#version 450 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 tintColor;   // default (1,1,1,1)
uniform float whiten;     // 0.0 = no highlight, 0.35-ish = hover

void main()
{
    vec4 tex = texture(texture1, TexCoord);
    vec3 mixed = mix(tex.rgb, vec3(1.0), clamp(whiten, 0.0, 1.0));
    vec3 finalRGB = mixed * tintColor.rgb;
    float finalA = tex.a * tintColor.a;
    FragColor = vec4(finalRGB, finalA);
}
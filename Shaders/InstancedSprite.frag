#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in float Whiten;

uniform sampler2D texture1;
uniform vec4 tintColor;   // default (1,1,1,1)

void main()
{
    vec4 tex = texture(texture1, TexCoord);
    vec3 mixed = mix(tex.rgb, vec3(1.0), clamp(Whiten, 0.0, 1.0));
    vec3 finalRGB = mixed * tintColor.rgb;
    float finalA = tex.a * tintColor.a;
    FragColor = vec4(finalRGB, finalA);
}
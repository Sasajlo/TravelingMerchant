#version 450 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D imageTex;
uniform vec4 tintColor;

void main()
{
    vec4 tex = texture(imageTex, TexCoord);
    FragColor = vec4(tex.rgb * tintColor.rgb, tex.a * tintColor.a);
}
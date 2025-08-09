#version 450 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D fontAtlas;
uniform vec4 textColor;

void main()
{
    // Atlas is R8 with swizzle to RGBA=(1,1,1,R)
    float alpha = texture(fontAtlas, TexCoord).a;
    FragColor = vec4(textColor.rgb, textColor.a * alpha);
}
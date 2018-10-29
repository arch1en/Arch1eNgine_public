#version 330 core

in vec4 VertColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D aTexture;

void main() 
{
    vec4 TextureColor = texture(aTexture, TexCoord);
    bool IsTextureEmpty = 
    TextureColor.r <= .0f && 
    TextureColor.g <= .0f && 
    TextureColor.b <= .0f && 
    TextureColor.a <= .0f;

    if(IsTextureEmpty)
    {
        FragColor = VertColor * TextureColor;
    }
    else
    {
        FragColor = VertColor;
    }
}

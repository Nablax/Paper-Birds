#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;

void main()
{
    float ambient = 0.3f;
    vec3 color = ambient * texture(floorTexture, fs_in.TexCoords).rgb;
    FragColor = vec4(color, 1.0);
}
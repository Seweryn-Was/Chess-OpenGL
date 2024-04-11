#version 430 core
in vec3 bColor;
in vec2 TexCoord;
in vec3 bPos; 
uniform sampler2D ourTexture;


void main()
{
    gl_FragColor = vec4(bColor, 1.0);
}
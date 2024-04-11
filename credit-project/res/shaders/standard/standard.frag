#version 430 core
in vec3 bColor;
in vec2 TexCoord;
in vec3 bPos; 
uniform sampler2D ourTexture;


void main()
{
    if(texture(ourTexture, TexCoord).a <= 0.02){
        discard;
    }
    gl_FragColor = texture(ourTexture, TexCoord);
}
#version 430 core
out vec4 FragColor;
  
in vec3 bColoer;
in vec2 TexCoord;
in vec3 bPos; 


uniform sampler2D ourTexture;

uniform vec3 u_color;
uniform float u_time;
uniform vec2 u_resolution;

float far = 50.0f; 
float near = 0.1f; 

float linearDepth(float depth){

    return (2*near*far)/(far+near-(2.0*depth-1.0)*(far-near));
}

void main()
{
    gl_FragColor = vec4(1.0, 0.0, 0.5, 1.0);
}
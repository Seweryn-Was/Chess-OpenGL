#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;


layout(std140, binding= 0 ) uniform Matrices {
    mat4 projection;
};

out vec3 bColor;
out vec2 TexCoord;
out vec3 bPos; 

uniform mat4 transform ;//mat4(1.0);

void main()
{
    gl_Position = projection * transform * vec4(aPos, 1.0);
    bPos = vec3(aPos);
    bColor = aColor; 
    TexCoord = aUv;

}
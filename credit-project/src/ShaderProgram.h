#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream> //string stream
#include <vector>
#include <sys/stat.h>


bool isPathValid(const char* Filename, struct stat* b);

/*
TO DO:
Geometry shader compatibility
Teseleation control shader compatibility
reselation evaluation shader compatibility

if specific shader wasn not found replace it with shader from standard shader folder
*/


/*
* create program 
* create shader objects (sahder source)
* attach shaders to program 
* link program 
* delete sahderes obj 
* detach shaders obj 
*/


#define FRAGMENT_SHADER			GL_FRAGMENT_SHADER
#define VERTEX_SHADER			GL_VERTEX_SHADER
#define GEOMETRY_SHADER			GL_GEOMETRY_SHADER
#define COMPUTE_SHADER			GL_COMPUTE_SHADER
#define TESSELATION_SHADER		GL_TESS_CONTROL_SHADER
#define TESS_EVALUATION_SHADER	GL_TESS_EVALUATION_SHADER

#define FRAGMENT_SHADER_INDEX			0	
#define VERTEX_SHADER_INDEX				1
#define GEOMETRY_SHADER_INDEX			2	
#define COMPUTE_SHADER_INDEX			3	
#define TESSELATION_SHADER_INDEX		4	
#define TESS_EVALUATION_SHADER_INDEX	5	

#define FRAGMENT_SHADER_EXTENSION			".frag"	
#define VERTEX_SHADER_EXTENSION				".vert"
#define GEOMETRY_SHADER_EXTENSION			".geo"
#define COMPUTE_SHADER_EXTENSION			".compute"	
#define TESSELATION_SHADER_EXTENSION		".tesc"	
#define TESS_EVALUATION_SHADER_EXTENSION	"tese"

#define SHADER_SOURCE_PATH_MAX_LENGTH 300

struct ShaderSource {
	char frag[SHADER_SOURCE_PATH_MAX_LENGTH];
	char vert[SHADER_SOURCE_PATH_MAX_LENGTH];
};
typedef struct ShaderSource ShaderSource;

struct ShaderProgram {
	unsigned int programId;
	unsigned int shaderIdList[2];
	char* shaderPathList[2]; 
	ShaderSource *source; 
};
typedef struct ShaderProgram ShaderProgarm;


void CreateShaderProgram(ShaderProgarm *shader); 
bool LinkProgram(ShaderProgarm* shader);

void UseShaderProgram(ShaderProgarm* program);
void UnUseShaderPrograms(); 

void DeleteShaderProgram(ShaderProgarm* program); 

const char* loadShaderCode(const char* filename); 

void CreateAndAttachShaderObjectsToProgram(ShaderProgram *shader); 
bool CompileShader(unsigned int shaderObjectId); 

void detachShaders(ShaderProgarm* shader); 

#endif
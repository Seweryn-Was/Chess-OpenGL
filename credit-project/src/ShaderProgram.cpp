#include "ShaderProgram.h"

bool isPathValid(const char* Filename, struct stat* b) {
	//"Upon successful completion, 0 is returned. Otherwise, -1 is returned and errno is set to indicate the error."
	return stat(Filename, b) == 0;
}

void CreateShaderProgram(ShaderProgarm* shader) {
	shader->programId = glCreateProgram(); 
    shader->shaderPathList[0] = shader->source->vert; 
    shader->shaderPathList[1] = shader->source->frag; 
    //std::cout << shader->shaderPathList[0] << "\n";
    //std::cout << shader->shaderPathList[1] << "\n";
    CreateAndAttachShaderObjectsToProgram(shader);
    LinkProgram(shader); 
}

void CreateAndAttachShaderObjectsToProgram(ShaderProgram* shader) {
    struct stat sb;
    //checks is passed path is valid
    if (isPathValid(shader->source->frag, &sb) == false || isPathValid(shader->source->vert, &sb) == false) {
        std::cout << "[file] passed paths in Shader Source is invalid\n";
    }
    //checks is passed path is a directory
    if ((sb.st_mode & S_IFDIR) == true) {
        std::cout << "[file] passed path is a direcotry\n";
        return; 
    }

    const char* code;
    //fragment Shader
    code = loadShaderCode(shader->source->frag);
    shader->shaderIdList[FRAGMENT_SHADER_INDEX] = glCreateShader(FRAGMENT_SHADER); 
    if (shader->shaderIdList[FRAGMENT_SHADER_INDEX] == 0) { std::cout << "[OpenGL] Shader Creation was not succesfull\n"; }
    else {
        unsigned int fragId = shader->shaderIdList[FRAGMENT_SHADER_INDEX]; 
        glShaderSource(fragId, 1, &code, NULL); 
        if(CompileShader(fragId)==false)std::cout << "[Fragment shader] compilation failed\n";
        glAttachShader(shader->programId, fragId);
        glDeleteShader(fragId);
    }
    delete code;
    //vertex Shader 
    code = loadShaderCode(shader->source->vert); 
    shader->shaderIdList[VERTEX_SHADER_INDEX] = glCreateShader(VERTEX_SHADER);
    if (shader->shaderIdList[VERTEX_SHADER_INDEX] == 0) { std::cout << "[OpenGL] Shader Creation was not succesfull\n"; }
    else {
        unsigned int vertId = shader->shaderIdList[VERTEX_SHADER_INDEX];
        glShaderSource(vertId, 1, &code, NULL);
        if (CompileShader(vertId) == false)std::cout << "[Vertex shader] compilation failed\n";
        glAttachShader(shader->programId, vertId);
        glDeleteShader(vertId);
    }
    delete code; 
}

void UseShaderProgram(ShaderProgarm* shader) {
    glUseProgram(shader->programId); 
}

void UnUseShaderPrograms() {
    glUseProgram(0); 
}

void DeleteShaderProgram(ShaderProgarm* shader) {
    glDeleteProgram(shader->programId); 
    shader->programId = 0; 
}



bool LinkProgram(ShaderProgarm* shader) {
    glLinkProgram(shader->programId);
    GLint isLinked = GL_FALSE;
    glGetProgramiv(shader->programId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        std::cout << "[OpenGL] GL_LINK_STATUS : " << isLinked << "\n\tprogram was not linked\n";

        GLint maxlength = 0;
        glGetProgramiv(shader->programId, GL_INFO_LOG_LENGTH, &maxlength);
        if (maxlength == 0) {
            std::cout << "\tinformation log is empty\n";
            return false;
        }
        std::vector<GLchar> infolog(maxlength);
        glGetProgramInfoLog(shader->programId, maxlength, &maxlength, &infolog[0]);
        std::cout << "\tinfolog:\n\t" << infolog.data() << "\n";
        return false; 
    }
    detachShaders(shader);
    return true; 
}

void detachShaders(ShaderProgarm* shader) {
    glDetachShader(shader->programId, shader->shaderIdList[FRAGMENT_SHADER_INDEX]); 
    glDetachShader(shader->programId, shader->shaderIdList[VERTEX_SHADER_INDEX]); 
}

bool CompileShader(unsigned int shaderObjectId) {
    glCompileShader(shaderObjectId);
    /*
    checking if compilation was succesfull
    if wasn't then getting shader info log and displaying it
    after that deleting shader on GPU
    */
    GLint isCompiled = GL_FALSE;
    glGetShaderiv(shaderObjectId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxlength = 0;
        glGetShaderiv(shaderObjectId, GL_INFO_LOG_LENGTH, &maxlength);

        std::vector<GLchar> infolog(maxlength);
        glGetShaderInfoLog(shaderObjectId, maxlength, &maxlength, &infolog[0]);

        std::cout << "[Shader]Shader compilation failed:" << "\nERROR LIST : \n" << infolog.data();
        glDeleteShader(shaderObjectId);
        return false; 
    }
    return true; 
}

const char* loadShaderCode(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "[file load]Error: Failed to open the file." << std::endl;
        return "";
    }

    std::streamsize size = file.tellg();
    if (size == -1) {
        std::cerr << "[file load]Error: Failed to get the file size." << std::endl;
        return "";
    }

    char* buffer = new char[size+1];

    file.seekg(0, std::ios::beg);
    if (!file.read(buffer, size)) {
        std::cerr << "[file load]Error: Failed to read the file." << std::endl;
        delete[] buffer; 
        return "";
    }

    file.close();


    buffer[size] = '\0';

    const char* data = buffer;
    //std::cout << "File content as const char*: \n" << data << std::endl;
    //delete[] buffer;
	return data;
}
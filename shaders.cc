// shader handling
// Author: Supakorn "Jamie" Rassameemasmuang

#include "common.h"

#ifdef HAVE_GL

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include "shaders.h"

GLuint createShaders(GLchar const* src, int shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        GLint length; 

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> msg(length);

        glGetShaderInfoLog(shader, length, &length, msg.data());

        size_t n=msg.size();
        for(size_t i=0; i < n; ++i) 
        {
          std::cerr << msg[i];
        }

        std::cerr << std::endl << "GL Compile error" << std::endl;
        std::cerr << src << std::endl;
        throw 1; 
    }
    return shader;
}

GLuint createShaderFile(std::string file, int shaderType, size_t Nlights,
                        size_t Nmaterials, std::vector<std::string> const& defineflags, bool explicitcolor)
{
    std::ifstream shaderFile;
    shaderFile.open(file.c_str());
    std::stringstream shaderSrc;

    shaderSrc << "#version 450" << "\r\n";

    // I'm using C++11/14 notation for now, if does not work with Karl
    // we can change it.
    // Ideally, we should be using unordered_set but then Karl has some problems...

    for (std::string const& flag : defineflags) {
        shaderSrc << "#define " << flag << "\r\n";
    }

    
    if(explicitcolor)
      shaderSrc << "#define EXPLICIT_COLOR" << "\r\n";
    
    shaderSrc << "const int Nlights=" << Nlights << ";\r\n";
    shaderSrc << "const int Nmaterials=" << Nmaterials << ";\r\n";

    if (shaderFile)
    {
        shaderSrc << shaderFile.rdbuf();
        shaderFile.close();
    }
    else
    {
        throw 1;
    }

    return createShaders(shaderSrc.str().data(), shaderType);
}
#endif

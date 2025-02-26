#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;
 


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. 从文件路径中获取顶点/片段着色器
   ifstream vertexFile;
   ifstream fragmentFile;
   stringstream vertexSStream;
   stringstream fragmentSStream;

   vertexFile.open(vertexPath);
   fragmentFile.open(fragmentPath);
   // 保证ifstream对象可以抛出异常：
   vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
   fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);

   try
   {
       // 打开文件
       if (!vertexFile.is_open()||!fragmentFile.is_open())
           throw exception("open file error");
       // 读取文件的缓冲内容到数据流中
       vertexSStream << vertexFile.rdbuf();
       fragmentSStream << fragmentFile.rdbuf();
       // 关闭文件处理器
       vertexFile.close();
       fragmentFile.close();
       // 转换数据流到string
       vertexString = vertexSStream.str();
       fragmentString = fragmentSStream.str();

       vertexSource = vertexString.c_str();
       fragmentSource = fragmentString.c_str();

       unsigned int vertex, fragment;
       vertex = glCreateShader(GL_VERTEX_SHADER);
       glShaderSource(vertex, 1, &vertexSource, NULL);
       glCompileShader(vertex); //从源代码转成二进制
       checkCompileErrors(vertex, "VERTEX");


       fragment = glCreateShader(GL_FRAGMENT_SHADER);
       glShaderSource(fragment, 1, &fragmentSource, NULL);
       glCompileShader(fragment); //从源代码转成二进制
       checkCompileErrors(fragment, "FRAGMENT");


       // 着色器程序
       ID = glCreateProgram();
       glAttachShader(ID, vertex);
       glAttachShader(ID, fragment);
       glLinkProgram(ID); //编译把两个二进制代码链接起来
       checkCompileErrors(ID, "PROGRAM");

       glDeleteShader(vertex);
       glDeleteShader(fragment);



   }
   catch (const std::exception& ex)
   {
       printf(ex.what());

   }

}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::checkCompileErrors(unsigned int ID, std::string type)
{
    int success;
    char infoLog[512];
    if (type != "PROGRAM")
    {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
            std::cout << "shader compile error: " << infoLog <<endl;
        }
    }
    else
    {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "program link error " << infoLog << endl;
        }
    }
}

 
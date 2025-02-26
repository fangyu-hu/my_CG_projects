#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;
 


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
   ifstream vertexFile;
   ifstream fragmentFile;
   stringstream vertexSStream;
   stringstream fragmentSStream;

   vertexFile.open(vertexPath);
   fragmentFile.open(fragmentPath);
   // ��֤ifstream��������׳��쳣��
   vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
   fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);

   try
   {
       // ���ļ�
       if (!vertexFile.is_open()||!fragmentFile.is_open())
           throw exception("open file error");
       // ��ȡ�ļ��Ļ������ݵ���������
       vertexSStream << vertexFile.rdbuf();
       fragmentSStream << fragmentFile.rdbuf();
       // �ر��ļ�������
       vertexFile.close();
       fragmentFile.close();
       // ת����������string
       vertexString = vertexSStream.str();
       fragmentString = fragmentSStream.str();

       vertexSource = vertexString.c_str();
       fragmentSource = fragmentString.c_str();

       unsigned int vertex, fragment;
       vertex = glCreateShader(GL_VERTEX_SHADER);
       glShaderSource(vertex, 1, &vertexSource, NULL);
       glCompileShader(vertex); //��Դ����ת�ɶ�����
       checkCompileErrors(vertex, "VERTEX");


       fragment = glCreateShader(GL_FRAGMENT_SHADER);
       glShaderSource(fragment, 1, &fragmentSource, NULL);
       glCompileShader(fragment); //��Դ����ת�ɶ�����
       checkCompileErrors(fragment, "FRAGMENT");


       // ��ɫ������
       ID = glCreateProgram();
       glAttachShader(ID, vertex);
       glAttachShader(ID, fragment);
       glLinkProgram(ID); //��������������ƴ�����������
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

 
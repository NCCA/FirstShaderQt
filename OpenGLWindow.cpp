/*
 * Basic GL Window modified from the example here
 * http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
 * adapted to use NGL
 */
#include "OpenGLWindow.h"
#include <QKeyEvent>
#include <QApplication>
#include <iostream>


void OpenGLWindow::printInfoLog(const GLuint &_obj  )
{
  GLint infologLength = 0;
  GLint charsWritten  = 0;
  char *infoLog;

  glGetShaderiv(_obj, GL_INFO_LOG_LENGTH,&infologLength);
  std::cerr<<"info log length "<<infologLength<<"\n";
  if(infologLength > 0)
  {
    infoLog = new char[infologLength];
    glGetShaderInfoLog(_obj, infologLength, &charsWritten, infoLog);

    std::cerr<<infoLog<<std::endl;
    delete [] infoLog;
    glGetShaderiv(_obj, GL_COMPILE_STATUS,&infologLength);
    if( infologLength == GL_FALSE)
    {
      std::cerr<<"Shader compile failed or had warnings \n";
     // exit(EXIT_FAILURE);
    }
  }

}

void OpenGLWindow::createQuad()
{
  // a simple quad object
  float* vert = new float[18];	// vertex array
  const static float s=0.4;
  vert[0] =-s; vert[1] =  s; vert[2] =-1.0;
  vert[3] = s; vert[4] =  s; vert[5] =-1.0;
  vert[6] = -s; vert[7] = -s; vert[8]= -1.0;

  vert[9] =-s; vert[10]= -s; vert[11]=-1.0;
  vert[12] =s; vert[13]= -s; vert[14]=-1.0;
  vert[15] =s; vert[16]= s; vert[17]=-1.0;
  // allocate a VertexArray
  glGenVertexArrays(1, &m_vaoID);
  // now bind a vertex array object for our verts
  glBindVertexArray(m_vaoID);
  // now we are going to bind this to our vbo
  GLuint vboID;
  glGenBuffers(1, &vboID);
  // now bind this to the VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  // allocate the buffer datra
  glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), vert, GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  delete [] vert;
}

void OpenGLWindow::createShader()
{
 // here we create a program
 m_shaderID=glCreateProgram();
 // some raw data for our vertex shader
 const std::string vertex =
 R"DELIM(
          #version 400 core

          layout (location = 0) in vec3  inPosition;
          layout (location = 1) in vec3 inColour;
          out vec3 vertColour;
          void main()
          {
             gl_Position = vec4(inPosition, 1.0);
             if (inPosition.x >0.0 && inPosition.y<0.5)
              vertColour = inColour;
             else vertColour=vec3(1,1,1);
          }
  )DELIM";
  // create a Vertex shader object
  GLuint vertexID=glCreateShader(GL_VERTEX_SHADER);
  // attatch the shader source we need to convert to GL format
  const char* source=vertex.c_str();
  glShaderSource(vertexID,1,&source,NULL);
  // now compile the shader
  glCompileShader(vertexID);
  std::cerr<<"compiling vertex shader\n";
  printInfoLog(vertexID);

  // some source for our fragment shader
  const std::string fragment =
  R"DELIM(
          #version 400 core
          in vec3 vertColour;
          out vec4 fragColour;
          void main()
          {
            fragColour = vec4(vertColour,1.0);
          }
  )DELIM";
// now create a fragment shader
  GLuint fragmentID=glCreateShader(GL_FRAGMENT_SHADER);
  // attatch the shader source
  source=fragment.c_str();
  glShaderSource(fragmentID,1,&source,NULL);
  // compile the shader
  std::cerr<<"compiling frag shader shader\n";
  glCompileShader(fragmentID);
  printInfoLog(fragmentID);
  // now attach to the program object
  glAttachShader(m_shaderID,vertexID);
  glAttachShader(m_shaderID,fragmentID);
  glBindAttribLocation(m_shaderID,0,"inPosition");
  glBindAttribLocation(m_shaderID,1,"inColour");

  // link the program
  glLinkProgram(m_shaderID);
  // and enable it for use
  glUseProgram(m_shaderID);
  // now tidy up the shaders as we don't need them
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
}

OpenGLWindow::OpenGLWindow()
{
  setTitle("First Shaders in OpenGL 3.2");

}

OpenGLWindow::~OpenGLWindow()
{
  glDeleteProgram(m_shaderID);
}



void OpenGLWindow::initializeGL()
{
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background

  std::cerr << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

  // create the Quad
  createQuad();
  // load the shaders
  createShader();
}


void OpenGLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // set the first attrib (1) to 1,0,0 red
  //glVertexAttrib3f(1,1,0,0);
  glBindVertexArray(m_vaoID);		// select first bind the array
  glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object
}

void OpenGLWindow::resizeGL(QResizeEvent *_event)
{
}

void OpenGLWindow::resizeGL(int _w , int _h)
{
}

void OpenGLWindow::keyPressEvent(QKeyEvent *_event)
{
  switch (_event->key())
  {
   case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
   case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
   case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
   case Qt::Key_1 : glVertexAttrib3f(1,1,0,0); break;
   case Qt::Key_2 : glVertexAttrib3f(1,0,1,0); break;
   case Qt::Key_3 : glVertexAttrib3f(1,0,0,1); break;
  }
  update();
}

/*
 * Basic GL Window modified from the example here
 * http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
 * adapted to use NGL
 */
#include "OpenGLWindow.h"
#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
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
  glGenBuffers(1, &m_vboID);
  // now bind this to the VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
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
 const char *vertex[]={
                 "#version 400 core \n"

                 "layout (location = 0) in vec3  inPosition; \n"
                 "layout (location = 1) in vec3 inColour; \n"
                 "out vec3 vertColour; \n"
                 "void main() \n"
                 "{ \n"
                 "   gl_Position = vec4(inPosition, 1.0); \n"
                 "   if (inPosition.x >0.0 && inPosition.y<0.5) \n"
                 "    vertColour = inColour; \n"
                 "   else vertColour=vec3(1,1,1); \n"
                 "} \0"};
  // create a Vertex shader object
  GLuint vertexID=glCreateShader(GL_VERTEX_SHADER);
  // attatch the shader source
  glShaderSource(vertexID,1,&vertex[0],NULL);
  // now compile the shader
  glCompileShader(vertexID);
  std::cerr<<"compiling vertex shader\n";
  printInfoLog(vertexID);

  // some source for our fragment shader
  const char *fragment[]={
                  "#version 400 core \n"
                  "in vec3 vertColour; \n"
                  "out vec4 fragColour; \n"
                  "void main() \n"
                  "{ \n"
                  "fragColour = vec4(vertColour,1.0); \n"
                  "} \0"};
  // now create a fragment shader
  GLuint fragmentID=glCreateShader(GL_FRAGMENT_SHADER);
  // attatch the shader source
  glShaderSource(fragmentID,1,&fragment[0],NULL);
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

OpenGLWindow::OpenGLWindow(QWindow *_parent)
    : QWindow(_parent)
    , m_context(0)
    , m_device(0)
{
  // ensure we render to OpenGL and not a QPainter by setting the surface type
  setSurfaceType(QWindow::OpenGLSurface);

  setTitle("First Shaders in OpenGL 3.2");

}

OpenGLWindow::~OpenGLWindow()
{
  // now we have finished clear the device
  delete m_device;
  glDeleteProgram(m_shaderID);
}



void OpenGLWindow::initialize()
{
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background

  std::cerr << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

  // create the Quad
  createQuad();
  // load the shaders
  createShader();
}


void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
  // don't use the event
  Q_UNUSED(event);
  // if the window is exposed (visible) render
  if (isExposed())
  {
    render();
  }
}


void OpenGLWindow::render()
{
  // if this is the first time round init the GL context this will only be called once
  if (!m_context)
  {

    m_context = new QOpenGLContext(this);
    m_context->setFormat(requestedFormat());
    m_context->create();
    m_context->makeCurrent(this);
    // now call the int method in our child class to do all the one time GL init stuff
    initialize();
  }

  m_context->makeCurrent(this);
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // set the first attrib (1) to 1,0,0 red
  glVertexAttrib3f(1,1,0,0);
  glBindVertexArray(m_vaoID);		// select first bind the array
  glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object
  // finally swap the buffers to make visible
  m_context->swapBuffers(this);
}


void OpenGLWindow::keyPressEvent(QKeyEvent *_event)
{
  switch (_event->key())
  {
   case Qt::Key_Escape : QApplication::exit(EXIT_SUCCESS); break;
   case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
   case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;

  }
  render();
}

#ifndef OPENGLWINDOW_H__
#define OPENGLWINDOW_H__

#include <QOpenGLWindow>

//----------------------------------------------------------------------------------------------------------------------
/// @class OpenGLWindow
/// @file OpenGLWindow.h
/// @brief this is the base class for all our OpenGL widgets, inherit from this class and overide the methods for
/// OpenGL drawing modified from the Qt demo here  http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
//----------------------------------------------------------------------------------------------------------------------



class OpenGLWindow : public QOpenGLWindow
{
  // need to tell Qt to run the MOC
  Q_OBJECT
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for OpenGL window must set the surface type to OpenGL
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    explicit OpenGLWindow();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor, remember to remove the device once finished
    //----------------------------------------------------------------------------------------------------------------------

    ~OpenGLWindow();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief render method called every update
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pure virtual initialize method we override in our base class to do our drawing
    /// this is only called one time, just after we have a valid GL context use this to init any global GL elements
    //----------------------------------------------------------------------------------------------------------------------
   void initializeGL();
   //----------------------------------------------------------------------------------------------------------------------
   /// @brief this is called everytime we want to draw the scene
   //----------------------------------------------------------------------------------------------------------------------
   // Qt 5.5.1 must have this implemented and uses it
   void resizeGL(QResizeEvent *_event);
   // Qt 5.x uses this instead! http://doc.qt.io/qt-5/qopenglwindow.html#resizeGL
   void resizeGL(int _w, int _h);


  private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief process key events
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our vertex array object pointer
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_vaoID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our vertex buffer object pointer
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_vboID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our shader id
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_shaderID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to create our quad
    //----------------------------------------------------------------------------------------------------------------------
    void createQuad();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create our shader
    //----------------------------------------------------------------------------------------------------------------------
    void createShader();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create our shader
    //----------------------------------------------------------------------------------------------------------------------
    void printInfoLog(const GLuint &_obj  );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief window width
    //----------------------------------------------------------------------------------------------------------------------
    int m_width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief window height
    //----------------------------------------------------------------------------------------------------------------------
    int m_height;

};

#endif

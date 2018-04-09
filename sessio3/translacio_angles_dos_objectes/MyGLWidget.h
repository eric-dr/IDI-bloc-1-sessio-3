#define GLM_FORCE_RADIANS

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

  private:
    void createBuffers ();
    void carregaShaders ();
    void keyPressEvent(QKeyEvent *e);
    void modelTransform1();
    void modelTransform2();
    
    float scl;
    float x,y1, y2;
    float angle1, angle2;
    float despla;
    glm::mat4 TG;
    
    
    // attribute locations
    GLuint vertexLoc, colorLoc, varLoc, transLoc, vertexLoc2, colorLoc2, transLoc2;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VAO2, VBO, VBO2;
};

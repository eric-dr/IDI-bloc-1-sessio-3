#include "MyGLWidget.h"

#include <iostream>


MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  scl = 0.5;
  x = 0.0;
  y1 = 0.0;
  y2 = 0.0;
  angle1 = 0.0;
  angle2 = 0.0;
  despla = M_PI/6.0;
  glUniform1f(varLoc,scl);	//inicialització tamany vertexs
  modelTransform1();		//inicialitzacio pos caseta
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
  // Activem l'Array a pintar 
  glBindVertexArray(VAO);
  modelTransform1();
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
 
  
  
  //OBJECTE 2

  // Activem l'Array a pintar 
  glBindVertexArray(VAO2);
  modelTransform2();
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
 
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices[5];  // Tres vèrtexs amb X, Y i Z
  Vertices[3] = glm::vec3(-0.5, 0.25, 0.0);
  Vertices[4] = glm::vec3(0.0, 0.75, 0.0);
  Vertices[2] = glm::vec3(0.5, 0.25, 0.0); 
  Vertices[0] = glm::vec3(0.5, -0.5, 0.0);
  Vertices[1] = glm::vec3(-0.5, -0.5, 0.0);
  
  glm::vec3 Color[5];  
  Color[3] = glm::vec3(0.0, 0.0, 1.0);
  Color[4] = glm::vec3(0.0, 1.0, 0.0);
  Color[2] = glm::vec3(1.0, 0.0, 0.0); 
  Color[0] = glm::vec3(0.0, 1.0, 1.0);
  Color[1] = glm::vec3(1.0, 1.0, 0.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Creació del buffer amb les dades dels colors
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Color), Color, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per color	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  // Desactivem el VAO
  glBindVertexArray(0);
  
  
  
  
  //OBJECTE 2
  
  glm::vec3 Vertices2[5];  // Tres vèrtexs amb X, Y i Z
  Vertices2[0] = glm::vec3(-0.5, 0.25, 0.0);
  Vertices2[1] = glm::vec3(-1.0, 0.25, 0.0);
  Vertices2[2] = glm::vec3(-0.5, 0.75, 0.0); 
  Vertices2[3] = glm::vec3(-1.0, 0.75, 0.0);
  Vertices2[4] = glm::vec3(-0.75, 0.90, 0.0);
  
  glm::vec3 Color2[5];  
  Color2[3] = glm::vec3(0.0, 1.0, 1.0);
  Color2[4] = glm::vec3(1.0, 1.0, 0.0);
  Color2[2] = glm::vec3(0.0, 0.0, 1.0); 
  Color2[0] = glm::vec3(1.0, 1.0, 1.0);
  Color2[1] = glm::vec3(0.0, 0.0, 0.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc2, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc2);
  
  // Creació del buffer amb les dades dels colors
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Color2), Color2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per color	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  varLoc = glGetUniformLocation (program->programId(), "val");
  transLoc = glGetUniformLocation (program->programId(), "TG");

}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
  makeCurrent();
  switch(e->key()) {
    case Qt::Key_S:
      scl += 0.1;
      glUniform1f(varLoc, scl);
      break;
    case Qt::Key_D:
      scl -= 0.1;
      glUniform1f(varLoc, scl);
      break;
    case Qt::Key_P:
      angle1 += despla;
      angle2 -= despla;
      break;
    default: e->ignore();
  }
  update();	//envia a pintar al paintGL
}

void MyGLWidget::modelTransform1() {
  glm::mat4 TG(1.0);
  TG = glm::translate(TG, glm::vec3(0.0, 0.0, 0.0));
  TG = glm::rotate(TG, angle1, glm::vec3(0.0, 0.0, 1.0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransform2() {
  glm::mat4 TG(1.0);
  
  TG = glm::translate(TG, glm::vec3(-0.75, 0.575, 0.0));
  TG = glm::rotate(TG, angle2, glm::vec3(0.0, 0.0, 1.0));
  TG = glm::translate(TG, glm::vec3(0.75,-0.575, 0.0));
  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}
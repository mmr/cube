/*
 * $Id: cube.c,v 1.5 2006/01/12 22:32:38 mmr Exp $
 * <mmr@b1n.org> - 2004-03-04 03:31 BRST -
 */

#include <time.h>
#include <stdlib.h>

#ifdef __APPLE__
  #include  <OpenGL/gl.h>
  #include  <OpenGL/glu.h>
  #include  <GLUT/glut.h>
#else
  #include  <GL/gl.h>
  #include  <GL/glu.h>
  #include  <GL/glut.h>
#endif
 
#define MAX_VELOCITY 10
#define B1N_TIME 1
 
typedef struct {
  char r, g, b; 
} b1n_color;
 
typedef struct {
  float p;
  float n;
  float t;
  float x;
  float y;
  float xv;
  float yv;
  b1n_color c[24];
  GLboolean animate;
} b1n_cube;
 
void b1n_display(void);
void b1n_init(void);
void b1n_keyboard(unsigned char, int, int);
void b1n_reshape(int, int);
void b1n_drawCube(b1n_cube);
void b1n_idle(void);
void b1n_timer(int value);
void b1n_setColor(b1n_color, float);
 
b1n_cube cube1;
b1n_cube cube2;
 
int
main(int argc, char **argv)
{
  glutInitWindowSize(800, 600);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("CompuGlobalMegaNet"); // CompuGlobalMegaNet = Empresa do Homer
 
  b1n_init();
 
  glutReshapeFunc(b1n_reshape);
  glutKeyboardFunc(b1n_keyboard);
  glutDisplayFunc(b1n_display);
  glutIdleFunc(b1n_idle);
  //glutTimerFunc(B1N_TIME, b1n_timer, B1N_TIME);

  glutMainLoop();

  exit(1);
}
 
void
b1n_keyboard(unsigned char key, int x, int y)
{
  switch(key){
  case '1':
    cube1.animate = !cube1.animate;
    break;
  case '2':
    cube2.animate = !cube2.animate;
    break;
  case 'Q':
  case 'q':
  case 27:
    exit(1);
  }
  glutPostRedisplay();
}
 
void
b1n_init(void)
{
  int i;
  srand(time(NULL));
 
  cube1.p = 1; 
  cube1.n = -1; 
  cube1.x = 0;
  cube1.y = 0;
  cube1.t = 0.5;
  cube1.xv = MAX_VELOCITY * (GLfloat)rand()/(GLfloat)RAND_MAX;
  cube1.yv = MAX_VELOCITY * (GLfloat)rand()/(GLfloat)RAND_MAX;
  cube1.animate = GL_FALSE;
  for(i=0;i<24;i++){
    cube1.c[i].r = 255*(GLfloat)rand()/(GLfloat)RAND_MAX;
    cube1.c[i].g = 255*(GLfloat)rand()/(GLfloat)RAND_MAX;
    cube1.c[i].b = 255*(GLfloat)rand()/(GLfloat)RAND_MAX;
  }
 
  cube2.p = 0.5; 
  cube2.n = -0.5; 
  cube2.x = 0;
  cube2.y = 0;
  cube2.t = 0.3;
  cube2.xv = MAX_VELOCITY * (GLfloat)rand()/(GLfloat)RAND_MAX;
  cube2.yv = MAX_VELOCITY * (GLfloat)rand()/(GLfloat)RAND_MAX;
  cube1.animate = GL_FALSE;
  for(i=0;i<24;i++){
    cube2.c[i].r = 255*(GLfloat)rand()/(GLfloat)RAND_MAX;
    cube2.c[i].g = 255*(GLfloat)rand()/(GLfloat)RAND_MAX;
    cube2.c[i].b = 255*(GLfloat)rand()/(GLfloat)RAND_MAX;
  }
 
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.1, 0.1, 0.0);
}
 
void b1n_idle(void)
{
  if(cube1.animate){
    cube1.x += cube1.xv;
    cube1.y += cube1.yv;
  }
  if(cube2.animate){
    cube2.x -= cube2.xv;
    cube2.y -= cube2.yv;
  }
  if(cube1.x < 0){
    cube1.x += 360;
  }
  if(cube1.x > 360){
    cube1.x -= 360;
  }
  if(cube1.y < 0){
    cube1.y += 360;
  }
  if(cube1.y > 360){
    cube1.y -= 360;
  }
 
  if(cube2.x < 0){
    cube2.x += 360;
  }
  if(cube2.x > 360){
    cube2.x -= 360;
  }
  if(cube2.y < 0){
    cube2.y += 360;
  }
  if(cube2.y > 360){
    cube2.y -= 360;
  }
  glutPostRedisplay();
}

void b1n_timer(int value)
{
  glutTimerFunc(value, b1n_timer, value);
  b1n_idle();
}

void
b1n_display(void)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  /* Cube2 - solid */
  glPushMatrix();
  glRotatef(cube2.x, 1.0, 0.0, 0.0);
  glRotatef(cube2.y, 0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
    b1n_drawCube(cube2);
  glEnd();
  glPopMatrix();
 
  /* Cube1 - translucid */
  glPushMatrix();
  glDepthMask(GL_FALSE); 
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glRotatef(cube1.x, 1.0, 0.0, 0.0);
  glRotatef(cube1.y, 0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
    b1n_drawCube(cube1);
  glEnd();
  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glPopMatrix();
 
  glutSwapBuffers();
}

void 
b1n_reshape(GLint w, GLint h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
}

/*
void 
b1n_reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 100.0);
 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}
*/
 
void b1n_setColor(b1n_color c, float t){
  glColor4d(c.r, c.g, c.b, t);
}
 
void
b1n_drawCube(b1n_cube c)
{
  b1n_setColor(c.c[0], c.t);
  glVertex3f(c.n, c.p, c.n);
  b1n_setColor(c.c[1], c.t);
  glVertex3f(c.p, c.p, c.n);
  b1n_setColor(c.c[2], c.t);
  glVertex3f(c.p, c.n, c.n);
  b1n_setColor(c.c[3], c.t);
  glVertex3f(c.n, c.n, c.n);
 
  b1n_setColor(c.c[4], c.t);
  glVertex3f(c.n, c.p, c.p);
  b1n_setColor(c.c[5], c.t);
  glVertex3f(c.p, c.p, c.p);
  b1n_setColor(c.c[6], c.t);
  glVertex3f(c.p, c.n, c.p);
  b1n_setColor(c.c[7], c.t);
  glVertex3f(c.n, c.n, c.p);
 
  b1n_setColor(c.c[8], c.t);
  glVertex3f(c.n, c.p, c.n);
  b1n_setColor(c.c[9], c.t);
  glVertex3f(c.p, c.p, c.n);
  b1n_setColor(c.c[10], c.t);
  glVertex3f(c.p, c.p, c.p);
  b1n_setColor(c.c[11], c.t);
  glVertex3f(c.n, c.p, c.p);
 
  b1n_setColor(c.c[12], c.t);
  glVertex3f(c.p, c.p, c.n);
  b1n_setColor(c.c[13], c.t);
  glVertex3f(c.p, c.n, c.n);
  b1n_setColor(c.c[14], c.t);
  glVertex3f(c.p, c.n, c.p);
  b1n_setColor(c.c[15], c.t);
  glVertex3f(c.p, c.p, c.p);
 
  b1n_setColor(c.c[16], c.t);
  glVertex3f(c.p, c.n, c.n);
  b1n_setColor(c.c[17], c.t);
  glVertex3f(c.n, c.n, c.n);
  b1n_setColor(c.c[18], c.t);
  glVertex3f(c.n, c.n, c.p);
  b1n_setColor(c.c[19], c.t);
  glVertex3f(c.p, c.n, c.p);
 
  b1n_setColor(c.c[20], c.t);
  glVertex3f(c.n, c.n, c.n);
  b1n_setColor(c.c[21], c.t);
  glVertex3f(c.n, c.p, c.n);
  b1n_setColor(c.c[22], c.t);
  glVertex3f(c.n, c.p, c.p);
  b1n_setColor(c.c[23], c.t);
  glVertex3f(c.n, c.n, c.p);
}

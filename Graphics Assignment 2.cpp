#include<iostream>
#include<stdlib.h>

#include<GL/glut.h>

using namespace std;

double rotate_by_key=0;  //variable to track rotation position
double scale_x=0.5;		//variable to track scaling position
double translate_y=0.5; //variable to track translation position

void keyPress(int key,int x,int y)
{

    if(key==27)
            exit(0);
    if(key==GLUT_KEY_RIGHT)       //rotating to the right
        rotate_by_key+=5;
    if(key==GLUT_KEY_LEFT)        //rotating to the left
        rotate_by_key-=5;
	if (key == GLUT_KEY_UP)       //scaling in
            scale_x += .05;
        if (key == GLUT_KEY_DOWN)  //scaling out
            scale_x -= .05;
		if (key == GLUT_KEY_PAGE_UP)   //translating right
            translate_y += .05;
        if (key == GLUT_KEY_PAGE_DOWN)  //translating left
            translate_y -= .05;
		//if (key == GLUT_KEY_PAGE_DOWN)
		//{rotate_by_key += 5; translate_y -= .05;}
    glutPostRedisplay();
   
}


   

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
}


void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION); 
    
    glLoadIdentity(); 
    gluPerspective(0.0,(double)w / (double)h,1.0,20.0);  //used to change the position and angle of the camera
}

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   
    glMatrixMode(GL_MODELVIEW);
   
    glLoadIdentity();   
  // glScalef( rotate_x,rotate_x,1.0f );
    glScalef(1.0f,1.0f,scale_x);       //scaling the triangle
   glTranslatef(translate_y,0.5f,0.5f);    //translating the triangle along the x axis currently but can be used with any axis
    glRotatef(  rotate_by_key,-1.0f, 0.5f, -5.0f );  //rotating the triangle about the x axis currently but can be used with any axis
    glBegin(GL_TRIANGLES);
   
        glVertex3f(-0.5f, 0.5f, -1.0f);				//the position of the triangle to begin with
        glVertex3f(-1.0f, 1.5f, -3.0f);
        glVertex3f(-1.5f, 0.5f, -5.0f);
   
        //glVertex3f(-0.5f, 0.5f, -1.0f);
        //glVertex3f(-1.0f, 1.5f, -3.0f);      attempted to draw multiple triangles
        //glVertex3f(-1.5f, 0.5f, -5.0f);

    glEnd();

    glutSwapBuffers();
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
   
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
   
    glutInitWindowSize(800,600);
   
    glutCreateWindow("Hello Triangle");
   
    initRendering();
   
    glutDisplayFunc(drawScene);
    glutSpecialFunc(keyPress);
	
    glutReshapeFunc(handleResize);

    glutMainLoop();
   
    return(0);
}
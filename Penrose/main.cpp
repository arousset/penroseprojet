/* Les escaliers de Penrose                     */
/*                                              */
/* Auteurs:     Rousset Alban                   */
/*              Kawczak Clément                 */
/*                                              */
/* Informatique graphique                       */

#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Rotation de la scene
static float rx = 39.0F;
static float ry = 513.5F;
static float rz = 0.0F;

// Couleurs
static const float vert[]        = { 0.0F,1.0F,0.0F,1.0F };
static const float bleu[]        = { 0.0F,0.0F,1.0F,1.0F };
static const float gris[]   = { 0.8F,0.8F,0.8F,1.0F };
static const float defaultDiff[] = { 0.8F,0.8F,0.8F,1.0F };

// Mode fil de fer
static int aff = 1;

// Vitesse de la balle
float speed = 0.04;

// Variables utiles
float tmp = -0.25;
float tmp2 = 0.0;
int cptface = 0;
bool pause = false;
bool cheat = false;
static float posx = -1.0;
static float posy = 0.8;
static float posz = 0.0;
float angle = 0.0;
static int px = 0;
static int py = 0;

void init(void) {
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_NORMALIZE);
        glEnable(GL_AUTO_NORMAL);
}

void face(int x)
{
        int y = 2;
        for(int i = 0;i < x;i++)
        {
                glPushMatrix();
                for(int j = 0;j < 1;j++)
                {
                        glutSolidCube(1.0);
                        glTranslatef(0.0, 1.0, 0.0);
                }
                glPopMatrix();
                glTranslatef(1.0, 0.2, 0.0);
        }
}

void scene(void)
{
		// L'escalier
		glMaterialfv(GL_FRONT,GL_DIFFUSE,defaultDiff);
		glTranslatef(0.0, 0.0, 3.0);
        glPushMatrix();
        face(3);
        glRotatef(90, 0.0, 1.0, 0.0);
        face(6);
        glRotatef(90, 0.0, 1.0, 0.0);
        face(6);
        glRotatef(90, 0.0, 1.0, 0.0);
        face(3);

		// La balle
		glMaterialfv(GL_FRONT,GL_DIFFUSE,bleu);
        glTranslatef(posx, posy, posz);
        glRotatef(90, 0.0, 1.0, 0.0);  
        glutSolidSphere(0.5,36,36);

        glPopMatrix();
}

void configurationLumieresEtMateriaux(void) {
	float lum0Pos[] = {0.0F, 10.0F, 0.0F, 0.0F};
	glLightfv(GL_LIGHT0, GL_POSITION, lum0Pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gris);
	

	float lum1Pos[] = {0.0F, 0.0F, 0.0F, 1.0F};
	glLightfv(GL_LIGHT1, GL_POSITION, lum1Pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, vert);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

}

void reshape(int w,int h) {
        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //gluPerspective(8.0F,(float) w/h,80.0,120.0);
		int r = 10;
		int r2 = 7;
		if(w > h)
			glOrtho(-r2*w/h, r2*w/h, -r2, r2, -r, r);
		else
			glOrtho(-r2, r2, -r2*w/h, r2*w/h, -r, r);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

void display(void) {
        glClearColor(0.5F,0.5F,0.5F,1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        configurationLumieresEtMateriaux();
        glPolygonMode(GL_FRONT_AND_BACK,(aff) ? GL_FILL : GL_LINE);
        glPushMatrix();
        glRotatef(rx,1.0F,0.0F,0.0F);
        glRotatef(ry,0.0F,1.0F,0.0F);
        glRotatef(rz,0.0F,0.0F,1.0F);
        scene();
        glPopMatrix();
        glFlush();
        glutSwapBuffers();
}

void idle(void) {
    if(!pause)
    {
        if(cptface == 0)
                posx -= speed;
        if(cptface == 1)
                posz -= speed;
        if(cptface == 2)
                posx += speed;
        if(cptface == 3)
                posz += speed;

        tmp -= speed;
        tmp2 -= speed;
        if(tmp <= -1)
        {
                tmp = 0.0;
                posy -= 0.2;
        }
        if(cptface == 0 && tmp2 <= -2.0)
        {
                angle = 90;
                tmp2 = 0.0;
                cptface++;
				cheat = false;
        }
        if(cptface == 1 && tmp2 <= -6.0)
        {
                angle = 180;
                tmp2 = 0.0;
                cptface++;
        }
        if(cptface == 2 && tmp2 <= -6.0)
        {
                angle = 270;
                tmp2 = 0.0;
                cptface++;
        }
        if(cptface == 3)
        {
			if(tmp2 <= -4.0)
			{
				tmp = 0.0;
                tmp2 = 0.0;
                cptface = 0;
                posx = -1.0;
                posy = 0.8;
                posz = 0.0;
			}
			else if(tmp2 <= -3.0 && !cheat)
			{
				tmp = 0.0;
				cheat = true;
                posy = 0.8;
				posx = -1.0;
				posz = -1.0;
			}
        }

    }

    glutPostRedisplay();
}

void special(int code,int x,int y) {
        switch ( code ) {
        case GLUT_KEY_UP :
                rx += 1.0F;
                glutPostRedisplay();
                break;
        case GLUT_KEY_DOWN :
                rx -= 1.0F;
                glutPostRedisplay();
                break;
        case GLUT_KEY_RIGHT :
                ry += 1.0F;
                glutPostRedisplay();
                break;
        case GLUT_KEY_LEFT :
                ry -= 1.0F;
                glutPostRedisplay();
                break;
        case GLUT_KEY_PAGE_UP :
                rz += 1.0F;
                glutPostRedisplay();
                break;
        case GLUT_KEY_PAGE_DOWN :
                rz -= 1.0F;
                glutPostRedisplay();
                break; }
}

void keyboard(unsigned char key,int x,int y) {
        switch (key) {
        case 0x20 :
                rx = 39.0F;
				ry = 513.5F;
				rz = 0.0F;
                glutPostRedisplay();
                break;
        case 'm' :
                aff = !aff;
                glutPostRedisplay();
                break;
        case 'p' :
                pause = !pause;
                break;
        case 0x1B :
                exit(0);
                break; }
}

void mousemove(int x, int y)
{
		ry += (x-px);
		rx -= (y-py);
		px = x;
		py = y;
		glutPostRedisplay();
}

/* Fonction principale                          */

int main(int argc,char **argv) {
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
        glutInitWindowSize(900,750);
        glutInitWindowPosition(50,50);
        glutCreateWindow("Penrose");
        init();
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(special);
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutMotionFunc(mousemove);
        glutIdleFunc(idle);
        glutMainLoop();
        return(0);
}
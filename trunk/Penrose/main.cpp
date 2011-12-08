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
static const float vert[] = { 0.0F,1.0F,0.0F,1.0F };
static const float bleu[] = { 0.0F,0.0F,1.0F,1.0F };
static const float gris[] = { 0.8F,0.8F,0.8F,1.0F };
static const float blanc[] = { 1.0F,1.0F,1.0F,1.0F };
static const float rouge[] = { 1.0F, 0.0F, 0.0F, 1.0F };
static const float defaultDiff[] = { 0.8F,0.8F,0.8F,1.0F };

// Mode fil de fer
static int aff = 1;

// Vitesse de la balle
float speed = 0.005;

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

// Fonction de lecture de texture au format .raw
GLbyte *lireImageRaw(int tx,int ty,char *filename) {
  GLbyte *img = NULL;
  FILE *file = fopen(filename,"rb");
  if ( file ) {
    img =(GLbyte *) calloc(tx*ty*3,sizeof(GLbyte));
    fread(img,1,tx*ty*3,file);
    fclose(file); }
  return(img);
}

// Initialisation
void init(void) {
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);

	// Chargement de la texture
	GLbyte *image = lireImageRaw(16,16,"texture.raw");
	if (image)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT,1); 
		glTexImage2D(GL_TEXTURE_2D,0,3,16,16,0,GL_RGB,GL_UNSIGNED_BYTE,image);
		free(image);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT); 
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT); 
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); 
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
		glEnable(GL_TEXTURE_2D);
	}
}

// Permet de dessiné un cube texturé
void Cube(float longeur)
{
	float l = longeur/2;
	glBegin(GL_QUADS);

	// Face 1
	glNormal3f(0.0F,-1.0F,0.0F) ;
	glVertex3f(l,-l,-l); 
	glVertex3f(l,-l,l); 
	glVertex3f(-l,-l,l); 
	glVertex3f(-l,-l,-l);

	// Face 2
	glNormal3f(-1.0F,0.0F,0.0F) ;
	glVertex3f(-l,-l,l);
	glVertex3f(-l,-l,-l); 
	glVertex3f(-l,l,-l); 
	glVertex3f(-l,l,l); 

	// Face 3
	glNormal3f(0.0F,0.0F,1.0F) ;
	glVertex3f(-l,-l,l); 
	glVertex3f(l,-l,l); 
	glVertex3f(l,l,l); 
	glVertex3f(-l,l,l);

	// Face 4
	glNormal3f(1.0F,0.0F,0.0F) ;
	glVertex3f(l,-l,l); 
	glVertex3f(l,-l,-l); 
	glVertex3f(l,l,-l); 
	glVertex3f(l,l,l);

	// Face 5
	glNormal3f(0.0F,0.0F,-1.0F) ;
	glVertex3f(l,-l,-l); 
	glVertex3f(l,l,-l); 
	glVertex3f(-l,l,-l); 
	glVertex3f(-l,-l,-l);

	// Face 6 (texturée)
	glNormal3f(0.0F,1.0F,0.0F) ;
	glTexCoord2f(0.0F,0.0F);
	glVertex3f(-l,l,l);
	glTexCoord2f(0.5F,0.0F);
	glVertex3f(l,l,l); 
	glTexCoord2f(0.5F,0.5F);
	glVertex3f(l,l,-l);
	glTexCoord2f(0.0F,0.5F);
	glVertex3f(-l,l,-l);

	glEnd();
}

// Dessin d'un coté de l'escalier
void cote(int x)
{
        int y = 2;
        for(int i = 0;i < x;i++)
        {
                glPushMatrix();
                for(int j = 0;j < 1;j++)
                {
						Cube(1.0);
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
        cote(3);
        glRotatef(90, 0.0, 1.0, 0.0);
        cote(6);
        glRotatef(90, 0.0, 1.0, 0.0);
        cote(6);
        glRotatef(90, 0.0, 1.0, 0.0);
        cote(3);

		// Dessin de la balle
		glMaterialfv(GL_FRONT,GL_DIFFUSE,bleu);
        glTranslatef(posx, posy, posz);
        glRotatef(90, 0.0, 1.0, 0.0);  
        glutSolidSphere(0.5,36,36);

        glPopMatrix();
}

void configurationLumieres(void) {

	float lum0Pos[] = {0.0F, 10.0F, 0.0F, 0.0F};
	glLightfv(GL_LIGHT0, GL_POSITION, lum0Pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gris);

	float lum1Pos[] = {0.0F, 0.0F, 0.0F, 1.0F};
	glLightfv(GL_LIGHT1, GL_POSITION, lum1Pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, vert);

	float lum2Pos[] = {-20.0F, -10.0F, 20.0F, 0.0F};
	glLightfv(GL_LIGHT2, GL_POSITION, lum2Pos);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, rouge);

	float lum3Pos[] = {30.0F, -10.0F, 20.0F, 0.0F};
	glLightfv(GL_LIGHT3, GL_POSITION, lum3Pos);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, bleu);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHTING);

}

// Changement de la taille de la fenêtre et placement de la caméra de visualisation en projection orthographique
void reshape(int w,int h) {
        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
		int r = 10;
		int r2 = 7;
		if(w > h)
			glOrtho(-r2*w/h, r2*w/h, -r2, r2, -r, r);
		else
			glOrtho(-r2, r2, -r2*w/h, r2*w/h, -r, r);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

// Affichage
void display(void) {
        glClearColor(0.5F,0.5F,0.5F,1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        configurationLumieres();
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

// Permet de faire l'animation de la scene
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

		if(tmp <= -1)
        {
                tmp = 0.0;
                posy -= 0.2;
        }
        tmp -= speed;
        tmp2 -= speed;
        
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
				tmp = -0.25;
                tmp2 = 0.0;
                cptface = 0;
                posx = -1.0;
                posy = 0.8;
                posz = 0.0;
			}
			else if(tmp2 <= -3.0 && !cheat)
			{
				//tmp = 0.0;
				cheat = true;
                posy = 1.0;
				posx = -1.0;
				posz = -1.0;
			}
        }

    }

    glutPostRedisplay();
}

// Permet de détecter les évennements claviers
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

// Permet de détecter les évennements claviers
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
		case 'a' :
				if(speed > 0.01)
					speed -= 0.01;
                break;
		case 'z' :
                speed += 0.01;
                break;
        case 0x1B :
                exit(0);
                break; }
}

// Permet de détecter les évennements souris
void mousemove(int x, int y)
{
		ry += (x-px);
		rx -= (y-py);
		px = x;
		py = y;
		glutPostRedisplay();
}

int main(int argc,char **argv) {
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
        glutInitWindowSize(900,750);
        glutInitWindowPosition(50,50);
        glutCreateWindow("Escalier de Penrose");
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
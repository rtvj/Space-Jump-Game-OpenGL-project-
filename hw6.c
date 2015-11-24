/*
 *  3D house program with textures and light effects
 *  Author: Rutvij Karkhanis
 * 	Class: CSCI 5229
 *  Reference: ex15 ex9
 */

#include "CSCIx229.h"
int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=45;       //  Field of view (for perspective)
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[15]; // Texture names


/*
 *  Draw a House
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int text_index)	// textures for the walls
{
	//  Set specular color to white
	float white[] = {1,1,1,1};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
	//  Save transformation
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);
	//  Enable textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);

   //  Front
	glBindTexture(GL_TEXTURE_2D,texture[text_index]);
	glBegin(GL_POLYGON);
	glNormal3f( 0, 0, 1);

	glTexCoord2f(0,0);
	glVertex3f(-1,-1, 1);
	glTexCoord2f(1,0);
	glVertex3f(+1,-1, 1);
	glTexCoord2f(1,1);
	glVertex3f(+1,+1, 1);
	glTexCoord2f(0.5,1.5);
	glVertex3f(0, 2, 1);
	glTexCoord2f(0,1);
	glVertex3f(-1,+1, 1);
	glEnd();
	
	//  Back
	glBindTexture(GL_TEXTURE_2D,texture[9]);
	glBegin(GL_POLYGON);
	glNormal3f( 0, 0,-1);
	
	glTexCoord2f(0,0);
	glVertex3f(+1,-1,-1);
	glTexCoord2f(1,0);
	glVertex3f(-1,-1,-1);
	glTexCoord2f(1,1);
	glVertex3f(-1,+1,-1);
	glTexCoord2f(0.5,1.5);
	glVertex3f(0, 2, -1);
	glTexCoord2f(0,1);
	glVertex3f(+1,+1,-1);
	glEnd();

	//  Right
	glBindTexture(GL_TEXTURE_2D,texture[(text_index+2)%4]);
	glBegin(GL_POLYGON);
	glNormal3f(+1, 0, 0);

	glTexCoord2f(0,0);
	glVertex3f(+1,-1,+1);
	glTexCoord2f(1,0);
	glVertex3f(+1,-1,-1);
	glTexCoord2f(1,1);
	glVertex3f(+1,+1,-1);
	glTexCoord2f(0,1);
	glVertex3f(+1,+1,+1);
	glEnd();

	
	//  Left
	glBindTexture(GL_TEXTURE_2D,texture[(text_index+2)%4]);
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0,0);
	glVertex3f(-1,-1,-1);
	glTexCoord2f(1,0);
	glVertex3f(-1,-1,+1);
	glTexCoord2f(1,1);
	glVertex3f(-1,+1,+1);
	glTexCoord2f(0,1);
	glVertex3f(-1,+1,-1);
	glEnd();

	//  Top
		// left
	glBindTexture(GL_TEXTURE_2D,texture[3+text_index]);
	glBegin(GL_POLYGON);
	glNormal3f( -1,+1,0);
	glTexCoord2f(0,0);
	glVertex3f(-2,0,-1);
	glTexCoord2f(1,0);
	glVertex3f(-2,0,+1);
	glTexCoord2f(1,1);
	glVertex3f(0,+2,+1);
	glTexCoord2f(0,1);
	glVertex3f(0,+2,-1);
	glEnd();

		//right
	glBindTexture(GL_TEXTURE_2D,texture[3+text_index]);
	glBegin(GL_POLYGON);
	glNormal3f(1, 1, 0);
	glTexCoord2f(0,0);
	glVertex3f(2,0,+1);
	glTexCoord2f(1,0);
	glVertex3f(0,+2,+1);
	glTexCoord2f(1,1);
	glVertex3f(0,+2,-1);
	glTexCoord2f(0,1);
	glVertex3f(2,0,-1);
	glEnd();

   //  Bottom
	glBindTexture(GL_TEXTURE_2D,texture[6]);
	glBegin(GL_POLYGON);
	glNormal3f( 0,-1, 0);
	glTexCoord2f(0,0);
	glVertex3f(-1,-1,-1);
	glTexCoord2f(1,0);
	glVertex3f(+1,-1,-1);
	glTexCoord2f(1,1);
	glVertex3f(+1,-1,+1);
	glTexCoord2f(0,1);
	glVertex3f(-1,-1,+1);
	glEnd();

	// door
	glBindTexture(GL_TEXTURE_2D,texture[7]);
	glBegin(GL_POLYGON);
	glNormal3f( 0, 0, 1);
	glTexCoord2f(1,1);
	glVertex3f(0.25, -0.25, 1.01);
	glTexCoord2f(0,1);
	glVertex3f(-0.25, -0.25, 1.01);
	glTexCoord2f(0,0);
	glVertex3f(-0.25, -1, 1.01);
	glTexCoord2f(1,0);
	glVertex3f(0.25, -1, 1.01);
	glEnd();

	// right window
	glBindTexture(GL_TEXTURE_2D,texture[8]);
	glBegin(GL_POLYGON);
	glNormal3f( 0, 0, 1);
	glTexCoord2f(1,1);
	glVertex3f(-0.25, 0.75, 1.01);
	glTexCoord2f(0,1);
	glVertex3f(-0.75, 0.75, 1.01);
	glTexCoord2f(0,0);
	glVertex3f(-0.75, 0.25, 1.01);
	glTexCoord2f(1,0);
	glVertex3f(-0.25, 0.25, 1.01);
	glEnd();

	// Left window
	glBindTexture(GL_TEXTURE_2D,texture[8]);
	glBegin(GL_POLYGON);
	glNormal3f( 0, 0, 1);
	glTexCoord2f(1,1); glVertex3f(0.75, 0.75, 1.01);
	glTexCoord2f(0,1); glVertex3f(0.25, 0.75, 1.01);
	glTexCoord2f(0,0); glVertex3f(0.25, 0.25, 1.01);
	glTexCoord2f(1,0); glVertex3f(0.75, 0.25, 1.01);
	glEnd();

	//  Undo transformations and textures
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
	//  Save transformation
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(r,r,r);
	//  White ball
	glColor3f(1,1,1);
	glutSolidSphere(1.0,16,16);
	//  Undo transofrmations
	glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
	const double len=2.0;  //  Length of axes
	double Ex, Ey, Ez;
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   switch (mode)
   {
	    case 0:
			Ex = -2*dim*Sin(th)*Cos(ph);
			Ey = +2*dim        *Sin(ph);
			Ez = +2*dim*Cos(th)*Cos(ph);
			gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
			break;
	
		case 1:
			Ex = -2*dim*Sin(th)*Cos(ph);
			Ey = 0;
			Ez = +2*dim*Cos(th)*Cos(ph);
			fov = 45;
			gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,1,0);
			break;
   }
   if (light)
   {
	   //  Translate intensity to color vectors
	   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
	   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
	   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	   //  Light direction
	   float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
	   //  Draw light position as ball (still no lighting here)
	   glColor3f(1,1,1);
	   ball(Position[0],Position[1],Position[2] , 0.1);
	   //  OpenGL should normalize normal vectors
	   glEnable(GL_NORMALIZE);
	   //  Enable lighting
	   glEnable(GL_LIGHTING);
	   //  glColor sets ambient and diffuse color materials
	   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	   glEnable(GL_COLOR_MATERIAL);
	   //  Enable light 0
	   glEnable(GL_LIGHT0);
	   //  Set ambient, diffuse, specular components and position of light 0
	   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	   glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
   {
	   glDisable(GL_LIGHTING);
   }
   //  Draw the three houses

	house(1,0,0, 0.25,0.2,0.3, 20, 1);
	house(4,0,0, 0.5,0.4,0.3, -30, 2);
	house(-2,0,-1, 0.5,0.3,0.5, -120, 3);
	//  Draw axes - no lighting from here on
	glDisable(GL_LIGHTING);

	glColor3f(1,1,1);
	if (axes)
	{
		glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(len,0.0,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,len,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,0.0,len);
		glEnd();
		//  Label axes
		glRasterPos3d(len,0.0,0.0);
		Print("X");
		glRasterPos3d(0.0,len,0.0);
		Print("Y");
		glRasterPos3d(0.0,0.0,len);
		Print("Z");
	}
	//  Display parameters
	glWindowPos2i(5,5);
	Print("mode=%d Angle=%d,%d  Dim=%.1f Light=%s Projection=%s",mode,th,ph,dim,light?"On":"Off",((mode==0)?"Perpective":"First Person"));
	if (light)
	{
		glWindowPos2i(5,25);
		Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
	}
	//  Render the scene and make it visible
	ErrCheck("display");
	glFlush();
	glutSwapBuffers();
}


/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
	//  Elapsed time in seconds
	double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
	zh = fmod(90*t,360.0);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_LEFT)
		th += 5;
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_RIGHT)
		th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
	//  PageUp key - increase dim
	else if (key == GLUT_KEY_PAGE_DOWN)
		dim += 0.1;
	//  PageDown key - decrease dim
	else if (key == GLUT_KEY_PAGE_UP && dim>1)
		dim -= 0.1;
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	//  Update projection
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
	if (ch == 27)
	exit(0);
	//  Reset view angle
	else if (ch == '0')
	th = ph = 0;
	//  Toggle texture mode
	else if (ch == 'm' || ch == 'M')
	mode = (mode+1)%2;
	//  Toggle axes
	else if (ch == 'x' || ch == 'X')
	axes = 1-axes;
	//  Toggle lighting
	else if (ch == 'l' || ch == 'L')
	light = 1-light;
	//  Toggle textures mode
	else if (ch == 't')
	ntex = 1-ntex;
	//  Light elevation
	else if (ch=='[')
	ylight -= 0.1;
	else if (ch==']')
	ylight += 0.1;
	//  Ambient level
	else if (ch=='a' && ambient>0)
	ambient -= 5;
	else if (ch=='A' && ambient<100)
	ambient += 5;
	//  Diffuse level
	else if (ch=='d' && diffuse>0)
	diffuse -= 5;
	else if (ch=='D' && diffuse<100)
	diffuse += 5;
	//  Specular level
	else if (ch=='s' && specular>0)
	specular -= 5;
	else if (ch=='S' && specular<100)
	specular += 5;
	//  Emission level
	else if (ch=='e' && emission>0)
	emission -= 5;
	else if (ch=='E' && emission<100)
	emission += 5;
	//  Shininess level
	else if (ch=='n' && shininess>-1)
	shininess -= 1;
	else if (ch=='N' && shininess<7)
	shininess += 1;
	//  Repitition
	else if (ch=='+')
	rep++;
	else if (ch=='-' && rep>1)
	rep--;
	
	//  Translate shininess power to value (-1 => 0)
	shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
	//  Reproject
	Project(fov,asp,dim);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}
/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600,600);
	glutCreateWindow("Lighting And Textures");
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	//  Load textures
	texture[0] = LoadTexBMP("brick1.bmp");
	texture[1] = LoadTexBMP("brick2.bmp");
	texture[2] = LoadTexBMP("brick3.bmp");
	texture[3] = LoadTexBMP("brick4.bmp");
	texture[4] = LoadTexBMP("roof1.bmp");
	texture[5] = LoadTexBMP("roof2.bmp");
	texture[6] = LoadTexBMP("roof3.bmp");
	texture[7] = LoadTexBMP("door1.bmp");
	texture[8] = LoadTexBMP("window1.bmp");
	texture[9] = LoadTexBMP("brick5.bmp");
	texture[10] = LoadTexBMP("grass.bmp");
	//  Pass control to GLUT so it can interact with the user
	ErrCheck("init");
	glutMainLoop();
	return 0;
}
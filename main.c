/*
 * Project    : Spacejump
 * Author     : Rutvij Karkhanis
 * Course     : Computer Graphics
 * References : Coursework examples
 * 				 www.spacesimulator.net
 * 				 www.student.nada.kth.se/~nv91-gta/OpenGL/examples/explosion.c
 *
 * arrows     : Navigate the spaceship
 * spacebar   : Jump
 * PgDn/PgUp  : Move spaceship backword/forward
 * ESC        : Exit
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <sys/stat.h>
#include "CSCIx229.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


#define MAX_VERTICES 10000 // Max number of vertices (for each object)
#define MAX_POLYGONS 10000 // Max number of polygons (for each object)
#define SH 5000

// Our vertex type
typedef struct {
    float x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct {
    int a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct {
    float u,v;
}mapcoord_type;

// The object type
typedef struct {
	char name[20];
    
        int vertices_qty;
        int polygons_qty;

        vertex_type vertex[MAX_VERTICES]; 
        polygon_type polygon[MAX_POLYGONS];
        mapcoord_type mapcoord[MAX_VERTICES];
        int id_texture;
} obj_type, *obj_type_ptr;

typedef struct                       /**** BMP file info structure ****/
    {
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
    char *data;
    } BITMAPINFOHEADER;

double Ux = 1;   //  Up
double Uy = 0;   //  Up
double Uz = 0;   //  Up
double Ox = 0;   //  LookAt
double Oy = 0;   //  LookAt
double Oz = 0;   //  LookAt
double Ex = 1;   //  Eye
double Ey = 1;   //  Eye
double Ez = 1;   //  Eye
double X  = 0;   //  Location
double Y  = 0;   //  Location
double Z  = 0;   //  Location
double Dx = 1;   //  Direction
double Dy = 0;   //  Direction
double Dz = 0;   //  Direction
int    th=0;     //  Azimuth of view angle
int    ph=0;     //  Elevation of view angle
int    zh=0;     //  Azimuth of light
int    move=0;   //  Light movement
double dim=1500.0;  //  Size of world
static int px = 0;  // spaceship x coordinate
static int py = 0;  // spaceship y coordinate
static int pz = 2400;  // spaceship z coordinate
int space = 1;	// spaceship present variable
int prs = 75;  // precision
double side = 0;  // horizontal movement variable
double up = 0; // vertical movement variable
int fx = 0, fy = 0, fz = -12;
float rep = 1.0;
// The width and height of your window, change them as you like
int screen_width=1280;
int screen_height=720;
int	sky[2];   //  Sky textures
int zz[20] ;
float z[65][65];       //  DEM data
float zmin=+1e8;       //  DEM lowest location
float zmax=-1e8;       //  DEM highest location
float zmag=1;          //  DEM magnification
float ty = SH;
int ii=0;
int flag1 = 0, flag2 = 0, flag3 = 0;
int width, height;
int enter = 1;


// Absolute rotation values (0-359 degrees) and rotation increments for each frame
double rotation_x=0, rotation_x_increment=0.1;
double rotation_y=0, rotation_y_increment=0.05;
double rotation_z=0, rotation_z_increment=0.03;

int num_texture=-1;
int      wantNormalize = 0;   /* Speed vector normalization flag */
int      wantPause = 0;       /* Pause flag */
obj_type object;
int ov = 0, over = 0;


char Load3DS (obj_type_ptr ogg, char *filename);
int LoadBitmap(char *filename);
unsigned int LoadTexBMP(const char* file);
static void Reverse(void* x,const int n);
void DEM(double xx, double yy, double zz);
void ReadDEM(double xx, double yy, double zz);
void spaceship(double px, double py, double pz,	double dx, double dy, double dz, double sx, double sy, double sz);
void glWindowPos2i(GLint  x,  GLint  y);
void timer(int toggle);


// initialization function
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // This clear the background color to black
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
   	
    // Viewport transformation
    glViewport(0,0,screen_width,screen_height);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f); // We define the "viewing volume"
   
    glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    
    glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

    Load3DS (&object,"spaceship.3ds");

    object.id_texture=LoadBitmap("spaceshiptexture.bmp"); // The Function LoadBitmap() return the current texture ID
    
    // If the last function returns -1 it means the file was not found so we exit from the program
    if (object.id_texture==-1)
    {
    	printf("Image file: spaceshiptexture.bmp not found\n");
        //MessageBox(NULL,"Image file: spaceshiptexture.bmp not found", "Zetadeck",MB_OK | MB_ICONERROR);
        exit (0);
    }
}


// draw spaceship object
void spaceship(double px, double py, double pz,	double dx, double dy, double dz, double sx, double sy, double sz)
{

	int l_index;

   glPushMatrix();

   glTranslated(px,py,pz);

    glScaled(sz,sy,sy);

    rotation_x = dx;
    rotation_y = dy;
    rotation_z = dz;

    glRotatef(rotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
    glRotatef(rotation_y,0.0,1.0,0.0);  // 
    glRotatef(rotation_z,0.0,0.0,1.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 

    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)

    for (l_index=0;l_index<object.polygons_qty;l_index++)
    {
        // Texture coordinates of the first vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
                      object.mapcoord[ object.polygon[l_index].a ].v);
        // Coordinates of the first vertex
        glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
                    object.vertex[ object.polygon[l_index].a ].y,
                    object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

        // Texture coordinates of the second vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
                      object.mapcoord[ object.polygon[l_index].b ].v);
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
                    object.vertex[ object.polygon[l_index].b ].y,
                    object.vertex[ object.polygon[l_index].b ].z);
        
        // Texture coordinates of the third vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
                      object.mapcoord[ object.polygon[l_index].c ].v);
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
                    object.vertex[ object.polygon[l_index].c ].y,
                    object.vertex[ object.polygon[l_index].c ].z);
    }
   glEnd();
   glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


/* 
 *  Draw sky box
 */

static void Sky(double D)
{
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);

        //  Sides
        glBindTexture(GL_TEXTURE_2D,sky[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
        glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
        glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
        glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

        glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
        glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
        glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
        glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

        glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
        glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
        glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
        glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

        glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
        glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
        glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
        glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
        glEnd();

        //  Top and bottom
        glBindTexture(GL_TEXTURE_2D,sky[1]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
        glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
        glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
        glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

        glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
        glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
        glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
        glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
        glEnd();

        glDisable(GL_TEXTURE_2D);
}



/**********************************************************
 *
 * SUBROUTINE display()
 *
 * This is our main rendering subroutine, called each frame
 * 
 *********************************************************/
void display(void)
{
	int i;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the window and the depth buffer 

        glMatrixMode(GL_MODELVIEW);


	width =  glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

        glWindowPos2i(width - 100, height - 15);

        //  Erase the window and the depth buffer
        glClearColor(0,0.3,0.7,0);

        glLoadIdentity();

	Sky(3.5*dim); // draw space like looking texture

        glLoadIdentity(); // Initialize the model matrix as identity
        gluLookAt(Ex,Ey,Ez , Ox,Oy,Oz , Ux,Uy,Uz);
	


  	if(space)
		spaceship(px, py, pz, -90+up, side, 180,0.5,0.5,0.5);

	glTranslatef(fx,fy,fz);


        glFlush(); // This force the execution of OpenGL commands
        glutSwapBuffers(); // In double buffered mode we invert the positions of the visible buffer and the writing buffer


	flag3++;
}

long filelength(int f)
{
        struct stat buf;
        fstat(f, &buf);
        return(buf.st_size);
}



char Load3DS (obj_type_ptr p_object, char *p_filename)
{
        int i; //Index variable
        size_t size;
        FILE *l_file; //File pointer

        unsigned short l_chunk_id; //Chunk identifier
        unsigned int l_chunk_lenght; //Chunk lenght

        unsigned char l_char; //Char variable
        unsigned short l_qty; //Number of elements in each chunk

        unsigned short l_face_flags; //Flag that stores some face information

        if ((l_file=fopen (p_filename, "rb"))== NULL) return 0; //Open the file

        while (ftell (l_file) < filelength (fileno (l_file))) //Loop to scan the whole file
        {
                size = fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
                //printf("ChunkID: %x\n",l_chunk_id); 
                size = fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
                //printf("ChunkLenght: %x\n",l_chunk_lenght);

                switch (l_chunk_id)
                {
                        case 0x4d4d: 
                        break;    

                        case 0x3d3d:
                        break;

                        case 0x4000: 
                        i=0;
                        do
                        {
                                size = fread (&l_char, 1, 1, l_file);
                                p_object->name[i]=l_char;
                                i++;
                        } while(l_char != '\0' && i<20);
                        break;

                        case 0x4100:
                        break;

                        case 0x4110: 
                        size = fread (&l_qty, sizeof (unsigned short), 1, l_file);
                        p_object->vertices_qty = l_qty;
                        //printf("Number of vertices: %d\n",l_qty);
                        for (i=0; i<l_qty; i++)
                        {
                                size = fread (&p_object->vertex[i].x, sizeof(float), 1, l_file);
                                //printf("Vertices list x: %f\n",p_object->vertex[i].x);
                                size = fread (&p_object->vertex[i].y, sizeof(float), 1, l_file);
                                //printf("Vertices list y: %f\n",p_object->vertex[i].y);
                                size = fread (&p_object->vertex[i].z, sizeof(float), 1, l_file);
                                //printf("Vertices list z: %f\n",p_object->vertex[i].z);
                        }
                        break;

                        case 0x4120:
                        size = fread (&l_qty, sizeof (unsigned short), 1, l_file);
                        p_object->polygons_qty = l_qty;
                        //printf("Number of polygons: %d\n",l_qty); 
                        for (i=0; i<l_qty; i++)
                        {
                                size = fread (&p_object->polygon[i].a, sizeof (unsigned short), 1, l_file);
                                //printf("Polygon point a: %d\n",p_object->polygon[i].a);
                                size = fread (&p_object->polygon[i].b, sizeof (unsigned short), 1, l_file);
                                //printf("Polygon point b: %d\n",p_object->polygon[i].b);
                                size = fread (&p_object->polygon[i].c, sizeof (unsigned short), 1, l_file);
                                //printf("Polygon point c: %d\n",p_object->polygon[i].c);
                                size = fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
                                //printf("Face flags: %x\n",l_face_flags);
                        }
                        break;

                        case 0x4140:
                        size = fread (&l_qty, sizeof (unsigned short), 1, l_file);
                        for (i=0; i<l_qty; i++)
                        {
                                size = fread (&p_object->mapcoord[i].u, sizeof (float), 1, l_file);
                                //printf("Mapping list u: %f\n",p_object->mapcoord[i].u);
                                size = fread (&p_object->mapcoord[i].v, sizeof (float), 1, l_file);
                                //printf("Mapping list v: %f\n",p_object->mapcoord[i].v);
                        }
                        break;

                        size = size + 1;

                        default:
                        fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
                } 
        }
        fclose (l_file); // Closes the file stream
        return (1); // Returns ok
}


/*
 *  Reverse n bytes
 */
static void Reverse(void* x,const int n)
{
   int k;
   char* ch = (char*)x;
   for (k=0;k<n/2;k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

// Load texture from bmp
int LoadBitmap(char *filename)
{
	//printf("loadbitmap\n");
        FILE * file;
        char temp;
        long i;
         size_t size;

        BITMAPINFOHEADER infoheader;

        num_texture++; // The counter of the current texture is increased

        if( (file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading

        fseek(file, 18, SEEK_CUR);  /* start reading width & height */
        size = fread(&infoheader.biWidth, sizeof(int), 1, file);

        size = fread(&infoheader.biHeight, sizeof(int), 1, file);

        size = fread(&infoheader.biPlanes, sizeof(short int), 1, file);
        if (infoheader.biPlanes != 1) {
            //printf("Planes from %s is not 1: %u\n", filename, infoheader.biPlanes);
            return 0;
        }

        // read the bpp
        size = fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
        if (infoheader.biBitCount != 24) {
        // printf("Bpp from %s is not 24: %d\n", filename, infoheader.biBitCount);
        return 0;
        }


        fseek(file, 24, SEEK_CUR);

        // read the data.
        infoheader.data = (char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
        if (infoheader.data == NULL) {
           // printf("Error allocating memory for color-corrected image data\n");
            return 0;
        }

        if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
            //fprintf("Error reading image data from %s.\n", filename);
            return 0;
        }

        for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
            temp = infoheader.data[i];
            infoheader.data[i] = infoheader.data[i+2];
            infoheader.data[i+2] = temp;
        }


        fclose(file); // Closes the file stream


        glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

        // The next commands sets the texture parameters
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

        // Finally we define the 2d texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

        // And create 2d mipmaps for the minifying function
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);
        size = size + 1;
        free(infoheader.data); // Free the memory we used to load the texture

        return (num_texture); // Returns the current texture OpenGL ID
}


/*
 *  Load texture from BMP file
 */

unsigned int LoadTexBMP(const char* file)
{
        unsigned int   texture;    // Texture name
        FILE*          f;          // File pointer
        unsigned short magic;      // Image magic
        unsigned int   dx,dy,size; // Image dimensions
        unsigned short nbp,bpp;    // Planes and bits per pixel
        unsigned char* image;      // Image data
        unsigned int   k;          // Counter
        int            max;        // Maximum texture dimensions

        //  Open file
        f = fopen(file,"rb");
        if (!f) Fatal("Cannot open file %s\n",file);
        //  Check image magic
        if (fread(&magic,2,1,f)!=1) Fatal("Cannot read magic from %s\n",file);
        if (magic!=0x4D42 && magic!=0x424D) Fatal("Image magic not BMP in %s\n",file);
        //  Seek to and read header
        if (fseek(f,16,SEEK_CUR) || fread(&dx ,4,1,f)!=1 || fread(&dy ,4,1,f)!=1 ||
        fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
        Fatal("Cannot read header from %s\n",file);
        //  Reverse bytes on big endian hardware (detected by backwards magic)
        if (magic==0x424D)
        {
        Reverse(&dx,4);
        Reverse(&dy,4);
        Reverse(&nbp,2);
        Reverse(&bpp,2);
        Reverse(&k,4);
        }
        //  Check image parameters
        glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);
        if (dx<1 || dx>max) Fatal("%s image width %d out of range 1-%d\n",file,dx,max);
        if (dy<1 || dy>max) Fatal("%s image height %d out of range 1-%d\n",file,dy,max);
        if (nbp!=1)  Fatal("%s bit planes is not 1: %d\n",file,nbp);
        if (bpp!=24) Fatal("%s bits per pixel is not 24: %d\n",file,bpp);
        if (k!=0)    Fatal("%s compressed files not supported\n",file);
#ifndef GL_VERSION_2_0
        //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
        for (k=1;k<dx;k*=2);
        if (k!=dx) Fatal("%s image width not a power of two: %d\n",file,dx);
        for (k=1;k<dy;k*=2);
        if (k!=dy) Fatal("%s image height not a power of two: %d\n",file,dy);
#endif

        //  Allocate image memory
        size = 3*dx*dy;
        image = (unsigned char*) malloc(size);
        if (!image) Fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
        //  Seek to and read image
        if (fseek(f,20,SEEK_CUR) || fread(image,size,1,f)!=1) Fatal("Error reading data from image %s\n",file);
        fclose(f);
        //  Reverse colors (BGR -> RGB)
        for (k=0;k<size;k+=3)
        {
        unsigned char temp = image[k];
        image[k]   = image[k+2];
        image[k+2] = temp;
        }

        //  Sanity check
        ErrCheck("LoadTexBMP");
        //  Generate 2D texture
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D,texture);
        //  Copy image
        glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
        if (glGetError()) Fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
        //  Scale linearly when image size doesn't match
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        //  Free image memory
        free(image);
        //  Return texture name
        return texture;
}

/*
 *  Read DEM from file
 */
void ReadDEM(double xx, double yy, double zz)
{
        int i,j;
        for (j=0;j<=64;j++) {

                for (i=0;i<=64;i++) {

                        z[i][j] = -10 + (rand() % 21);
                        if (z[i][j] < zmin) zmin = z[i][j];
                        if (z[i][j] > zmax) zmax = z[i][j];
                }

        }
	
}


/**********************************************************
 *
 * SUBROUTINE resize(int,int)
 *
 * This routine must be called everytime we resize our window.
 * 
 *********************************************************/

void resize (int width, int height)
{
        screen_width=width; // We obtain the new screen width values and store it
        screen_height=height; // Height value

        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
        glViewport(0,0,screen_width,screen_height); // Viewport transformation

        glMatrixMode(GL_PROJECTION); // Projection transformation
        glLoadIdentity(); // We initialize the projection matrix as identity
        gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f);

        glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}


/**********************************************************
 *
 * SUBROUTINE keyboard(unsigned char,int,int)
 *
 * Used to handle the keyboard input (ASCII Characters)
 * 
 *********************************************************/

void keyboard (unsigned char key, int x, int y)
{
        
        switch (key)
        {

                case 27:
                exit(0);
                break;

                case 'x':
                case 'X':
                enter = 0;
                break;
        }
        th %= 360;
        ph %= 360;
        timer(-1);
}



/**********************************************************
 *
 * SUBROUTINE keyboard(int,int,int)
 *
 * Used to handle the keyboard input (not ASCII Characters)
 * 
 *********************************************************/

void keyboard_s (int key, int x, int y)
{
        
        switch (key)
        {

                case GLUT_KEY_UP:
                py = py + 10;
                if(up < 30)
                        up = up + 2;
                break;

                case GLUT_KEY_DOWN:
                py = py - 10;
                if(up > -30)
                        up = up - 2;
                break;

                case GLUT_KEY_LEFT:
                px = px - 10;
                if(side > -30)
                        side = side - 2;
                break;

                case GLUT_KEY_RIGHT:
                px = px + 10;
                if(side < 30)
                        side = side + 2;
                break;

                case GLUT_KEY_PAGE_UP:
                pz = pz + 50;
                break;

                case GLUT_KEY_PAGE_DOWN:
                pz = pz - 50;
                break;
                break;

        }
}

/*
 *  GLUT calls this routine every 50ms
 */
void timer(int toggle)
{
        int i;

        flag2++;

        //  Toggle movement
        if (toggle>0)
                move = !move;
        //  Increment light position
        else
                zh = (zh+5)%360;

        if(side < 0) side = side + 1;
        if(side > 0) side = side - 1;

        if(up < 0) up = up + 1;
        if(up > 0) up = up - 1;

        //  Animate flight using Lorenz transform
        Ex = -2*dim*Sin(th)*Cos(ph);
        Ey = +2*dim        *Sin(ph);
        Ez = +2*dim*Cos(th)*Cos(ph);
        Ox = Oy = Oz = 0;
        X = Y = Z = 0;
        Dx = 1; Dy = 0; Dz = 0;
        Ux = 0; Uy = 1; Uz = 0;

        //  Set timer to go again
        if (move && toggle>=0) glutTimerFunc(50,timer,0);
        //  Tell GLUT it is necessary to redisplay the scene
        glutPostRedisplay();
}



/**********************************************************
 *
 * The main routine
 * 
 *********************************************************/

int main(int argc, char **argv)
{
        // We use the GLUT utility to initialize the window, to handle the input and to interact with the windows system
        glutInit(&argc, argv);    
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(screen_width,screen_height);
        glutInitWindowPosition(0,0);
        glutCreateWindow("SpaceJump");  
        glutFullScreen();   
        glutDisplayFunc(display);
        //glutIdleFunc (idle);
        glutReshapeFunc (resize);
        glutKeyboardFunc (keyboard);
        glutSpecialFunc (keyboard_s);
        timer(1);
        sky[0] = LoadTexBMP("sky0.bmp");
        sky[1] = LoadTexBMP("sky1.bmp");
        ReadDEM(0.0,0.0,0.0);
        init();


        width =  glutGet(GLUT_WINDOW_WIDTH);
        height = glutGet(GLUT_WINDOW_HEIGHT);

        glutMainLoop();

        return(0);    
}



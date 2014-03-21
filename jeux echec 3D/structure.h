#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "glm.h"
#include "tga.h"

#define bool int
#define true 1
#define false 0
#define FICHIER_OBJ "./objet/colada.obj"

#define BUFSIZE 512;
#define NombreMaxPosibilite 64
 

typedef struct CASE{
       int id;
       int nl,nc;// enplacerment des pieces en numero de Ligne et numero deColonne
       int idPion;
       }Case; 
       
       
       
typedef struct PION{
       int camp;
       char nom[12];
       float ix,iy;// enplacerment initial des pieces en numero de Ligne et numero deColonne
       float x,y;// enplacerment des pieces en numero de Ligne et numero deColonne
       int idCase; 
       int nombrePosibilite;
       int posibilite[NombreMaxPosibilite];// liste des id des cases ou le pion a la possibilité de se posé
       bool prise;
       }Pion;


Case lesCases[65];     //case 0 n'est jamain uiliser donc soit de 1 à 64, c'est pour faire corespondre le numero au indice des cases
Pion lesPions[32] ;
char lesNoms[32][11] ={"tour21","cavalier21","fou21","roi2","dame2","fou22","cavalier22","tour22","pion21","pion22","pion23","pion24","pion25","pion26","pion27","pion28",
                           "pion11","pion12","pion13","pion14","pion15","pion16","pion17","pion18","tour11","cavalier11","fou11","roi1","dame1","fou12","cavalier12","tour12"
                        };
GLfloat place[9]={0,-7,-5,-3,-1,1,3,5,7};//pour les coordonnées

GLMmodel *pmodel ;GLMgroup *group ;

Texture	aTexture[2];


GLfloat echelle=1,lePas=0.25;
GLfloat alpha=0,beta=20,alpha2=0,beta2=0;//pour la rotation
int windowsH=700,windowsW=1000;//taile de la fenetre

/* pour le deplacement des pions */
int caseDepart=-1,caseArrivee=-1,idPionPourCasePossible=-1; 
bool clicker=false;bool fullscreen=false; 


/* Utile si on utilse une projection en perspective */
double fovy = 20.0, _zNear = 0.1, _zFar = 10.0;

/* Paramètre GLM */ 
int ww, wh, centerZ = -6,  Help_Font = (int) GLUT_BITMAP_8_BY_13,linestart = 10, linespace = 20;
int  animation = 0,show_help = 1,brouillard=0,ombre=0,reflet=0;//show_axis = 1,,xrotate = 0, yrotate = 1, zrotate = 0;

/* variables d'illumination  */
GLfloat light_ambient[] ={ 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = {0.0, 5.0, 0.0,  .70};

/* les callback   */
void Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void fleches(int key, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Display(void);
void HelpDisplay(GLint ww, GLint wh); 
void HelpRenderBitmapString(float x, float y, void *font, char *string);
 
/* mes fonctions */
void init(void);  
//void DrawModel(void);
void DrawScene();
void RenderReflet( void );
void DrawPlateau();
void DrawGrille(); 
void drawCasePosible(); 
void DrawSky(); 
bool chargerTextures(int it, char* nom); 
void setPicking(int x, int y);
void getPickingResult(GLint hits, GLuint *names); 
void casePossible(int idPion);
void posibiliteDuTour(int idPion);
void posibiliteDuPion1(int idPion);
void posibiliteDuPion2(int idPion);
void posibiliteDuCavalier(int idPion);
void posibiliteDuFou(int idPion);
void posibiliteDuRoi(int idPion);
void posibiliteDeLaDame(int idPion);
    
void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
void shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4]);
    
    
    
    
    
    
    
    

/* Find the plane equation given 3 points. */
enum { X, Y, Z, W };
enum { A, B, C, D };

/* Create a matrix that will project the desired shadow. */
void shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4])
{
    GLfloat dot;

    dot = groundplane[X] * lightpos[X] +
          groundplane[Y] * lightpos[Y] +
          groundplane[Z] * lightpos[Z] +
          groundplane[W] * lightpos[W];

    shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
    shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
    shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
    shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

    shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
    shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
    shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
    shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

    shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
    shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
    shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
    shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

    shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
    shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
    shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
    shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

 
void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
    GLfloat vec0[3], vec1[3];

    /* Need 2 vectors to find cross product. */
    vec0[X] = v1[X] - v0[X];
    vec0[Y] = v1[Y] - v0[Y];
    vec0[Z] = v1[Z] - v0[Z];

    vec1[X] = v2[X] - v0[X];
    vec1[Y] = v2[Y] - v0[Y];
    vec1[Z] = v2[Z] - v0[Z];

    /* find cross product to get A, B, and C of plane equation */
    plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
    plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
    plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

    plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}




















/*
static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		1,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
*/

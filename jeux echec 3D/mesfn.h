
#define bool int
#define true 1
#define false 0


#define NOMFICHIER "./myscene.obj"

/* Utile si on utilse une projection Ortho */
double _left = 0.0;		
double _right = 0.0;
double _bottom = 0.0;
double _top = 0.0;

/* Utile si on utilse une projection en perspective */
double fovy = 45.0;
double prev_z = 0;
double _zNear = 0.1;
double _zFar = 10.0;


/* Utilisation de la souris  */
int _mouseX = 0;		
int _mouseY = 0;
bool _mouseLeft = false;
bool _mouseMiddle = false;
bool _mouseRight = false;

double _dragPosX = 0.0;
double _dragPosY = 0.0;
double _dragPosZ = 0.0;

/* Pour lire les matrices Modelview  */
double _matrix[16];		
double _matrixI[16];
double module(double x, double y, double z);
void pos(double *px, double *py, double *pz, const int x, const int y,
	 const int *viewport);
void getMatrix();
void invertMatrix(const GLdouble * m, GLdouble * out);

/* Pointeur sur le modèle */
GLMmodel *pmodel = NULL;	

/* Paramètre GLM */
GLuint mode = 0;	

int ww, wh;			/* Taille fenêtre*/
int filaire = 0;		/* solide ou fil de fer  */
int show_axis = 1;
int smooth = 1;
int faces = 1;
int show_help = 1;		/* affichage du help */
int animation = 0;
int centerZ = -3;
float auto_rotate = 0;
int xrotate = 0;
int yrotate = 1;
int zrotate = 0;

int lighting = 1;

int Help_Font = (int) GLUT_BITMAP_8_BY_13;
int linestart = 10;		/* pour gérer l'affichage du texte du help  */
int linespace = 20;		

/* variables d'illumination  */
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };



void Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Display(void);
void Anime(void);*/

// fonctions d'affichage 
void DrawModel(void);
void DrawAxis(float scale);
void HelpDisplay(GLint ww, GLint wh);
void HelpRenderBitmapString(float x, float y, void *font, char *string);

void init(void);


/* Initialisation */
void init(void){
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

      if (lighting){
  	    glEnable(GL_LIGHTING);
  	    glEnable(GL_LIGHT0);
    }

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    if (!pmodel) {		/* load up the model */
	  pmodel = glmReadOBJ(NOMFICHIER);
	
	if (!pmodel) {
	    printf("\nErreur d'ouverture de %s\n",NOMFICHIER);
	    getchar(); exit(0);
	}
	
	glmUnitize(pmodel);
	glmVertexNormals(pmodel, 90.0, GL_TRUE);
    }
}

/* Callbacks */
void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    ww = w;
    wh = h;

    _top = 1.0;
    _bottom = -1.0;
    _left = -(double) w / (double) h;
    _right = -_left;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* glOrtho(_left,_right,_bottom,_top,_zNear,_zFar); */
    gluPerspective(fovy, (double) w / (double) h, _zNear, _zFar);

    glMatrixMode(GL_MODELVIEW);
}

void Mouse(int button, int state, int x, int y)
{
    int viewport[4];

    _mouseX = x;
    _mouseY = y;

    if (state == GLUT_UP)
	switch (button) {
	case GLUT_LEFT_BUTTON:
	    _mouseLeft = false;
	    break;
	case GLUT_MIDDLE_BUTTON:
	    _mouseMiddle = false;
	    break;
	case GLUT_RIGHT_BUTTON:
	    _mouseRight = false;
	    break;
    } else
	switch (button) {
	case GLUT_LEFT_BUTTON:
	    _mouseLeft = true;
	    break;
	case GLUT_MIDDLE_BUTTON:
	    _mouseMiddle = true;
	    break;
	case GLUT_RIGHT_BUTTON:
	    _mouseRight = true;
	    break;
	}

    glGetIntegerv(GL_VIEWPORT, viewport);
    pos(&_dragPosX, &_dragPosY, &_dragPosZ, x, y, viewport);
}

void Motion(int x, int y)
{
    bool changed = false;

    const int dx = x - _mouseX;
    const int dy = y - _mouseY;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (dx == 0 && dy == 0)
	return;

    if (_mouseMiddle || (_mouseLeft && _mouseRight)) {

	glLoadIdentity();
	glTranslatef(0, 0, dy * 0.01);
	glMultMatrixd(_matrix);

	changed = true;
    } else if (_mouseLeft) {
	double ax, ay, az;
	double bx, by, bz;
	double angle;

	ax = dy;
	ay = dx;
	az = 0.0;
	angle = module(ax, ay, az) / (double) (viewport[2] + 1) * 180.0;

	/* determiner les axes de rotation */

	bx = _matrixI[0] * ax + _matrixI[4] * ay + _matrixI[8] * az;
	by = _matrixI[1] * ax + _matrixI[5] * ay + _matrixI[9] * az;
	bz = _matrixI[2] * ax + _matrixI[6] * ay + _matrixI[10] * az;

	glRotatef(angle, bx, by, bz);

	changed = true;
    } else if (_mouseRight) {
	double px, py, pz;

	pos(&px, &py, &pz, x, y, viewport);

	glLoadIdentity();
	glTranslatef(px - _dragPosX, py - _dragPosY, pz - _dragPosZ);
	glMultMatrixd(_matrix);

	_dragPosX = px;
	_dragPosY = py;
	_dragPosZ = pz;

	changed = true;
    }

    _mouseX = x;
    _mouseY = y;

    if (changed) {
	getMatrix();
	glutPostRedisplay();
    }
}


void Anime(void)
{
    if (xrotate || yrotate || zrotate) {
	glRotatef(1, xrotate, yrotate, zrotate);
	getMatrix();
	glutPostRedisplay();
    }

}

void Keyboard(unsigned char key, int x, int y)
{

    switch (key) {
    case 'h':
    case 'H':{
	    show_help = !show_help;
	    break;
	}
    case 'r':
    case 'R':{
	    int i;
	    /* initialiser la scene */
	    for (i = 0; i < 16; i++) {
		if (i == 0 || i == 5 || i == 10 || i == 15) {
		    _matrix[i] = 1;
		} else {
		    _matrix[i] = 0;
		}
	    }
	    prev_z = 0;
	    glLoadIdentity();
	    glMultMatrixd(_matrix);
	    getMatrix();
	    break;
	}
    case 'a':
    case 'A':{
	    show_axis = !show_axis;
	    break;
	}
    case 'b':
    case 'B':
	{
	    animation = !animation;
	    if (animation)
		glutIdleFunc(Anime);
	    else
		glutIdleFunc(NULL);
	    break;
	}
    case 'x':
    case 'X':
	{
	    xrotate = !xrotate;
	    break;
	}
    case 'y':
    case 'Y':
	{
	    yrotate = !yrotate;
	    break;
	}
    case 'z':
    case 'Z':
	{
	    zrotate = !zrotate;
	    break;
	}

    case 'l':
    case 'L':
	lighting = !lighting;
	break;
    case 'w':
    case 'W':{
	    filaire = !filaire;
	    break;
	}
    case 'd':
    case 'D':{
	    faces = !faces;
	    break;
	}
    case 's':
    case 'S':{
	    smooth = !smooth;
	    break;
	}
    case 27:
     exit(0);
	break;
    default:{
	    fflush(stdin);
	    break;
	}			
    }
    fflush(stdin);	
    glutPostRedisplay();
}



double module(double x, double y, double z)
{
    return sqrt(x * x + y * y + z * z);
}

void pos(double *px, double *py, double *pz, const int x, const int y,
	 const int *viewport)
{
    /*
       Informations de projection et de viewport pour 
       à partir de la position de la souris.
     */

    *px = (double) (x - viewport[0]) / (double) (viewport[2]);
    *py = (double) (y - viewport[1]) / (double) (viewport[3]);

    *px = _left + (*px) * (_right - _left);
    *py = _top + (*py) * (_bottom - _top);
    *pz = _zNear;
}

void getMatrix()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, _matrix);
    invertMatrix(_matrix, _matrixI);
}


void DrawModel(void)
{

    mode = GLM_NONE;	/* Initialiser le mode */

    if (smooth)
	mode = mode | GLM_SMOOTH;
    else
	mode = mode | GLM_FLAT;

    if (faces)
	mode = mode | GLM_2_SIDED;
	
	mode = mode | GLM_MATERIAL;
   	mode = mode | GLM_TEXTURE;


    glPushMatrix();
    
    if (pmodel)
	glmDraw(pmodel, mode);
    glPopMatrix();
}

void DrawAxis(float scale)
{
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glScalef(scale, scale, scale);

    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(.8f, 0.05f, 0.0);
    glVertex3f(1.0, 0.25f, 0.0);	/*  afficher X sue l'axe */
    glVertex3f(0.8f, .25f, 0.0);
    glVertex3f(1.0, 0.05f, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);	/*  X  */

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);	/*  Y  */

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);	/*  Z  */
    glEnd();
    
    if (lighting)
	glEnable(GL_LIGHTING);
	
    if (lighting)
	glEnable(GL_TEXTURE_2D);
	
    glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
}

void HelpDisplay(GLint ww, GLint wh)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    /*  mode projection*/
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, ww, 0, wh);
    glScalef(1, -1, 1);
    glTranslatef(0, -wh, 0);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glLoadIdentity();
    linestart = 10;

    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font, "Menu .... ");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font, "---------");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "H/h = Affichage du Help");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "Esc = Quitter ");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "R/r = Initialiser position ");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "A/a = Afficher axes ");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "W/w = Solide/ fil de fer ");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "D/d = afficher les deux faces ");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "S/s = Lambert/Goraud");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "L/l = Activer l'illumination");
    HelpRenderBitmapString(30, linestart +=
			   linespace, (void *) Help_Font,
			   "B/b = Animation ");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    if (lighting)
	   glEnable(GL_LIGHTING);
	   
    glEnable(GL_TEXTURE_2D);
}

void HelpRenderBitmapString(float x, float y, void *font, char *string)
{
    char *c;
    /*  position*/
    glRasterPos2f(x, y);
    /*  afficher texte caractère par caractère */
    for (c = string; *c != '\0'; c++) {
     	glutBitmapCharacter(font, *c);
    }
}

void Display(void)
{

    if (lighting) {
    	glEnable(GL_LIGHTING);
     	glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHTING);
	    glDisable(GL_LIGHT0);
    }

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	glLoadIdentity();
   
   /* Center l'objet sur Z */
	glTranslatef(0, 0, centerZ);
	glMultMatrixd(_matrix);

	if (show_axis)
	    DrawAxis(1.0f);
	    
	if (filaire)		
	    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else		
	    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	DrawModel();

	glPopMatrix();

    if (show_help)
     	HelpDisplay(ww, wh);
    glutSwapBuffers();
}

/*  Calcule de la matrice inverse */
void invertMatrix(const GLdouble * m, GLdouble * out)
{

#define MAT(m,r,c) (m)[(c)*4+(r)]
#define m11 MAT(m,0,0)
#define m12 MAT(m,0,1)
#define m13 MAT(m,0,2)
#define m14 MAT(m,0,3)
#define m21 MAT(m,1,0)
#define m22 MAT(m,1,1)
#define m23 MAT(m,1,2)
#define m24 MAT(m,1,3)
#define m31 MAT(m,2,0)
#define m32 MAT(m,2,1)
#define m33 MAT(m,2,2)
#define m34 MAT(m,2,3)
#define m41 MAT(m,3,0)
#define m42 MAT(m,3,1)
#define m43 MAT(m,3,2)
#define m44 MAT(m,3,3)

    GLdouble det;
    GLdouble d12, d13, d23, d24, d34, d41;
    GLdouble tmp[16];		

    d12 = (m31 * m42 - m41 * m32);
    d13 = (m31 * m43 - m41 * m33);
    d23 = (m32 * m43 - m42 * m33);
    d24 = (m32 * m44 - m42 * m34);
    d34 = (m33 * m44 - m43 * m34);
    d41 = (m34 * m41 - m44 * m31);

    tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
    tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
    tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
    tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

    det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];


    if (det == 0.0) {
    } else {
	GLdouble invDet = 1.0 / det;


	tmp[0] *= invDet;
	tmp[1] *= invDet;
	tmp[2] *= invDet;
	tmp[3] *= invDet;

	tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
	tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
	tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
	tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

	d12 = m11 * m22 - m21 * m12;
	d13 = m11 * m23 - m21 * m13;
	d23 = m12 * m23 - m22 * m13;
	d24 = m12 * m24 - m22 * m14;
	d34 = m13 * m24 - m23 * m14;
	d41 = m14 * m21 - m24 * m11;

	tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
	tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
	tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
	tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
	tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
	tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
	tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
	tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

	memcpy(out, tmp, 16 * sizeof(GLdouble));
    }

#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44
#undef MAT
}

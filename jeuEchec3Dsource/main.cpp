
#include "structure.h"



/* MAIN ****************************************************************************/

void AnimeChangeRole();
int role=1;
int main(int argc, char **argv)
{ 
    glutInit(&argc, argv);
 	  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(windowsW, windowsH);	
    glutInitWindowPosition(300, 0);
    glutCreateWindow("JEUX D' ECHEC.Projet OpenGl du groupe 10: DIOUBATE SEKOU & ELHAMCHI SOUFIANE"); 
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glClearColor(0.0, 0.0, 0.0, 0.0);
	
    init();
    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutSpecialFunc(fleches);
    glutIdleFunc(NULL);
    glutMainLoop();
    return 0;
}


/* INITIALISATION ****************************************************************************/

GLfloat  vfloor[3][3];

void init(void){
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
  	    
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D); 
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
   	
	/* load up the model */
	        
  pmodel = glmReadOBJ(FICHIER_OBJ);
	 glmUnitize(pmodel);	//glmVertexNormals(obj, 90.0, GL_TRUE);
	
  glmScale(pmodel,1.6);
 
		GLfloat dimensions[3];
  glmDimensions(pmodel,  dimensions);
	 pmodel->position[2]=dimensions[1]/2; 
	 
	
	 for(int i=0;i<8;i++){
    place[i+1]=place[i+1]*(echelle/8);
  } 
  
  int n=0,m=0;
  for(int id=0;id<64;id++){
		   lesCases[id+1].id=id+1;
		   lesCases[id+1].idPion=-1;
		   
		   if((id)%8==0){
         n+=1;m=1;
		  // printf("\n");
     }
		   lesCases[id+1].nl=n;
		   lesCases[id+1].nc=m++;
		 // printf("%d(%d,%d) pion=%d\n",lesCases[id+1].id,lesCases[id+1].nl,lesCases[id+1].nc,lesCases[id+1].idPion);
   
   
                 // int j= lesCases[id+1].nc;  int k=lesCases[id+1].nl;  
   
   }
		  // printf("\n");
          
  GLMmaterial* mm=pmodel->materials ; 
	 group = pmodel->groups; int k=1;
    while(group) {
         group->position[0]=0.0;group->position[1]=0.0;
         for(int i=0;i<32;i++)  {      
            if(strstr(group->name,lesNoms[i])){
          			    if(i<16){
          				      	k=1;lesPions[i].camp=2;
                   }
              				else {
              					k=33;lesPions[i].camp=1;
                   }
               strcpy(lesPions[i].nom,lesNoms[i]);lesPions[i].nombrePosibilite=0;
               lesPions[i].ix=place[lesCases[i+k].nc];lesPions[i].iy=place[lesCases[i+k].nl];
           	   lesCases[i+k].idPion=i;lesPions[i].idCase=i+k; 
             //printf("--group %s : %d(%d,%d)\n",group->name,lesCases[i+k].id,lesCases[i+k].nl,lesCases[i+k].nc);
            }
       } 
     group = group->next;
    }  
    
    
	    
}
 
 
/* Callbacks ****************************************************************************/

void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    ww = w;    wh = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-(double) w / (double) h,(double) w / (double) h,-1.0,1.0,_zNear,_zFar); 
   gluPerspective(fovy, (double) w / (double) h, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}


void Display(void)
{  
    	glEnable(GL_LIGHTING);
   	glEnable(GL_LIGHT0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  | GL_STENCIL_BUFFER_BIT); 

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
	glPushMatrix(); 
	
 	 glLoadIdentity();
    /* Center l'objet sur Z  */
 	 glTranslatef(0, 0, centerZ);
   /* rotation avec les fleches */
   glRotated(alpha,0,1,0);
   glRotated(beta,1,0,0);
            
  DrawScene();   
  glPopMatrix();
 
    if (show_help)
     	HelpDisplay(ww, wh);
    glutSwapBuffers();
}



void DrawScene()
{
    glPushMatrix();
    
    glDisable(GL_LIGHTING);glDisable(GL_TEXTURE_2D);  
   
     //effet brouillard
   if(brouillard){
      GLint fogmode; //Initialisation de fogmode
      GLfloat fogcolor[4] = {0.5, 0.5, 0.5, 1} ; //Initialisation de la couleur (RGBA)
   
      glEnable(GL_FOG); //Cela permet d'activer le mode GL_FOG
      fogmode = GL_EXP ;
      glFogi(GL_FOG_MODE, fogmode); //On range le mode fogmode dans la variable GL_FOG_MODE 
      glFogfv(GL_FOG_COLOR, fogcolor); //On fait la meme chose pour la couleur et la variable GL_FOG_COLOR
      glFogf(GL_FOG_DENSITY, 0.05); //cela permet de definir la densite du brouillard (plus ou moins epais)
      glFogf(GL_FOG_START, 1.0);  //Debut du brouillard
      glFogf(GL_FOG_END, 5.0); //Fin du brouillar
    }
   else //desactiver effet brouillard
		    glDisable(GL_FOG);
		    
/**********************************/		    
 
//effect du reflet
    if(reflet){
       glDisable(GL_DEPTH_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
  
        glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D);
        
	 DrawGrille();
     
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);

        glStencilFunc(GL_EQUAL, 1, 0xffffffff); /* draw if ==1 */
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
   
    glPushMatrix();
          glScalef(1.0, -1.0, 1.0);
   	//glEnable(GL_LIGHTING);  glEnable(GL_TEXTURE_2D); 
          glmDraw22( pmodel, GLM_SMOOTH | GLM_2_SIDED | GLM_MATERIAL | GLM_TEXTURE); 
  	     glPopMatrix();
        
        glDisable(GL_STENCIL_TEST);
        
       
        glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D);
        DrawPlateau();
        
        }
        else
         DrawPlateau();
         
   	glEnable(GL_LIGHTING);  glEnable(GL_TEXTURE_2D); 
  	 glmDraw2( pmodel, GLM_SMOOTH | GLM_2_SIDED | GLM_MATERIAL | GLM_TEXTURE); 
        
/**********************************/		
//effect d ombre
   if(ombre){
        glPushMatrix();
        static GLfloat floorPlane[4];
        static GLfloat floorShadow[4][4];
        GLfloat light_pos[] = { .3, .5, .2, 0.0 };
        findPlane(floorPlane, vfloor[1], vfloor[2], vfloor[3]);
        shadowMatrix(floorShadow, floorPlane, light_pos);

            glMultMatrixf((GLfloat *) floorShadow);
           	glmDraw22( pmodel, GLM_NONE);
        glPopMatrix();
        }
    glPopMatrix();
}
 void drawCasePosible(){
      if(idPionPourCasePossible>=0 && lesPions[idPionPourCasePossible].nombrePosibilite>0){  
            int j,k;    
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
            glColor4f(.2,1.0,0.0,0.5);      
            for(int p=0;p<lesPions[idPionPourCasePossible].nombrePosibilite;p++){
                    if(lesPions[idPionPourCasePossible].posibilite[p]<=0)
                    break;
                    j= lesCases[lesPions[idPionPourCasePossible].posibilite[p]].nc; 
                     k=lesCases[lesPions[idPionPourCasePossible].posibilite[p]].nl;
                    glBegin(GL_POLYGON);  
                       glVertex3f(lePas*(j-1) -1.0,0.00001,lePas*(k-1)-1.0); 
                       glVertex3f(lePas*(j-1) -1.0,0.00001,lePas*k-1.0);
                       glVertex3f(lePas*j -1.0,0.00001,lePas*k -1.0);
                       glVertex3f(lePas*j -1.0,0.00001,lePas*(k-1)-1.0);
                    glEnd(); 
          }      
            glColor3f(1.,1.0,1.0);    
   }
 } 
void DrawGrille()
{ 
   int i,j,k;  bool caseNoir=true;
         for( i=1;i<=64;i++){ 
              
                 if((i-1)%8==0)
                      caseNoir=!caseNoir;   
                 if(caseNoir){ 
                    glBindTexture(GL_TEXTURE_2D, aTexture[0].texID); 
                 } 
                   else{ 
                     glBindTexture(GL_TEXTURE_2D, aTexture[1].texID);
                 }         
                  j= lesCases[i].nc;  k=lesCases[i].nl;     
                  glBegin(GL_POLYGON); 
                     	glTexCoord2i(1,1);glVertex3f(lePas*(j-1) -1.0,0.0,lePas*(k-1)-1.0); 
                     	glTexCoord2i(0,1);glVertex3f(lePas*(j-1) -1.0,0.0,lePas*k-1.0);
                     glTexCoord2i(0,0);glVertex3f(lePas*j -1.0,0.0,lePas*k -1.0);
                    	glTexCoord2i(1,0); glVertex3f(lePas*j -1.0,0.0,lePas*(k-1)-1.0);
                  glEnd();  
                  caseNoir=!caseNoir;
              } 
   drawCasePosible();
} 
void DrawPlateau(){
      
     glColor3f(1.0,1.0,1.0); 
   glClearColor (1.0,1.0,1.0,1.0);
   glEnable(GL_TEXTURE_2D);
     chargerTextures(0,"texture/sombre.tga");
    chargerTextures(1,"texture/wood_wide1.tga"); 
  glBindTexture(GL_TEXTURE_2D, aTexture[0].texID);  
  
   //les bords superieur
     glBegin(GL_QUADS);
          glTexCoord2i(1,1);glVertex3f(-1.0,0.0,-1.0); //a  ----> 1point de la grille
          glTexCoord2i(0,1);glVertex3f(-1.058,0.0,-1.058); //A
          glTexCoord2i(0,0);glVertex3f(-1.058,0.0,1.058); //B
          glTexCoord2i(1,0);glVertex3f(-1.0,0.0,1.0);//b  ----> 2point de la grille
          
          glTexCoord2i(1,1);glVertex3f(-1.0,0.0,1.0);//b
          glTexCoord2i(0,1); glVertex3f(-1.058,0.0,1.058);//B
          glTexCoord2i(0,0);glVertex3f(1.058,0.0,1.058);//C
          glTexCoord2i(1,0);glVertex3f(1.0,0.0,1.0);//c  ----> 3point de la grille
          
          glTexCoord2i(1,1);glVertex3f(1.0,0.0,1.0);//c
          glTexCoord2i(0,1);glVertex3f(1.058,0.0,1.058);//C
          glTexCoord2i(0,0);glVertex3f(1.058,0.0,-1.058);//D
          glTexCoord2i(1,0);glVertex3f(1.0,0.0,-1.0);//d
          
          glTexCoord2i(1,1);glVertex3f(1.0,0.0,-1.0);//d  ----> 4point de la grille
          glTexCoord2i(0,1);glVertex3f(1.058,0.0,-1.058);//D
          glTexCoord2i(0,0);glVertex3f(-1.058,0.0,-1.058);//A
          glTexCoord2i(1,0);glVertex3f(-1.0,0.0,-1.0); //a
     glEnd();
      
     //les bords lateral 
     glBegin(GL_QUADS);
    		   	glTexCoord2i(1,1);glVertex3f(-1.058,0.0,-1.058);//A
          glTexCoord2i(0,1);glVertex3f(-1.058,-0.02,-1.058); //A2
          glTexCoord2i(0,0);glVertex3f(-1.058,-0.02,1.058); //B2
          glTexCoord2i(1,0); glVertex3f(-1.058,0.0,1.058);//B 
         
          glTexCoord2i(1,1);glVertex3f(-1.058,0.0,1.058);//B
          glTexCoord2i(0,1);glVertex3f(-1.058,-0.02,1.058);//B2
          glTexCoord2i(0,0);glVertex3f(1.058,-0.02,1.058);//C2
          glTexCoord2i(1,0);glVertex3f(1.058,0.0,1.058);//C 
        
          glTexCoord2i(1,1);glVertex3f(1.058,0.0,1.058);//C
          glTexCoord2i(0,1);glVertex3f(1.058,-0.02,1.058);//C2
          glTexCoord2i(0,0);glVertex3f(1.058,-0.02,-1.058);//D2
          glTexCoord2i(1,0);glVertex3f(1.058,0.0,-1.058);//D
         
          glTexCoord2i(1,1);glVertex3f(1.058,0.0,-1.058);//D
          glTexCoord2i(0,1);glVertex3f(1.058,-0.02,-1.058);//D2
          glTexCoord2i(0,0);glVertex3f(-1.058,-0.02,-1.058); //A2
          glTexCoord2i(1,0); glVertex3f(-1.058,0.0,-1.058);  //A
     glEnd(); 
   
		 if(reflet){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0,1.0,1.0,0.5); 
        glClearColor (1.0,1.0,1.0,0.5); 
    }
    
  //la face posterieur
	    glBegin(GL_QUADS); 
    			glTexCoord2i(1,1); glVertex3f(-1.058,-0.02,-1.058); 
    			glTexCoord2i(0,1);	glVertex3f(-1.058,-0.02,1.058);	
    			glTexCoord2i(0,0); glVertex3f(1.058,-0.02,1.058);
    			glTexCoord2i(1,0); glVertex3f(1.058,-0.02,-1.058);
     glEnd();
      	     
		 DrawGrille();
		 
		 if(reflet){
     glDisable(GL_BLEND); 
     glColor3f(1.0,1.0,1.0); 
   glClearColor (1.0,1.0,1.0,1.0);
     }
   
     
   DrawSky();   
    
		  
		 glDeleteTextures(1, &aTexture[0].texID);
		 glDeleteTextures(1, &aTexture[1].texID);  
		 
		 
   glDisable(GL_TEXTURE_2D);
       
 //le plan de pojection de l'ombre
 
 vfloor[0][0]=-1.058;  vfloor[0][1]=0.0001;  vfloor[0][2]=-1.058;
 vfloor[1][0]=-1.058;  vfloor[1][1]=0.0001;  vfloor[1][2]=1.058;
 vfloor[2][0]=1.058;   vfloor[2][1]=0.0001;  vfloor[2][2]=1.058;
 vfloor[3][0]=1.058;   vfloor[3][1]=0.0001;  vfloor[3][2]=-1.058;  
   
 }
 
void DrawSky(){
    
      float cote=17.0;
    glBindTexture(GL_TEXTURE_2D, aTexture[1].texID);
      //top
    glBegin(GL_QUADS);
			   glTexCoord2i(1,1);	glVertex3f( cote, cote, cote);
			   glTexCoord2i(0,1); glVertex3f( cote, cote,-cote);	
			   glTexCoord2i(0,0); glVertex3f(-cote, cote,-cote);
		   	glTexCoord2i(1,0);	glVertex3f(-cote, cote, cote);
   	glEnd();

    //down	
    glBegin(GL_QUADS);		
	     glTexCoord2i(1,1); glVertex3f( cote,-cote, cote);
			   glTexCoord2i(0,1); glVertex3f( cote,-cote,-cote);
			   glTexCoord2i(0,0); glVertex3f(-cote,-cote,-cote);		
		   	glTexCoord2i(1,0); glVertex3f(-cote,-cote, cote);
	   glEnd();

    // front	
    glBegin(GL_QUADS);	
			   glTexCoord2i(1,1); glVertex3f(-cote,-cote,  cote);
			   glTexCoord2i(0,1); glVertex3f( cote,-cote, cote);
			   glTexCoord2i(0,0); glVertex3f( cote, cote, cote);
			   glTexCoord2i(1,0); glVertex3f(-cote, cote, cote);	
		  glEnd();

    // Back	
    glBegin(GL_QUADS);
		   	glTexCoord2i(1,1); glVertex3f(-cote,-cote,-cote);
			   glTexCoord2i(0,1); glVertex3f( cote,-cote,-cote);
      glTexCoord2i(0,0); glVertex3f( cote, cote,-cote);
			   glTexCoord2i(1,0); glVertex3f(-cote, cote,-cote);
		 	glEnd(); 
	
    // Left
    glBegin(GL_QUADS);
			   glTexCoord2i(1,1); glVertex3f(-cote,-cote,-cote);
		    glTexCoord2i(0,1); glVertex3f(-cote, cote,-cote);
      glTexCoord2i(0,0); glVertex3f(-cote, cote, cote);
      glTexCoord2i(1,0); glVertex3f(-cote,-cote, cote);
		  glEnd();
	
    // Right	
    glBegin(GL_QUADS); 
			   glTexCoord2i(1,1); glVertex3f( cote,-cote, cote);
			   glTexCoord2i(0,1); glVertex3f( cote,-cote,-cote);	
		    glTexCoord2i(0,0); glVertex3f( cote, cote,-cote);
      glTexCoord2i(1,0);	glVertex3f( cote, cote, cote);
	  	glEnd();
  
  
} 

/* setPicking ****************************************************************************/

void setPicking(int x, int y)
{
 	GLuint buff[64] = {0};
 	GLint hits, view[4];
 	int id; 
 	glSelectBuffer(64, buff); 
 	glGetIntegerv(GL_VIEWPORT, view); 
 	glRenderMode(GL_SELECT); 
 	glInitNames(); 
 	glPushName(0); 
 	glMatrixMode(GL_PROJECTION);
 	glPushMatrix();
		glLoadIdentity(); 
		gluPickMatrix(x, y, 1.0, 1.0, view);
		
		
 	//Reshape	
    //glOrtho(-(double) ww / (double) wh,(double) ww / (double) wh,-1.0,1.0,_zNear,_zFar); 
 gluPerspective(fovy, (double) ww / (double) wh, 1, 1000);
    
		glMatrixMode(GL_MODELVIEW); 
		glutSwapBuffers();
 		
 	//display
 	glLoadIdentity(); 
   	glTranslatef(0, 0, centerZ); 
    glRotated(alpha,0,1,0);glRotated(beta,1,0,0);
         
    /********redesine les objets (les case de la grille) en les nommant(en le numerotant)******/
   
   	  int i,j,k;  
         for( i=1;i<=64;i++){    
               glLoadName(lesCases[i].id); //pour nommé (numeroté) les case
                  j= lesCases[i].nc;  k=lesCases[i].nl;       
                  glBegin(GL_POLYGON); 
                     glVertex3f(lePas*(j-1) -1.0,0.0,lePas*(k-1)-1.0); 
                     glVertex3f(lePas*(j-1) -1.0,0.0,lePas*k-1.0);
                     glVertex3f(lePas*j -1.0,0.0,lePas*k -1.0);
                     glVertex3f(lePas*j -1.0,0.0,lePas*(k-1)-1.0);
                  glEnd();  
              } 
               
 /*********************************************************************/
   	  
    glutSwapBuffers();
    
 		glMatrixMode(GL_PROJECTION);
 	glPopMatrix(); 
 	hits = glRenderMode(GL_RENDER); 
 	getPickingResult(hits, buff); 
 	glMatrixMode(GL_MODELVIEW);
 }
 
 
/* getPickingResult ****************************************************************************/
 bool possible(){
   int i,min,max;
     if(caseDepart<caseArrivee){
         min=caseDepart;max=caseArrivee;
     }
     else{
         max=caseDepart;min=caseArrivee;
     }
     
     
 if(lesCases[caseDepart].nl==lesCases[caseArrivee].nl){
      for(i=min;i<max;i++){
        if(lesCases[i].idPion>=0){
            printf("\n PAs depl lign\n"); return false;
             }
      }                                               ;
     }
 else{
    if(lesCases[caseDepart].nc==lesCases[caseArrivee].nc){
          for(i=min;i<max;i+=9){
             if(lesCases[i].idPion>=0){
                 printf("\n PAs depl lign--%d\n",i); //return false;
                  }
           }                                                    ;
        }
    else {   
        if((lesCases[caseDepart].nl-lesCases[caseArrivee].nl)==(lesCases[caseDepart].nc-lesCases[caseArrivee].nc)){
             printf("\ndepl diag\n")     ;                                                ;
            }
            else{
                    printf("\ndepl cheaval\n")     ;  
                 }
        }
  }
}    
void getPickingResult(GLint hits, GLuint *names)
 { 
  
   if(!clicker){
  	  caseDepart=(int)names[3];
     if(caseDepart>0 && lesCases[caseDepart].idPion >= 0){// si la caseDepart contient un pion
  	      clicker=true;
      			printf("\nPion '%s' du Camp %d\n-caseDepart:%d\n",lesNoms[lesCases[caseDepart].idPion],lesPions[lesCases[caseDepart].idPion].camp,lesCases[caseDepart].id); 
      			
         printf("-caseArrivee possible : ");
      			 casePossible(lesCases[caseDepart].idPion);
         printf("\n");
      }
      else{
   	  caseDepart=-1;
      printf("\nCase:%d (ligne:%d , colonne:%d)\n--------",(int)names[3],lesCases[(int)names[3]].nl,lesCases[(int)names[3]].nc);
      }
  	}
  	else{
     	  clicker=false;
     	  caseArrivee=(int)names[3];
       int idp=lesCases[caseDepart].idPion;
       int idp2=lesCases[caseArrivee].idPion;
     	   
          for(int i=0;i< lesPions[idp].nombrePosibilite;i++){
                 
                  if(lesPions[idp].posibilite[i] == caseArrivee && lesPions[idp].posibilite[i]!=caseDepart ){ 
                 	    
                         /*DEPLACEMENT*/
                		       
                         lesCases[caseDepart].idPion=-1;
                         lesCases[caseArrivee].idPion=idp; 
                      		                           
                      	   group = pmodel->groups;
                          while(group) {      
                          	   if(strstr(group->name,lesPions[idp].nom)){ 
                              		 group->position[1]=place[lesCases[caseArrivee].nl]-lesPions[idp].iy;
                              		 group->position[0]=place[lesCases[caseArrivee].nc]-lesPions[idp].ix;
                             		//  printf("\n idp=%s : (%f,%f)\n",group->name,group->position[0],group->position[1]);
                                 ///glutPostRedisplay();
                            		}
                          	   if(strstr(group->name,lesPions[idp2].nom)){ 
                              group->position[0]=2000;group->position[1]=2000;
                              }
                         group = group->next;
                         } 
                         printf("-caseArrivee:%d\n",caseArrivee);

                         break;
                    }        
              }
    
   	  caseDepart=-1;caseArrivee=-1;
     			lesPions[lesCases[caseDepart].idPion].nombrePosibilite=0; idPionPourCasePossible=-1;  
 	  }
   glutPostRedisplay();
}

void AnimeChangeRole2()
{
 float pas=3;
 if(role%2==0)
 {
  if(alpha<180.0-alpha2) alpha+=pas;
  if(0<beta2)
  {
      beta-=pas;
      if(beta<-beta2)beta=-beta2;
  }
  else
  {
      beta+=pas;
      if(-beta2<beta)beta=-beta2;
  }
 }
 else
 {
  if(alpha<360.0&&alpha!=0) alpha+=pas;
  if(alpha>360) alpha=0;
  
  if(0<beta2)
  {
      beta-=pas;
      if(beta<-beta2)beta=-beta2;
  }
  else
  {
      beta+=pas;
      if(-beta2<beta)beta=-beta2;
  }
 if(alpha>=360.0) alpha=0.0; 
 }
//  Sleep();
	 glutPostRedisplay();
	 if(alpha==180.0 || alpha==0)
	  glutIdleFunc(NULL);
}


void AnimeChangeRole22()
{ 
       if(beta!=-beta2){
           if(beta>-beta2)
               beta-=2;
              else
               beta+=2;
         }
       else{
       beta=-beta2; 
       }
       if(alpha!=180-alpha2){
           if(alpha>180-alpha2)
               beta-=2;
              else 
               alpha+=2;
         }
        else{
        alpha=180-alpha2; 
        }
        Sleep(500);
       /*if(alpha>360.0) alpha=0.0;             
           if(alpha<0.0)   alpha=360.0;            
          if(beta>360.0) beta=0.0;            
           if(beta<0.0)   beta=360.0;  */
      	 glutPostRedisplay(); 
}
/* posibilite *************************


             if(lesCases[i*8+j].idPion!=-1){//si la casa est oqp
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                  break;
               }
***************************************************/

void posibiliteDuTour(int idPion){ 
   			lesPions[idPion].nombrePosibilite=0;
      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=caseDepart; 
   			
   			//position posible en horizontal     
      int i=lesCases[caseDepart].nl-1; 
     for(int j=lesCases[caseDepart].nc-1;j>=1;j--){//gauche
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          }
     for(int j=lesCases[caseDepart].nc+1;j<=8;j++){//droit
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          } 
    	//position posible en vertical    
       int j=lesCases[caseDepart].nc; 
     for(int i=lesCases[caseDepart].nl-2;i>=1;i--){//haut
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          }      
     for(int i=lesCases[caseDepart].nl;i<=8;i++){//bas
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          } 
  }
void posibiliteDuPion1(int idPion){ 
   			lesPions[idPion].nombrePosibilite=0; 
      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=caseDepart;    
      int i=lesCases[caseDepart].nl-2;    
      int j=lesCases[caseDepart].nc; 
    	//position posible d'une case en vertical devant lui
      if(lesCases[i*8+j].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[i*8+j].idPion].camp)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
        
      if(lesCases[caseDepart].nl==7) //pour le 1er coup
      if(lesCases[(i-1)*8+j].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[(i-1)*8+j].idPion].camp)
        lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[(i-1)*8+j].id; 
  }
void posibiliteDuPion2(int idPion){ 
   			lesPions[idPion].nombrePosibilite=0;   
      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=caseDepart;  
      int i=lesCases[caseDepart].nl;    
      int j=lesCases[caseDepart].nc; 
    	//position posible d'une case en vertical devant lui
      if(lesCases[i*8+j].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[i*8+j].idPion].camp)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
        
      if(lesCases[caseDepart].nl==2) //pour le 1er coup
      if(lesCases[(i+1)*8+j].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[(i+1)*8+j].idPion].camp)
        lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[(i+1)*8+j].id; 
  }
void posibiliteDeLaDame(int idPion){ 
   			lesPions[idPion].nombrePosibilite=0;   
      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=caseDepart;  
      //position posible en horizontal     
      int i=lesCases[caseDepart].nl-1; 
     for(int j=lesCases[caseDepart].nc-1;j>=1;j--){//gauche
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          }
     for(int j=lesCases[caseDepart].nc+1;j<=8;j++){//droit
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          } 
    	//position posible en vertical    
       int j=lesCases[caseDepart].nc; 
     for(int i=lesCases[caseDepart].nl-2;i>=1;i--){//haut
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          }      
     for(int i=lesCases[caseDepart].nl;i<=8;i++){//bas
             if(lesCases[i*8+j].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[i*8+j].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id;  break;
			                 }
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+j].id; 
          } 
          
          
     int idCase;
     /*diagonale*/ 
     i=lesCases[caseDepart].nl-2; j=lesCases[caseDepart].nc-1;
     while(i>0 && j>0){//gauche-haut
            idCase= i*8+j; i--;j--;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  
          }  
     i=lesCases[caseDepart].nl-2;j=lesCases[caseDepart].nc+1;
     while(i>0 && j<8){//droit-haut
            idCase= i*8+j;i--;j++;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  
          }
     i=lesCases[caseDepart].nl;j=lesCases[caseDepart].nc-1;
     while(i<8 && j>0){//gauche-bas
            idCase= i*8+j;i++;j--;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  
          } 
     i=lesCases[caseDepart].nl;j=lesCases[caseDepart].nc+1;
     while(i<8 && j<8){//droit-bas
            idCase= i*8+j;i++;j++;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
          } 
  } 
void posibiliteDuRoi(int idPion){ 
   			lesPions[idPion].nombrePosibilite=0;    
      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=caseDepart; 
     int idCase;
     for(int i=lesCases[caseDepart].nl-1;i<=lesCases[caseDepart].nl+1;i++){
     for(int j=lesCases[caseDepart].nc-1;j<=lesCases[caseDepart].nc+1;j++){
            idCase= (i-1)*8+j;
            if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
             if(idCase>=1 && idCase<=64)
             lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
          }  
     } 
  } 
  
void posibiliteDuCavalier(int idPion){
   			lesPions[idPion].nombrePosibilite=0;    
      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=caseDepart; 
      int i=lesCases[caseDepart].nl-1;    
      int j=lesCases[caseDepart].nc; 
    	 int idCase;
      //lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[i*8+1+j].id;
      idCase=(i-2)*8+(j-1);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;
      idCase=(i-2)*8+(j+1);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
      idCase=(i-1)*8+(j-2);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
      idCase=(i-1)*8+(j+2);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
      idCase=(i+1)*8+(j-2);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;
      idCase=(i+1)*8+(j+2);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
      idCase=(i+2)*8+(j-1);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
      idCase=(i+2)*8+(j+1);
      if(lesCases[idCase].idPion==-1 || lesPions[lesCases[caseDepart].idPion].camp!=lesPions[lesCases[idCase].idPion].camp)
        if(idCase>=1 && idCase<=64)
         lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  
  }
void posibiliteDuFou(int idPion){ 
     lesPions[idPion].nombrePosibilite=0;    
     lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=caseDepart; 
     int idCase,i,j;
     /*diagonale*/ 
     i=lesCases[caseDepart].nl-2; j=lesCases[caseDepart].nc-1;
     while(i>0 && j>0){//gauche-haut
            idCase= i*8+j; i--;j--;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  
          }  
     i=lesCases[caseDepart].nl-2;j=lesCases[caseDepart].nc+1;
     while(i>0 && j<8){//droit-haut
            idCase= i*8+j;i--;j++;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  
          }
     i=lesCases[caseDepart].nl;j=lesCases[caseDepart].nc-1;
     while(i<8 && j>0){//gauche-bas
            idCase= i*8+j;i++;j--;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  
          } 
     i=lesCases[caseDepart].nl;j=lesCases[caseDepart].nc+1;
     while(i<8 && j<8){//droit-bas
            idCase= i*8+j;i++;j++;
            if(lesCases[idCase].idPion>0)
                if( lesPions[lesCases[caseDepart].idPion].camp==lesPions[lesCases[idCase].idPion].camp)
                      break;
			             else{
                      lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id;  break;
			                 }
            if(idCase>=1 && idCase<=64)
              lesPions[idPion].posibilite[lesPions[idPion].nombrePosibilite++]=lesCases[idCase].id; 
          } 
            
  } 
  
/*************************************/  

void casePossible(int idPion){
     if(strstr(lesNoms[idPion],"tour"))
   			  posibiliteDuTour(idPion);
     if(strstr(lesNoms[idPion],"pion1"))
   			  posibiliteDuPion1(idPion);
     if(strstr(lesNoms[idPion],"pion2"))
   			  posibiliteDuPion2(idPion);
     if(strstr(lesNoms[idPion],"dame"))
   			  posibiliteDeLaDame(idPion);
     if(strstr(lesNoms[idPion],"roi"))
   			  posibiliteDuRoi(idPion);
     if(strstr(lesNoms[idPion],"cavalier"))
   			  posibiliteDuCavalier(idPion);
     if(strstr(lesNoms[idPion],"fou"))
   			  posibiliteDuFou(idPion);
       
     idPionPourCasePossible=idPion;
   	 for(int p=0;p<lesPions[idPion].nombrePosibilite;p++){ 
              printf("%d,", lesPions[idPion].posibilite[p]);
              }   
  }      
      
 


void Anime(void)
{
  alpha+=1;  if(alpha>360.0) alpha=0.0;
	 glutPostRedisplay();  Sleep(20);
}



void Mouse(int button, int state, int x, int y)
{  
      if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
 	     setPicking(x,wh - y); //Important: gl (0,0) ist bottom left but window coords (0,0) are top left so we have to change this!
}

void Motion(int x, int y)
{
 
}

 
void fleches(int key, int x, int y){
    
    switch(key){
    case GLUT_KEY_LEFT      : alpha+=2;  if(alpha>360.0) alpha=0.0; break;            
    case GLUT_KEY_RIGHT     : alpha-=2;  if(alpha<0.0)   alpha=360.0; break;            
    case GLUT_KEY_UP        : beta+=2;  if(beta>360.0) beta=0.0; break;            
    case GLUT_KEY_DOWN      : beta-=2;  if(beta<0.0)   beta=360.0; break;            
    //case GLUT_KEY_PAGE_UP   : anglez+=2;  if(anglez>360.0) anglez=0.0; break;            
    //case GLUT_KEY_PAGE_DOWN : anglez-=2;  if(anglez<0.0)   anglez=360.0; break;            
    
    default:;
    } 
    glutPostRedisplay();    
     
}
void Keyboard(unsigned char key, int x, int y)
{
 
    switch (key) {
      case 'h':
      case 'H':{
  	    show_help = !show_help;
  	    break;
  	    }  
      case 'b':
      case 'B':{
  	    brouillard = !brouillard;
  	    break;
  	    }    
      case 'f':
      case 'F':{
  	    reflet = !reflet;
  	    break;
  	    }
      case 'o':
      case 'O':{
  	    ombre = !ombre;
  	    break;
  	    }  
    case 'r':
    case 'R':
	{
	    animation = !animation;
	    if (animation)
    		glutIdleFunc(Anime);
    	    else
    		glutIdleFunc(NULL);
	    break;
	}
      case ' ':{
           if (fullscreen){
                glutFullScreen();
            }else{
               glutInitWindowSize(windowsW, windowsH);	
            }
            fullscreen =!fullscreen;
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
 



// structure pour gérer la texture
bool chargerTextures(int it, char* nom)
{
  	// Charger le fichier image tga et mettre les infos dans la structure
	// Hauteur, Largeur, bits par pixel.
	if (LoadTGA(&aTexture[it], nom)){ 
		// Créer la texture
		glGenTextures(1, &aTexture[it].texID);
		
        // Définir comme texture par défaut
		glBindTexture(GL_TEXTURE_2D, aTexture[it].texID);
		
        // Charger les données de l'image dans OpenGL et dans la mémoire de la carte vidéo
		glTexImage2D(GL_TEXTURE_2D, 0, aTexture[it].bpp / 8, aTexture[it].width, aTexture[it].height, 0, aTexture[it].type, GL_UNSIGNED_BYTE, aTexture[it].imageData);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		if (aTexture[it].imageData){
			// libérer la mémoire alloué par le chargeur 
			free(aTexture[it].imageData);			
		}
		return true;	
	}												
	else return false;
}

void HelpDisplay(GLint ww, GLint wh)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0,0.0,0.0);

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

    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"Pour deplacer un pion, cliquer dans la case qui le contient puis dans la case cible");
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"----------------------------------------");
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"H/h = Masquer ou Afficher l'Aide");  
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"O/o = Masquer ou Afficher l'ombre");  
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"B/b = Activer ou deactiver le brouillard"); 
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"F/f = Activer ou deactiver le reflet"); 
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"R/r = La rotation automatique"); 
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"Fleches = La rotation manuelle");
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"Espace = Affichage en plain ecran ");
    HelpRenderBitmapString(20, linestart +=linespace, (void *) Help_Font,"Esc = Quitter ");
    HelpRenderBitmapString(20, windowsH-40, (void *) Help_Font,"Realise par DIOUBATE SEKOU et ELHAMCHI SOUFIANE, Groupe 10, LSI");
    HelpRenderBitmapString(20, windowsH-20, (void *) Help_Font,"Encadre par Mr AITKBIR, proffesseur de traitement d'image");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);


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





























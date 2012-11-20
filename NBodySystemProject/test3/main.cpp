#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

// ---- Prototypes ----
void Render(void);
void Initialise();
void createParticles(void);
float RandomFloat(float a, float b);

typedef struct{
    
    float x;
    float y;
    float z;
    
}Particle;

const int numberParticles =  1000;
int i = 0;
Particle particles[numberParticles];

// ---- Main Function ----
int main(int argc, char** argv)
{
    
	glutInit(&argc,argv); 
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);  
	glutInitWindowSize(600,600);
	glutInitWindowPosition(0,0);
    createParticles();
	glutCreateWindow("GlutApp"); 
    
        
    glutDisplayFunc(Render);
	
	Initialise();
	glutMainLoop();
}

// ---- Render Function ----
void Render(void)
{   
    glClear(GL_COLOR_BUFFER_BIT); 
    
    glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
    
    for(int i=0; i<numberParticles;i++){
        glVertex3f(particles[i].x, particles[i].y, particles[i].z);
    }
    
	glEnd();
    
	glFlush();
    
}

// ---- Initialise Function ----
void Initialise()
{
	glClearColor (0.0, 0.0, 0.0, 0.0); 
}

void createParticles(void){
    srand(time(NULL));
    for(int i= 0; i < numberParticles;i++){
        particles[i].x = RandomFloat(-1.0f, 1.0f);
        particles[i].y = RandomFloat(-1.0f, 1.0f);
        particles[i].z = RandomFloat(-1.0f, 1.0f);;
        std::cout<<"x: "<<particles[i].x<<"y: "<<particles[i].y<<"z: "<<particles[i].z<<std::endl;
    }
}

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
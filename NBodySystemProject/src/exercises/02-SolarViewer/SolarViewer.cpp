//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS MeshViewer - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include "SolarViewer.h"
#include "../../utils/Mesh3DReader.h"
#include <stdlib.h>
#include <math.h>
#include <cstdlib>

//== IMPLEMENTATION ========================================================== 
#define GRAVITY 6.673*pow(10, -11)
#define NUMBER_PARTICLES 600
#define RANGE 100
#define SPEED 0.001


SolarViewer::
SolarViewer(const char* _title, int _width, int _height)
: TrackballViewer(_title, _width, _height)
{
  init();
}


//-----------------------------------------------------------------------------


void 
SolarViewer::
init()
{
  // initialize parent
  TrackballViewer::init();
   cntPlanet = 0;
    

  // set camera to look at world coordinate center
  set_scene_pos(Vector3(0.0, 0.0, 0.0), 2.0);
	
	// load mesh shader
	m_meshShaderDiffuse.create("diffuse.vs", "diffuse.fs");
	m_meshShaderTexture.create("tex.vs","tex.fs");
	
	m_showTextureStars = false;
	m_showTextureSun = false;
	m_showTextureEarth = false;
	m_showTextureMoon = false;
	
	currentTime = 0.0;
	isWatchOn = false;
	particlesNumber = 30;
	daysPerMiliSecond = 1 / 180.0;
	totalDaysElapsed = 0;
	m_geocentric = false;
	m_recSunlightInt = 1.0;
	
	m_moonScale = 10.0;

	m_starsScale = m_moonScale * 100.0;
    
	m_sunTransX =  m_sunScale + 900;
	m_sunTransY =  m_sunScale + 900;
	m_sunTransZ =  m_sunScale + 900;
    
	m_earthTrans = m_sunScale + 1250;
	m_moonTrans = m_earthTrans + m_earthScale + 200;
	
	m_starsScale = m_moonScale * 100.0;
    
	// initialize parent
    TrackballViewer::init();
    
	// create uniform cube
    for (int i = 0; i < NUMBER_PARTICLES; i++) {
        m_meshes.push_back(createCube());
    }
    m_meshes.push_back(createCube());
    m_meshes[NUMBER_PARTICLES]->setCurrentPosition(Vector3 (0.0, 0.0, 0.0));
    m_meshes[NUMBER_PARTICLES]->setSpeed(0.0);
    m_meshes[NUMBER_PARTICLES]->setMass(100000000000000.0);
    m_meshes[NUMBER_PARTICLES]->scaleObject( Vector3(5,5,5) );
    m_meshes[NUMBER_PARTICLES]->setID(NUMBER_PARTICLES);
	
    for (int i = 0; i < 200; i++) {
        // move cube to (0,0,1)
        float x = RandomFloat(-RANGE, RANGE);
        float y = RandomFloat(-RANGE, RANGE);
        float z = RandomFloat(-RANGE, RANGE);
        
        m_meshes[i]->setCurrentPosition(Vector3(x ,y ,z));
        std::cout<<"x: "<<m_meshes[i]->getCurrentPosition().x<<" y: "<<m_meshes[i]->getCurrentPosition().y<<" z: "<<m_meshes[i]->getCurrentPosition().z<<std::endl;
        m_meshes[i]->scaleObject( Vector3(15,15,15) );
        m_meshes[i]->translateWorld(m_meshes[i]->getCurrentPosition());
        m_meshes[i]->setID(i);
        
        float mass = RandomFloat(100.0, 10000.0);
        m_meshes[i]->setMass(mass);
        float speedX = RandomFloat(-SPEED, SPEED);
        float speedY = RandomFloat(-SPEED, SPEED);
        float speedZ = RandomFloat(-SPEED, SPEED);
        m_meshes[i]->setSpeed(Vector3(speedX, speedY, speedZ));
        std::cout<<"x: "<<m_meshes[i]->getSpeed().x<<" y: "<<m_meshes[i]->getSpeed().y<<" z: "<<m_meshes[i]->getSpeed().z<<std::endl;
        
    }
    //block2
    for (int i = 200; i < 400; i++) {
        // move cube to (0,0,1)
        float x = RandomFloat(-RANGE, RANGE);
        float y = RandomFloat(-RANGE, RANGE);
        float z = RandomFloat(-RANGE, RANGE);
        
        m_meshes[i]->setCurrentPosition(Vector3(x ,y ,z));
        std::cout<<"x: "<<m_meshes[i]->getCurrentPosition().x<<" y: "<<m_meshes[i]->getCurrentPosition().y<<" z: "<<m_meshes[i]->getCurrentPosition().z<<std::endl;
        m_meshes[i]->scaleObject( Vector3(15,15,15) );
        m_meshes[i]->translateWorld(m_meshes[i]->getCurrentPosition());
        m_meshes[i]->setID(i);
        
        float mass = RandomFloat(100000.0, 1000000.0);
        m_meshes[i]->setMass(mass);
        float speedX = RandomFloat(-SPEED, SPEED);
        float speedY = RandomFloat(-SPEED, SPEED);
        float speedZ = RandomFloat(-SPEED, SPEED);
        m_meshes[i]->setSpeed(Vector3(speedX, speedY, speedZ));
        std::cout<<"x: "<<m_meshes[i]->getSpeed().x<<" y: "<<m_meshes[i]->getSpeed().y<<" z: "<<m_meshes[i]->getSpeed().z<<std::endl;
        
    }
    //block3
    for (int i = 400; i < NUMBER_PARTICLES; i++) {
        // move cube to (0,0,1)
        float x = RandomFloat(-RANGE, RANGE);
        float y = RandomFloat(-RANGE, RANGE);
        float z = RandomFloat(-RANGE, RANGE);
        
        m_meshes[i]->setCurrentPosition(Vector3(x ,y ,z));
        std::cout<<"x: "<<m_meshes[i]->getCurrentPosition().x<<" y: "<<m_meshes[i]->getCurrentPosition().y<<" z: "<<m_meshes[i]->getCurrentPosition().z<<std::endl;
        m_meshes[i]->scaleObject( Vector3(15,15,15) );
        m_meshes[i]->translateWorld(m_meshes[i]->getCurrentPosition());
        m_meshes[i]->setID(i);
        
        float mass = RandomFloat(500000.0, 1000000000.0);
        m_meshes[i]->setMass(mass);
        float speedX = RandomFloat(-SPEED, SPEED);
        float speedY = RandomFloat(-SPEED, SPEED);
        float speedZ = RandomFloat(-SPEED, SPEED);
        m_meshes[i]->setSpeed(Vector3(speedX, speedY, speedZ));
        std::cout<<"x: "<<m_meshes[i]->getSpeed().x<<" y: "<<m_meshes[i]->getSpeed().y<<" z: "<<m_meshes[i]->getSpeed().z<<std::endl;
        
    }
    
	// set camera to look at world coordinate center
    set_scene_pos(Vector3(0.0, 0.0, 0.0), 2.0);
	
	// load cube shader
	m_cubeShader.create("cube.vs", "cube.fs");
		
}


//-----------------------------------------------------------------------------

void
SolarViewer::
load_mesh(const std::string& filenameObj, MeshType type)
{
	Vector3 bbmin, bbmax;
	double radius;
	Vector3 center;
	switch(type)
	{
        {case STARS:
			// load mesh from obj
			Mesh3DReader::read( filenameObj, m_Stars);
			
			// calculate normals
			if(!m_Stars.hasNormals())
				m_Stars.calculateVertexNormals();
			
			
			m_Stars.calculateBoundingBox(bbmin, bbmax);
			
			radius = 0.5*(bbmin - bbmax).length();
			center = 0.5*(bbmin + bbmax);
			
			m_Stars.scaleObject(Vector3(m_starsScale, m_starsScale, m_starsScale));
			radius *= 700.0;
			
			set_scene_pos(m_Stars.origin(), m_earthTrans*3.3);
			
			m_showTextureStars = m_Stars.hasUvTextureCoord();
			
			break;}
		{default:
			break;}
	}
	
}

Mesh3D* 
SolarViewer::
createCube()
{
    // initialize Mesh3D
    Mesh3D *cube = new Mesh3D();
    
	// setup uniform cube with side length 0.5 and center of cube being (0,0,0)
	std::vector< Vector3 > cubeVertices;
	std::vector< Vector3 > cubeNormals;
	std::vector< Vector3 > cubeColors;
	std::vector< unsigned int > cubeIndices;
	float d = 1.0;
    
	
	// front
	cubeVertices.push_back(Vector3(-d,-d, d));
	cubeVertices.push_back(Vector3( d,-d, d));
	cubeVertices.push_back(Vector3( d, d, d));
	cubeVertices.push_back(Vector3(-d, d, d));
	for(int k = 0; k < 4; k++) cubeNormals.push_back(Vector3(0,0,1));
	for(int k = 0; k < 4; k++) cubeColors.push_back(Vector3(0.8,0.3,0.3));
	cubeIndices.push_back(0);
	cubeIndices.push_back(1);
	cubeIndices.push_back(2);
	cubeIndices.push_back(0);
	cubeIndices.push_back(2);
	cubeIndices.push_back(3);
	
	cube->setIndices(cubeIndices);
	cube->setVertexPositions(cubeVertices);
	cube->setVertexNormals(cubeNormals);
	cube->setVertexColors(cubeColors);
    
    return cube;
}

//-------------------------------------------------------------------------------

void
SolarViewer::
keyboard(int key, int x, int y)
{
	switch (key)
	{			
		case 'h':
			printf("Help:\n");
			printf("'h'\t-\thelp\n");
			printf("'t'\t-\ttoggle texture\n");
			printf("'arrow keys\t-\tchange speed of rotation\n");
			break;
		case 't':
			m_showTextureStars = !m_showTextureStars;
			if(!m_Stars.hasUvTextureCoord()) m_showTextureStars = false;
			
			m_showTextureSun = !m_showTextureSun;
            
            
//                if(!m_particle.hasUvTextureCoord()) m_showTextureSun = false;
            
			
            break;
		case 'g':
			m_geocentric = !m_geocentric;
			break;
		case ' ':
			if(isWatchOn)
			{
				watch.stop();
				currentTime = 0.0;
			}
			else 
			{
				watch.start();
			}

			isWatchOn = !isWatchOn;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------


void
SolarViewer::
special(int key, int x, int y)
{
	switch (key)
	{			
		case GLUT_KEY_UP:
			daysPerMiliSecond += 0.001;
			if(daysPerMiliSecond > 0.1)
				daysPerMiliSecond = 0.1;
			break;
		case GLUT_KEY_DOWN:
			daysPerMiliSecond -= 0.001;
			if(daysPerMiliSecond < 0.001)
				daysPerMiliSecond = 0.001;
			break;
		default:
			TrackballViewer::special(key, x, y);
			break;
	}
	
	glutPostRedisplay();
}


//-----------------------------------------------------------------------------

void SolarViewer::idle()
{
	if(isWatchOn)
	{
		float prevTime = currentTime;
		currentTime = watch.stop();
        float timeElapsed = currentTime - prevTime;
//        totalTime = totalTime + timeElapsed;
//        std::cout<<"timeElapsed: "<<timeElapsed<<std::endl;
		float daysElapsed = daysPerMiliSecond * (currentTime-prevTime);
		totalDaysElapsed += daysElapsed;
		
		//Exercise 4.3 Rotate the earth and the moon
		
		//Optional: Rotate the planets
        std::cout<<"totalTIME"<<totalTime<<std::endl;
        move(daysElapsed);
        for (int i = 0; i < NUMBER_PARTICLES; i++) {
//            Vector3 distanceVertex = calculateVertexDistance(Vector3(0.0,0.0,0.0), m_meshes[i]->getCurrentPosition());
//            
//            float norme = sqrt(pow(distanceVertex.x, 2) + pow(distanceVertex.y,2) + pow(distanceVertex.z,2));
//            if(norme <1000){
           m_meshes[i]->translateWorld(m_meshes[i]->getCurrentPosition());
            
        }
		
		glutPostRedisplay();
	}
}

//-----------------------------------------------------------------------------

void 
SolarViewer::
draw_scene(DrawMode _draw_mode)
{

	
	// clear screen
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	m_meshShaderTexture.bind(); 
	
	// set parameters
	m_meshShaderTexture.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderTexture.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	
	
	//stars
	glDisable(GL_DEPTH_TEST);
	m_Stars.setIdentity();
	m_Stars.scaleObject(Vector3(m_starsScale, m_starsScale, m_starsScale));
	m_Stars.translateWorld(Vector3(m_camera.origin()));
	m_meshShaderTexture.setMatrix4x4Uniform("modelworld", m_Stars.getTransformation() );
	m_Stars.getMaterial(0).m_diffuseTexture.bind();
	m_meshShaderTexture.setIntUniform("texture", m_Stars.getMaterial(0).m_diffuseTexture.getLayer());
	draw_object(m_meshShaderTexture, m_Stars);
	glEnable(GL_DEPTH_TEST);
	
	m_meshShaderTexture.unbind();
    m_camera.origin();
    
    ////////////////////////////////////////////particles//////////////////////////////////////////
	
    // first bind the shader
	m_cubeShader.bind(); 
	
	// set parameters to send to the shader
	m_cubeShader.setMatrix4x4Uniform("WorldCameraTransform", m_camera.getTransformation().Inverse());
	m_cubeShader.setMatrix3x3Uniform("WorldCameraNormalTransform", m_camera.getTransformation().Transpose());
	m_cubeShader.setMatrix4x4Uniform("ProjectionMatrix", m_camera.getProjectionMatrix());
    
    for (std::vector<Mesh3D*>::iterator mIt = m_meshes.begin(); mIt != m_meshes.end(); ++mIt)
    {
        Mesh3D *cube = *mIt;
        
        // besides during we can apply transformations just before rendering:
        // save the original transformation of the cube
        Matrix4 originalTransformation = cube->getTransformation();
        
        // rotate the cube before rendering
        //cube->rotateObject(Vector3(0,1,0), M_PI/4);
        
        
        
        Vector3 lookAt,objToCamProj, objToCam, upAux;
        float angleCosine;
        
        
      //  glPushMatrix();
        
        // objToCamProj is the vector in world coordinates from the
        // local origin to the camera projected in the XZ plane
        objToCamProj.x = m_camera.origin().x - cube->origin().x;
        objToCamProj.y = 0;
        objToCamProj.z = m_camera.origin().z - cube->origin().z;
        
        // This is the original lookAt vector for the object
        // in world coordinates
        lookAt.x = 0;
        lookAt.y = 0;
        lookAt.z = 1;
        
        
        // normalize both vectors to get the cosine directly afterwards
        objToCamProj = objToCamProj.normalize();
        
        // easy fix to determine wether the angle is negative or positive
        // for positive angles upAux will be a vector pointing in the
        // positive y direction, otherwise upAux will point downwards
        // effectively reversing the rotation.
        
        upAux = lookAt.cross(objToCamProj);
        
        // compute the angle
        angleCosine = (lookAt.x*objToCamProj.x + lookAt.y*objToCamProj.y + lookAt.z*objToCamProj.z);
        
        // perform the rotation. The if statement is used for stability reasons
        // if the lookAt and objToCamProj vectors are too close together then
        // |angleCosine| could be bigger than 1 due to lack of precision
        if ((angleCosine < 0.99990) && (angleCosine > -0.9999)){
            //glRotatef(acos(angleCosine)*180/3.14,upAux[0], upAux[1], upAux[2]);
            cube->rotateObject(upAux, acos(angleCosine));
        
        }
        
        // so far it is just like the cylindrical billboard. The code for the
        // second rotation comes now
        // The second part tilts the object so that it faces the camera
        
        // objToCam is the vector in world coordinates from
        // the local origin to the camera
        
//        cout<<"origine camera : ("<<m_camera.origin().x<<", "<<m_camera.origin().y<<", "<<m_camera.origin().z<<")"<<std::endl;
        
        objToCam[0] = m_camera.origin().x - cube->origin().x;
        objToCam[1] = m_camera.origin().y - cube->origin().y;
        objToCam[2] = m_camera.origin().z - cube->origin().z;
        
        // Normalize to get the cosine afterwards
        objToCam = objToCam.normalize();
        
        // Compute the angle between objToCamProj and objToCam,
        //i.e. compute the required angle for the lookup vector
        
        angleCosine = (objToCamProj.x*objToCam.x + objToCamProj.y*objToCam.y + objToCamProj.z*objToCam.z);
        
        
        // Tilt the object. The test is done to prevent instability 
        // when objToCam and objToCamProj have a very small
        // angle between them
        
        Vector3 vec;
        vec.y=0;
        vec.z=0;
        
        
        cout<<"angle : "<<angleCosine<<std::endl;
        if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
            if (objToCam[1] < 0){
                vec.x=1;
                cube->rotateObject(vec, acos(angleCosine));
                
                //  glRotatef(acos(angleCosine)*180/3.14,1,0,0);
            }else{
                vec.x=-1;
                cube->rotateObject(vec, acos(angleCosine));
              //  glRotatef(acos(angleCosine)*180/3.14,-1,0,0);
            }

        //cube->rotateWorld(<#const Vector3 &_axis#>, <#float _angle#>)
        
        // send the model parameters to the shader
        m_cubeShader.setMatrix4x4Uniform("ModelWorldTransform", cube->getTransformation() );
        m_cubeShader.setMatrix3x3Uniform("ModelWorldNormalTransform", cube->getTransformation().Inverse().Transpose());
        
        // render the cube
        draw_cube(cube);
        
        // ((( Exercise 3.6 )))
        
        // then reset the original transformation
        cube->setTransformation( originalTransformation );
    }
	
    // for illustration render a small sphere at the world center
	Matrix4 ident;
	ident.loadIdentity();
	m_cubeShader.setMatrix4x4Uniform("ModelWorldTransform", ident );
	m_cubeShader.setMatrix3x3Uniform("ModelWorldNormalTransform", ident );
	
	glColor3f(1.0,1.0,1.0); // set sphere color to white
	glutSolidSphere( 0.05, 10, 10 );
	
	// finally, unbind the shader
	m_cubeShader.unbind();
	////////////////////////////////////////////////////////////////////////////////////////////////
	//-------------------------------
	
	m_meshShaderDiffuse.bind();
	
	//Exercise 4.4: Calculate the light position in camera coordinates
	Vector3 lightPosInCamera(0.0,0.0,0.0);
	
	m_meshShaderDiffuse.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderDiffuse.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	m_meshShaderDiffuse.setMatrix3x3Uniform("worldcameraNormal", m_camera.getTransformation().Transpose());
	m_meshShaderDiffuse.setVector3Uniform("lightposition", lightPosInCamera.x, lightPosInCamera.y, lightPosInCamera.z );
	m_meshShaderDiffuse.setVector3Uniform("lightcolor", m_recSunlightInt, m_recSunlightInt, m_recSunlightInt);

	
	
	
	
	
	//Optional: Draw the planets
	
	m_meshShaderDiffuse.unbind();
    

}

void
SolarViewer::
draw_cube(Mesh3D *mesh)
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer( 3, GL_DOUBLE, 0, mesh->getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh->getNormalPointer() );
	glColorPointer( 3, GL_DOUBLE, 0, mesh->getColorPointer() );
	
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, mesh->getVertexIndicesPointer() );

	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void SolarViewer::draw_object(Shader& sh, Mesh3D& mesh)
{
	
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation() );
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh.getNormalPointer() );
	glTexCoordPointer( 2, GL_DOUBLE, 0, mesh.getUvTextureCoordPointer() );
	
	for(unsigned int i = 0; i < mesh.getNumberOfParts(); i++)
	{
		glDrawElements( GL_TRIANGLES, mesh.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh.getVertexIndicesPointer(i) );
	}
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

void SolarViewer::draw_object(Shader& sh, Mesh3D& mesh, bool showTexture)
{
	
	sh.setMatrix4x4Uniform("modelworld", mesh.getTransformation() );
			
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if(showTexture)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
	glVertexPointer( 3, GL_DOUBLE, 0, mesh.getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh.getNormalPointer() );
	if(showTexture)
		glTexCoordPointer( 2, GL_DOUBLE, 0, mesh.getUvTextureCoordPointer() );
			
	for(unsigned int i = 0; i < mesh.getNumberOfParts(); i++)
	{
		sh.setIntUniform("useTexture", showTexture && mesh.getMaterial(i).hasDiffuseTexture());
		sh.setVector3Uniform("diffuseColor", 
							 mesh.getMaterial(i).m_diffuseColor.x, 
							 mesh.getMaterial(i).m_diffuseColor.y, 
							 mesh.getMaterial(i).m_diffuseColor.z );
		sh.setFloatUniform("specularExp", mesh.getMaterial(i).m_specularExp);
		if(showTexture && mesh.getMaterial(i).hasDiffuseTexture())
		{
			mesh.getMaterial(i).m_diffuseTexture.bind();
			sh.setIntUniform("texture", mesh.getMaterial(i).m_diffuseTexture.getLayer());
		}
		glDrawElements( GL_TRIANGLES, mesh.getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh.getVertexIndicesPointer(i) );
		if(showTexture && mesh.getMaterial(i).hasDiffuseTexture())
		{
			mesh.getMaterial(i).m_diffuseTexture.unbind();
		}
	}
			
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if(showTexture)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

float SolarViewer::RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

Vector3 SolarViewer::calculateForces(Mesh3D* p){
    Vector3 force ;
    for(int i = 0; i < NUMBER_PARTICLES + 1; i++){
        if(m_meshes[i]->getID() != p->getID()){
            
            if(i < NUMBER_PARTICLES){
                
                Vector3 distanceVertex = calculateVertexDistance(m_meshes[i]->getCurrentPosition(), p->getCurrentPosition());
                float norme = sqrt(pow(distanceVertex.x, 2) + pow(distanceVertex.y,2) + pow(distanceVertex.z,2));
                force += -((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(norme, 2))*distanceVertex.normalize();
            }else{
                Vector3 distanceVertex = calculateVertexDistance(m_meshes[i]->getCurrentPosition(), p->getCurrentPosition());
                float norme = sqrt(pow(distanceVertex.x, 2) + pow(distanceVertex.y,2) + pow(distanceVertex.z,2));
                
                if(norme > 500){
                 force += -10000000000*((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(norme, 2))*distanceVertex.normalize();
                }else if(norme < 100){
                    
                    force += -(1/10000)*((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(norme, 2))*distanceVertex.normalize();
                }else{
                    force += -((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(norme, 2))*distanceVertex.normalize();
                }
            }
            
//            std::cout<<"m1*m2*g: "<<m_meshes[i]->getMass()*p->getMass()*GRAVITY<<std::endl;
//            std:cout<<"norme: "<<norme<<std::endl;
//            std::cout<<"valueinsideforce: "<<((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(norme, 2))<<std::endl;
//            std::cout<<"pow x-----" << pow(distance(m_meshes[i]->getCurrentPosition().x,p->getCurrentPosition().x), 2)<<std::endl;
//            std::cout<<"force x: "<<force.x<<" force y: "<<force.y<<" force z: "<<force.z<<std::endl;
            
        }
    }
    
    return force;
    
}


Vector3 SolarViewer::calculateVertexDistance(Vector3 a, Vector3 b){
    return Vector3 (b.x-a.x, b.y-a.y, b.z-a.z);
}

void SolarViewer::move(float dt){
std:cout<<"timeElapsed"<<dt<<std::endl;
    for(int i= 0; i < NUMBER_PARTICLES;i++){
        
//        Vector3 currentPos = m_meshes[i]->getCurrentPosition();
//        Vector3 previousPosition = (currentPos.x, currentPos.y, currentPos.z);
        Vector3 forceTot = calculateForces(m_meshes[i]);
//        std::cout<<"Forcex: "<<forceTot.x<<" y: "<<forceTot.y<<" z: "<<forceTot.z<<std::endl;
        Vector3 distanceVertex = calculateVertexDistance(m_meshes[i]->getCurrentPosition(), Vector3(0.0f,0.0f,0.0f));
        float norme = sqrt(pow(distanceVertex.x, 2) + pow(distanceVertex.y,2) + pow(distanceVertex.z,2));
        //Calculate acceleration
        Vector3 a;
        a.x=100*forceTot.x/m_meshes[i]->getMass();
        a.y=100*forceTot.y/m_meshes[i]->getMass();
        a.z=100*forceTot.z/m_meshes[i]->getMass();
//        std::cout<<"ACCEx: "<<a.x<<" y: "<<a.y<<" z: "<<a.z<<std::endl;
        


        
//        if(norme > 100){
        //set the new speed
        m_meshes[i]->setSpeed(Vector3 (m_meshes[i]->getSpeed().x + dt*a.x, m_meshes[i]->getSpeed().y + dt*a.y, m_meshes[i]->getSpeed().z + dt*a.z));
//        std::cout<<"SPEEDx: "<<m_meshes[i]->getSpeed().x<<" y: "<<m_meshes[i]->getSpeed().y<<" z: "<<m_meshes[i]->getSpeed().z<<std::endl;
            
            
//            std::cout<<"GOOD: "<<norme<<"index: "<<i<<std::endl;
//        }else{
//            m_meshes[i]->setSpeed(Vector3(0.0,0.0,0.0));
//            std::cout<<"BAD: "<<norme<<"index: "<<i<<std::endl;
//        }

        
        //set the new position
        
        m_meshes[i]->setCurrentPosition(Vector3 (m_meshes[i]->getCurrentPosition().x + dt*m_meshes[i]->getSpeed().x, m_meshes[i]->getCurrentPosition().y + dt*m_meshes[i]->getSpeed().y, m_meshes[i]->getCurrentPosition().z + dt*m_meshes[i]->getSpeed().z));


//        std::cout<<"SPEEDx: "<<m_meshes[i]->getSpeed().x<<" y: "<<m_meshes[i]->getSpeed().y<<" z: "<<m_meshes[i]->getSpeed().z<<std::endl;
        
        
//        float nextPosX = previousPosition.x+m_meshes[i]->getSpeed().x*dt+(((1/2)*forceTot.x)/m_meshes[i]->getMass())*pow(dt, 2);
//        float nextPosY = previousPosition.y+m_meshes[i]->getSpeed().y*dt+(((1/2)*forceTot.y)/m_meshes[i]->getMass())*pow(dt, 2);
//        float nextPosZ = previousPosition.z+m_meshes[i]->getSpeed().z*dt+(((1/2)*forceTot.z)/m_meshes[i]->getMass())*pow(dt, 2);
//        Vector3 nextPostion = (nextPosX, nextPosY, nextPosZ);
//        m_meshes[i]->setCurrentPosition(nextPostion);
//        float speedX = distance(m_meshes[i]->getCurrentPosition().x, previousPosition.x)/dt;
//        float speedY = distance(m_meshes[i]->getCurrentPosition().y, previousPosition.y)/dt;
//        float speedZ = distance(m_meshes[i]->getCurrentPosition().z, previousPosition.z)/dt;
//        std::cout<<"x: "<<m_meshes[i]->getCurrentPosition().x<<" y: "<<m_meshes[i]->getCurrentPosition().y<<" z: "<<m_meshes[i]->getCurrentPosition().z<<std::endl;
//        std::cout<<"speed x: "<<m_meshes[i]->getSpeed().x<<" speed y: "<<m_meshes[i]->getSpeed().y<<" speed z: "<<m_meshes[i]->getSpeed().z<<std::endl;
    }
     std::cout<<"CAMx: "<<m_camera.origin().x<<std::endl;
    std::cout<<"CAMy: "<<m_camera.origin().y<<std::endl;

    std::cout<<"CAMz: "<<m_camera.origin().z<<std::endl;


    glutPostRedisplay();
    
    
    
    
    
    
}









//=============================================================================

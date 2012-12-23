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
#define NUMBER_PARTICLES 550
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
//        std::cout<<"x: "<<m_meshes[i]->getCurrentPosition().x<<" y: "<<m_meshes[i]->getCurrentPosition().y<<" z: "<<m_meshes[i]->getCurrentPosition().z<<std::endl;
        
        m_meshes[i]->translateWorld(m_meshes[i]->getCurrentPosition());
        m_meshes[i]->setID(i);
        
        float mass = RandomFloat(100.0, 10000.0);
        m_meshes[i]->setMass(mass);
        
        m_meshes[i]->scaleObject( Vector3(200,200,200) );
        float speedX = RandomFloat(-SPEED, SPEED);
        float speedY = RandomFloat(-SPEED, SPEED);
        float speedZ = RandomFloat(-SPEED, SPEED);
        m_meshes[i]->setSpeed(Vector3(speedX, speedY, speedZ));
//        std::cout<<"x: "<<m_meshes[i]->getSpeed().x<<" y: "<<m_meshes[i]->getSpeed().y<<" z: "<<m_meshes[i]->getSpeed().z<<std::endl;
        
    }
    //block2
    for (int i = 200; i < 400; i++) {
        // move cube to (0,0,1)
        float x = RandomFloat(-RANGE, RANGE);
        float y = RandomFloat(-RANGE, RANGE);
        float z = RandomFloat(-RANGE, RANGE);
        
        m_meshes[i]->setCurrentPosition(Vector3(x ,y ,z));

        
        m_meshes[i]->translateWorld(m_meshes[i]->getCurrentPosition());
        m_meshes[i]->setID(i);
        
        float mass = RandomFloat(100000.0, 1000000.0);
        m_meshes[i]->setMass(mass);
        
        m_meshes[i]->scaleObject( Vector3(200,200,200) );
        float speedX = RandomFloat(-SPEED, SPEED);
        float speedY = RandomFloat(-SPEED, SPEED);
        float speedZ = RandomFloat(-SPEED, SPEED);
        m_meshes[i]->setSpeed(Vector3(speedX, speedY, speedZ));

        
    }
    //block3
    for (int i = 400; i < NUMBER_PARTICLES; i++) {
        // move cube to (0,0,1)
        float x = RandomFloat(-RANGE, RANGE);
        float y = RandomFloat(-RANGE, RANGE);
        float z = RandomFloat(-RANGE, RANGE);
        
        m_meshes[i]->setCurrentPosition(Vector3(x ,y ,z));
        std::cout<<"x: "<<m_meshes[i]->getCurrentPosition().x<<" y: "<<m_meshes[i]->getCurrentPosition().y<<" z: "<<m_meshes[i]->getCurrentPosition().z<<std::endl;
//        m_meshes[i]->scaleObject( Vector3(20,20,20) );
        m_meshes[i]->translateWorld(m_meshes[i]->getCurrentPosition());
        m_meshes[i]->setID(i);
        
        float mass = RandomFloat(500000.0, 1000000000.0);
        m_meshes[i]->setMass(mass);
        
        m_meshes[i]->scaleObject( Vector3(200,200,200) );

        float speedX = RandomFloat(-SPEED, SPEED);
        float speedY = RandomFloat(-SPEED, SPEED);
        float speedZ = RandomFloat(-SPEED, SPEED);
        m_meshes[i]->setSpeed(Vector3(speedX, speedY, speedZ));

        
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
    MeshMaterial* material = new MeshMaterial;
    material->m_diffuseTexture.create("particle.tga");
	// setup uniform cube with side length 0.5 and center of cube being (0,0,0)
	std::vector< Vector3 > cubeVertices;
	std::vector< Vector3 > cubeNormals;
	std::vector< Vector3 > cubeColors;
	std::vector< unsigned int > cubeIndices;
    std::vector<Vector2> uvTextureCoord;
    
    uvTextureCoord.push_back(Vector2(0.0f, 0.0f));
    uvTextureCoord.push_back(Vector2(1.0f, 0.0f));
    uvTextureCoord.push_back(Vector2(1.0f, 1.0f));
    uvTextureCoord.push_back(Vector2(0.0f, 1.0f));
    
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
	
	cube->setIndices(cubeIndices, material);
	cube->setVertexPositions(cubeVertices);
	cube->setVertexNormals(cubeNormals);
	cube->setVertexColors(cubeColors);
    cube->setVertexUV(uvTextureCoord);
    
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
//        std::cout<<"timeElapsed: "<<timeElapsed<<std::endl;
		float daysElapsed = daysPerMiliSecond * (currentTime-prevTime);
		totalDaysElapsed += daysElapsed;
		
		//Exercise 4.3 Rotate the earth and the moon
		
		//Optional: Rotate the planets

        move(daysElapsed, totalDaysElapsed);
        for (int i = 0; i < NUMBER_PARTICLES; i++) {

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	m_meshShaderTexture.bind(); 
	
	// set parameters
	m_meshShaderTexture.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
	m_meshShaderTexture.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
	
	
	//stars
    //	glDisable(GL_DEPTH_TEST);
    //	m_Stars.setIdentity();
    //	m_Stars.scaleObject(Vector3(m_starsScale, m_starsScale, m_starsScale));
    //	m_Stars.translateWorld(Vector3(m_camera.origin()));
    //	m_meshShaderTexture.setMatrix4x4Uniform("modelworld", m_Stars.getTransformation() );
    //	m_Stars.getMaterial(0).m_diffuseTexture.bind();
    //	m_meshShaderTexture.setIntUniform("texture", m_Stars.getMaterial(0).m_diffuseTexture.getLayer());
    //	draw_object(m_meshShaderTexture, m_Stars);
    //	glEnable(GL_DEPTH_TEST);
    
    //    m_meshShaderTexture.setMatrix4x4Uniform("modelworld", m_Sun.getTransformation() );
    //	m_Sun.getMaterial(0).m_diffuseTexture.bind();
    //	m_meshShaderTexture.setIntUniform("texture", m_Sun.getMaterial(0).m_diffuseTexture.getLayer());
    //	draw_object(m_meshShaderTexture, m_Sun);
	
	m_meshShaderTexture.unbind();
    
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
        m_meshShaderTexture.bind();
        m_meshShaderTexture.setMatrix4x4Uniform("worldcamera", m_camera.getTransformation().Inverse());
        m_meshShaderTexture.setMatrix4x4Uniform("projection", m_camera.getProjectionMatrix());
        m_meshShaderTexture.setMatrix4x4Uniform("modelworld", cube->getTransformation());
        cube->getMaterial(0).m_diffuseTexture.bind();
        m_meshShaderTexture.setIntUniform("texture", m_Stars.getMaterial(0).m_diffuseTexture.getLayer());
        
        // besides during we can apply transformations just before rendering:
        // save the original transformation of the cube
        Matrix4 originalTransformation = cube->getTransformation();
        
        
        Vector3 lookAt,objToCamProj, objToCam, upAux;
        float angleCosine;
        
        
        
        
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
            cube->rotateObject(upAux, acos(angleCosine));
            
        }
        
        // so far it is just like the cylindrical billboard. The code for the
        // second rotation comes now
        // The second part tilts the object so that it faces the camera
        
        // objToCam is the vector in world coordinates from
        // the local origin to the camera
        
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
        
        
//        cout<<"angle : "<<angleCosine<<std::endl;
        if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
            if (objToCam[1] < 0){
                vec.x=1;
                cube->rotateObject(vec, acos(angleCosine));
                
            }else{
                vec.x=-1;
                cube->rotateObject(vec, acos(angleCosine));
                
            }
        
        
        
        // send the model parameters to the shader
        m_cubeShader.setMatrix4x4Uniform("ModelWorldTransform", cube->getTransformation() );
        m_cubeShader.setMatrix3x3Uniform("ModelWorldNormalTransform", cube->getTransformation().Inverse().Transpose());
        
        // render the cube
        draw_cube(m_meshShaderTexture, cube);
        m_meshShaderTexture.unbind();
        
        // ((( Exercise 3.6 )))
        
        // then reset the original transformation
        cube->setTransformation( originalTransformation );
    }
	
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
draw_cube(Shader& sh, Mesh3D *mesh)
{
    glDisable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    sh.setMatrix4x4Uniform("modelworld", mesh->getTransformation() );
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
	
	glVertexPointer( 3, GL_DOUBLE, 0, mesh->getVertexPointer() );
	glNormalPointer( GL_DOUBLE, 0, mesh->getNormalPointer() );
	glTexCoordPointer( 2, GL_DOUBLE, 0, mesh->getUvTextureCoordPointer() );
    
	
	for(unsigned int i = 0; i < mesh->getNumberOfParts(); i++)
	{
		glDrawElements( GL_TRIANGLES, mesh->getNumberOfFaces(i)*3, GL_UNSIGNED_INT, mesh->getVertexIndicesPointer(i) );
	}
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_DEPTH_TEST);
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
            float norme;
            if(i < NUMBER_PARTICLES){
                
                Vector3 distanceVertex = calculateVertexDistance(m_meshes[i]->getCurrentPosition(), p->getCurrentPosition());
                norme = sqrt(pow(distanceVertex.x, 2) + pow(distanceVertex.y,2) + pow(distanceVertex.z,2));
                force += -((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(norme, 2))*distanceVertex.normalize();
            }else{
                Vector3 distanceVertexSUN = calculateVertexDistance(m_meshes[i]->getCurrentPosition(), p->getCurrentPosition());
                float normeSUN = sqrt(pow(distanceVertexSUN.x, 2) + pow(distanceVertexSUN.y,2) + pow(distanceVertexSUN.z,2));
                
                if(normeSUN > 500){
                 force += -10000000000*((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(normeSUN, 2))*distanceVertexSUN.normalize();
                }else if(normeSUN < 100){
                    
                    force += -(1/10000)*((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(normeSUN, 2))*distanceVertexSUN.normalize();
                }else{
                    force += -((m_meshes[i]->getMass()*p->getMass()*GRAVITY)/pow(normeSUN, 2))*distanceVertexSUN.normalize();
                }
                
                
            }

            
        }
    }
    
    return force;
    
}


Vector3 SolarViewer::calculateVertexDistance(Vector3 a, Vector3 b){
    return Vector3 (b.x-a.x, b.y-a.y, b.z-a.z);
}

void SolarViewer::move(float dt, float totalTime){

    
    int timeToRescale = (int) (totalTime + 0.5);
    if(timeToRescale%50 == 0){
        rescaleOuterPosition();
        std::cout<<"rescaling----------------------"<<std::endl;
    }

    std::cout<<"totalDays: "<<totalTime<<" TIME ELAPSED: "<<dt<<std::endl;
    for(int i= 0; i < NUMBER_PARTICLES;i++){

        Vector3 forceTot = calculateForces(m_meshes[i]);
        Vector3 distanceVertex = calculateVertexDistance(m_meshes[i]->getCurrentPosition(), Vector3(0.0f,0.0f,0.0f));
        float norme = sqrt(pow(distanceVertex.x, 2) + pow(distanceVertex.y,2) + pow(distanceVertex.z,2));
        float normeForce = sqrt(pow(forceTot.x,2)+pow(forceTot.y,2)+pow(forceTot.z,2));
        if(normeForce > 10000000000){
            forceTot = forceTot/1000000;
            
        }
        //Calculate acceleration
        Vector3 a;
        a.x=100*forceTot.x/m_meshes[i]->getMass();
        a.y=100*forceTot.y/m_meshes[i]->getMass();
        a.z=100*forceTot.z/m_meshes[i]->getMass();

        m_meshes[i]->setSpeed(Vector3 (m_meshes[i]->getSpeed().x + dt*a.x, m_meshes[i]->getSpeed().y + dt*a.y, m_meshes[i]->getSpeed().z + dt*a.z));
        float normeSpeed = sqrt(pow(m_meshes[i]->getSpeed().x,2)+pow(m_meshes[i]->getSpeed().y,2)+pow(m_meshes[i]->getSpeed().z,2));
        

        if(i==0){
            std::cout<<"ID: "<<m_meshes[i]->getID()<<" Norme: "<<norme<<" Masse: "<<m_meshes[i]->getMass()<<" NORMEFORCE: "<< normeForce<<" x :"<<forceTot.x<<" y :"<<forceTot.y<<" z :"<<forceTot.z<<" NORMESPEED: "<<normeSpeed<<std::endl;

        }
            
            

        //set the new position
        
        m_meshes[i]->setCurrentPosition(Vector3 (m_meshes[i]->getCurrentPosition().x + dt*m_meshes[i]->getSpeed().x, m_meshes[i]->getCurrentPosition().y + dt*m_meshes[i]->getSpeed().y, m_meshes[i]->getCurrentPosition().z + dt*m_meshes[i]->getSpeed().z));

    }

    glutPostRedisplay();
    
    
    
}

void SolarViewer::rescaleOuterPosition(){
    for(int i = 0; i< NUMBER_PARTICLES;i++){
        Vector3 distanceVertex = calculateVertexDistance(m_meshes[i]->getCurrentPosition(), Vector3(0.0f,0.0f,0.0f));
        float norme = sqrt(pow(distanceVertex.x, 2) + pow(distanceVertex.y,2) + pow(distanceVertex.z,2));    
    
    if(norme > 100000){
        float rand = RandomFloat(-100, 100);
        Vector3 position = Vector3(rand,rand,rand);
        m_meshes[i]->setCurrentPosition(position);
        Vector3 nullSpeed = Vector3(0.0f,0.0f,0.0f);
        m_meshes[i]->setSpeed(nullSpeed);
        std::cout<<"rescaling particles: "<<i<< " at position x:"<<m_meshes[i]->getCurrentPosition().x<<" y: "<< m_meshes[i]->getCurrentPosition().y<<" z: "<< m_meshes[i]->getCurrentPosition().z<<std::endl;
    }
}




}





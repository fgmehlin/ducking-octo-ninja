//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS MeshViewer
//
//=============================================================================


#ifndef MESH_VIEWER_HH
#define MESH_VIEWER_HH


//== INCLUDES =================================================================
#include <string>
#include <vector>

#include "../../gl/TrackballViewer.h"
#include "../../gl/shader.h"
#include "../../gl/Mesh3D.h"
#include "../../gl/Light3D.h"
#include "../../gl/texture.h"
#include "../../utils/StopWatch.h"

//== CLASS DEFINITION =========================================================
	      
/*
 MeshViewer.
*/

enum MeshType
{
	STARS = 0,
	SUN1,
//    SUN2,
//    SUN3,
//    SUN4
};



class SolarViewer : public TrackballViewer
{
    
    

public:
   
	SolarViewer(const char* _title, int _width, int _height);
  
	void load_mesh(const std::string& filenameObj, MeshType type);
    
    int particlesNumber = 10;
    
    int cntPlanet;
	
	Mesh3D& getStars() {return m_Stars; }

protected:

	// overloaded GUI function
	virtual void init();
	virtual void keyboard(int key, int x, int y);
	virtual void special(int key, int x, int y);
public:
	virtual void idle();
protected:
    Mesh3D *createCube();
	
	virtual void draw_scene(DrawMode _draw_mode);
    void draw_cube(Mesh3D *mesh);

    void draw_object(Shader& sh, Mesh3D& mesh);
	void draw_object(Shader& sh, Mesh3D& mesh, bool showTexture);
    
    float RandomFloat(float a, float b);    
    Vector3 calculateForces(Mesh3D* p);
    Vector3 calculateVertexDistance(Vector3 a, Vector3 b);
    
public:
    void move(float timeElapsed);

protected:
	
	// mesh objects
	Mesh3D m_Stars;
    std::vector< Mesh3D* > m_meshes;
	
//	float m_PlanetsScale[8];
//	Vector3 m_PlanetsTranslate[8];
//	float m_PlanetsYear[8];
	
	// directional light
	Light3D m_light;
	float m_recSunlightInt;
	
	// mesh shader
	Shader m_meshShaderDiffuse;
	Shader m_meshShaderPhong;
	Shader m_meshShaderTexture;
    Shader m_cubeShader;
	
	bool m_showTextureStars;
	bool m_showTextureSun;
	bool m_showTextureEarth;
	bool m_showTextureMoon;
	bool m_geocentric;
	
	//scaling factors
	float m_moonScale;
	float m_earthScale;
	float m_sunScale;
	float m_sunTransX;
	float m_sunTransY;
	float m_sunTransZ;
	float m_starsScale;
	
	//translation factors
	float m_moonTrans;
	float m_earthTrans;
	
	//timer
	StopWatch watch;
	bool isWatchOn;
	
	float daysPerMiliSecond;
	float totalDaysElapsed;
	float currentTime;
    float totalTime;
    
   
	
//protected:
//    
//	// overloaded GUI function
//    virtual void init();
//    virtual void keyboard(int key, int x, int y);
//    virtual void special(int key, int x, int y);
//    
//    virtual void draw_scene(DrawMode _draw_mode);
    
    
	

	
	
};



//=============================================================================
#endif // MESH_VIEWER_HH defined
//=============================================================================

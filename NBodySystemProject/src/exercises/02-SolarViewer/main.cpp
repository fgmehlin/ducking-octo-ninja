//=============================================================================
//                                                                            
//   Exercise code for the lecture
//                                                                            
//=============================================================================

#include "SolarViewer.h"



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	SolarViewer window("Solar Viewer", 800, 800);
	
	if(argc > 1)
	{
		window.load_mesh(argv[1], STARS);
	}
	
	glutMainLoop();
}

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
	if(argc > 2)
	{
//        for(int i = 0; i < 20; i++){
            window.load_mesh(argv[2], SUN1);
        
//        }
//		window.load_mesh(argv[2], SUN2);
//		window.load_mesh(argv[2], SUN3);
//		window.load_mesh(argv[2], SUN4);
    }
    
		
	glutMainLoop();
}

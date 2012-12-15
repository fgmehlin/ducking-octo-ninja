varying vec4 color;
varying vec3 normal;



void main()
{
	// Exercise 3.3.2
    
    vec3 n = normalize(normal);
    vec3 l= normalize(vec3(0,0,1));
    float diffuse = dot(n,l);
    
    gl_FragColor = color*diffuse;
	
	// need this line so OpenGL doesn't optimize out the variables -- remove in your solution
    // {
	normal; color;
    // }
}
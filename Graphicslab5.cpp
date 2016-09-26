
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "maths_funcs.h"

// Assimp includes

#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.


/*----------------------------------------------------------------------------
                   MESH TO LOAD
  ----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
//#define MESH_NAME "monkeyhead.dae"
#define MESH_NAME1 "viper.obj"
#define MESH_NAME2 "asteroid 3DS.3ds"
#define MESH_NAME3 "monkeyhead.dae"
#define MESH_NAME4 "sun1.dae"
/*----------------------------------------------------------------------------
  ----------------------------------------------------------------------------*/

std::vector<float> g_vp, g_vn, g_vt;
int g_point_count = 0;


// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;
GLuint shaderProgramID;


unsigned int mesh_vao = 0;
int width = 800;
int height = 600;
int g_point_count1 = 0;
int g_point_count2 = 0;
int g_point_count3 = 0;
int g_point_count5 = 0;
int xyf =0;
double rotate_by_key=0;  //variable to track rotation position
double rotate_by_key1=0;  //variable to track rotation position
double scale_x=0.5;		//variable to track scaling position
double translate_y=0.5; //variable to track translation position
double translate_x=0.5;
double translate_z=0.5;
double lookupordowntheway = 0;







GLuint vao,vao1,vao2,vao3;

GLuint loc1, loc2, loc3,loc4,loc5,loc6,loc7,loc8,loc9,loc10,loc11,loc12;
GLfloat rotate_y = 0.0f;



#pragma region MESH LOADING
/*----------------------------------------------------------------------------
                   MESH LOADING FUNCTION
  ----------------------------------------------------------------------------*/

bool load_mesh (const char* file_name) {
  const aiScene* scene = aiImportFile (file_name, aiProcess_Triangulate); // TRIANGLES!
        fprintf (stderr, "ERROR: reading mesh %s\n", file_name);
  if (!scene) {
    fprintf (stderr, "ERROR: reading mesh %s\n", file_name);
    return false;
  }
  printf ("  %i animations\n", scene->mNumAnimations);
  printf ("  %i cameras\n", scene->mNumCameras);
  printf ("  %i lights\n", scene->mNumLights);
  printf ("  %i materials\n", scene->mNumMaterials);
  printf ("  %i meshes\n", scene->mNumMeshes);
  printf ("  %i textures\n", scene->mNumTextures);
  
  for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
    const aiMesh* mesh = scene->mMeshes[m_i];
    printf ("    %i vertices in mesh\n", mesh->mNumVertices);
    g_point_count = mesh->mNumVertices;
    for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
      if (mesh->HasPositions ()) {
        const aiVector3D* vp = &(mesh->mVertices[v_i]);
        //printf ("      vp %i (%f,%f,%f)\n", v_i, vp->x, vp->y, vp->z);
        g_vp.push_back (vp->x);
        g_vp.push_back (vp->y);
        g_vp.push_back (vp->z);
      }
      if (mesh->HasNormals ()) {
        const aiVector3D* vn = &(mesh->mNormals[v_i]);
        //printf ("      vn %i (%f,%f,%f)\n", v_i, vn->x, vn->y, vn->z);
        g_vn.push_back (vn->x);
        g_vn.push_back (vn->y);
        g_vn.push_back (vn->z);
      }
      if (mesh->HasTextureCoords (0)) {
        const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
        //printf ("      vt %i (%f,%f)\n", v_i, vt->x, vt->y);
        g_vt.push_back (vt->x);
        g_vt.push_back (vt->y);
      }
      if (mesh->HasTangentsAndBitangents ()) {
        // NB: could store/print tangents here
      }
    }
  }
  
  aiReleaseImport (scene);
  return true;
}

#pragma endregion MESH LOADING

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS

// Create a NULL-terminated string by reading the provided file
char* readShaderSource(const char* shaderFile) {   
    FILE* fp = fopen(shaderFile, "rb"); //!->Why does binary flag "RB" work and not "R"... wierd msvc thing?

    if ( fp == NULL ) { return NULL; }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);

    return buf;
}


void keyPress(int key,int x,int y)
{

    if(key==27)
            exit(0);
    if(key==GLUT_KEY_RIGHT)       //rotating to the right, turning right
        rotate_by_key+=1;
    if(key==GLUT_KEY_LEFT)        //rotating to the left, turning left
        rotate_by_key-=1;
	 if(key==GLUT_KEY_F1)       //rotating down, looking down
        lookupordowntheway+=1;
    if(key==GLUT_KEY_F2)        //rotating up, looking up
        lookupordowntheway-=1;
	if (key == GLUT_KEY_UP)       //strafe left
            translate_x += .5;
        if (key == GLUT_KEY_DOWN)  //strafe right
            translate_x -= .5;
		if (key == GLUT_KEY_PAGE_UP)   //moving down
            translate_y += .5;
        if (key == GLUT_KEY_PAGE_DOWN)  //moving up
            translate_y -= .5;
		if (key == GLUT_KEY_END)       //walk forwards
            translate_z += .5;
		if (key == GLUT_KEY_HOME)       //walk backwards
            translate_z -= .5;
		//if (key == GLUT_KEY_PAGE_DOWN)
		//{rotate_by_key += 5; translate_y -= .05;}
    glutPostRedisplay();
   
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }
	const char* pShaderSource = readShaderSource( pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
    glCompileShader(ShaderObj);
    GLint success;
	// check for shader related errors using glGetShaderiv
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }
	// Attach the compiled shader object to the program object
    glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
    shaderProgramID = glCreateProgram();
    if (shaderProgramID == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

	// Create two shader objects, one for the vertex, and one for the fragment shader
    AddShader(shaderProgramID, "../Shaders/simpleVertexShader1.txt", GL_VERTEX_SHADER);
  AddShader(shaderProgramID, "../Shaders/simpleFragmentShader3.txt", GL_FRAGMENT_SHADER);
	

	//AddShader(shaderProgramID, "../Shaders/vertex1.txt", GL_VERTEX_SHADER);
  // AddShader(shaderProgramID, "../Shaders/fragment1.txt", GL_FRAGMENT_SHADER);
	

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };
	// After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
    glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS



void generateObjectBufferMesh() {
/*----------------------------------------------------------------------------
                   LOAD MESH HERE AND COPY INTO BUFFERS
  ----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.
	
	load_mesh (MESH_NAME1);
	g_point_count1 = g_point_count;
	cout<< g_point_count1;
	unsigned int vp_vbo = 0;
	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glGenBuffers (1, &vp_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glBufferData (GL_ARRAY_BUFFER, g_point_count1 * 3 * sizeof (float), &g_vp[0], GL_STATIC_DRAW);
	unsigned int vn_vbo = 0;
	glGenBuffers (1, &vn_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	glBufferData (GL_ARRAY_BUFFER, g_point_count1 * 3 * sizeof (float), &g_vn[0], GL_STATIC_DRAW);

//	This is for texture coordinates which you don't currently need, so I have commented it out
	unsigned int vt_vbo = 0;
	glGenBuffers (1, &vt_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glBufferData (GL_ARRAY_BUFFER, g_point_count * 2 * sizeof (float), &g_vt[0], GL_STATIC_DRAW);
	
	vao = 0;
	glBindVertexArray (vao);

	glEnableVertexAttribArray (loc1);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer (loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (loc2);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer (loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//This is for texture coordinates which you don't currently need, so I have commented it out
	glEnableVertexAttribArray (loc3);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);





	load_mesh (MESH_NAME2);
	//g_point_count2 = g_point_count;
	
	
	xyf = g_point_count+g_point_count1;
	g_point_count2=xyf;
	cout<<g_point_count2;
	cout<<xyf;
	cout<<g_point_count;
	unsigned int vp_vbo1 = 0;
	loc4 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc5 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc6 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glGenBuffers (1, &vp_vbo1);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo1);
	glBufferData (GL_ARRAY_BUFFER, g_point_count2 * 3 * sizeof (float), &g_vp[0], GL_STATIC_DRAW);
	unsigned int vn_vbo1 = 0;
	glGenBuffers (1, &vn_vbo1);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo1);
	glBufferData (GL_ARRAY_BUFFER, g_point_count2 * 3 * sizeof (float), &g_vn[0], GL_STATIC_DRAW);

//	This is for texture coordinates which you don't currently need, so I have commented it out
	unsigned int vt_vbo1 = 0;
	glGenBuffers (1, &vt_vbo1);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo1);
	glBufferData (GL_ARRAY_BUFFER, g_point_count * 2 * sizeof (float), &g_vt[0], GL_STATIC_DRAW);
	
	 vao1 = 0;
	glBindVertexArray (vao1);

	glEnableVertexAttribArray (loc4);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo1);
	glVertexAttribPointer (loc4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (loc5);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo1);
	glVertexAttribPointer (loc5, 3, GL_FLOAT, GL_FALSE, 0, NULL);

//	This is for texture coordinates which you don't currently need, so I have commented it out
	glEnableVertexAttribArray (loc6);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo1);
	glVertexAttribPointer (loc6, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	

	




	
	int xyf2;
	load_mesh (MESH_NAME4);
	//g_point_count3 = g_point_count;
	//cout<<g_point_count3;
	//xyf2 = g_point_count3-g_point_count2;

	//g_point_count2 = g_point_count;
	
	
	xyf2 = g_point_count+g_point_count2;
	g_point_count3=xyf2;
	cout<<g_point_count3;
	cout<<xyf2;
	cout<<g_point_count;
	unsigned int vp_vbo2 = 0;
	loc7 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc9 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glGenBuffers (1, &vp_vbo2);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo2);
	glBufferData (GL_ARRAY_BUFFER, g_point_count3 * 3 * sizeof (float), &g_vp[0], GL_STATIC_DRAW);
	unsigned int vn_vbo2 = 0;
	glGenBuffers (1, &vn_vbo2);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo2);
	glBufferData (GL_ARRAY_BUFFER, g_point_count3 * 3 * sizeof (float), &g_vn[0], GL_STATIC_DRAW);

//	This is for texture coordinates which you don't currently need, so I have commented it out
//	unsigned int vt_vbo = 0;
//	glGenBuffers (1, &vt_vbo);
//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
//	glBufferData (GL_ARRAY_BUFFER, g_point_count * 2 * sizeof (float), &g_vt[0], GL_STATIC_DRAW);
	
	 vao2 = 0;
	glBindVertexArray (vao2);

	glEnableVertexAttribArray (loc7);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo2);
	glVertexAttribPointer (loc7, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (loc8);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo2);
	glVertexAttribPointer (loc8, 3, GL_FLOAT, GL_FALSE, 0, NULL);

//	This is for texture coordinates which you don't currently need, so I have commented it out
//	glEnableVertexAttribArray (loc3);
//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
}


#pragma endregion VBO_FUNCTIONS


void display(){

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glShadeModel(GL_SMOOTH);
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (0.1f, 0.1f, 0.1f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram (shaderProgramID);


	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation (shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation (shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation (shaderProgramID, "proj");

	//spaceship
	mat4 view = identity_mat4 ();
	mat4 persp_proj = perspective(90.0, (float)width/(float)height, 0.1, 100.0);
	mat4 model = identity_mat4 ();
	model = rotate_y_deg (model, rotate_y); 
	model = translate(model,vec3(-15.0,0.0,20.0));
	view = translate (view, vec3 (translate_x, translate_y, translate_z));
	view = rotate_y_deg(view,rotate_by_key);
	view = rotate_x_deg(view,lookupordowntheway);
	view = translate(view, vec3(10.0,0.0,0.0));
	
	//view= scale(view,vec3(1.0,1.0,1.0));

	//glBindVertexArray(vao1);
	// update uniforms & draw
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model.m);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
  


	//start of asteroids, asteroid 1
	//glBindVertexArray(vao);
  //root of the hierarchy, not currently used
	//mat4 view1 = identity_mat4 ();
	mat4 persp_proj1 = perspective(120.0, (float)width/(float)height, 0.1, 1000.0);
	mat4 model1 = identity_mat4 ();
	model1 = rotate_y_deg (model1, rotate_y); 
	model1 = translate (model1, vec3 (-20.0, -20.0, -50.0f));
	model1= scale(model1,vec3(1.0,1.0,1.0));  // glScalef(0.5f, 0.5f, 0.5f);
	mat4 global1 = model1;
	// update uniforms & draw
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj1.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model1.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);




	
	//glBindVertexArray(vao);
	mat4 model2 = identity_mat4 ();
	model2 = rotate_z_deg (model2, rotate_y); 
	model2 = translate (model2, vec3 (50.0, -10.0, 25.0f));
	//model2 = rotate_y_deg (model2, rotate_y); 
	model2= scale(model2,vec3(.5,.5,.5));
	mat4 global2 = global1*model2;


	// update uniforms & draw
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model2.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);



	mat4 model3 = identity_mat4 ();
	model3 = rotate_y_deg (model3, rotate_y);
	model3 = translate (model3, vec3 (-60.0, -40.0, -15.0f));
	 
	mat4 global3 = global1*model3;


	// update uniforms & draw
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model3.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);


	//glBindVertexArray(vao);
	mat4 model5 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	model5 = rotate_x_deg (model5, rotate_y); 
	model5 = translate (model5, vec3 (-10.0, 60.0, -20.0f));
	
	mat4 global5 = global1*model5;

	// update uniforms & draw
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model5.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);




	//glBindVertexArray(vao);
	mat4 model6 = identity_mat4 ();
	model6 = translate (model6, vec3 (-50.0, 90.0, 10.0f));
	model6= scale(model6,vec3(.5,1.5,1.0));
	model6 = rotate_y_deg (model6, rotate_y); 
	mat4 global6 = global1*model6;

	// update uniforms & draw
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model6.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);


	mat4 model4 = identity_mat4 ();
	model4 = rotate_z_deg (model4, rotate_y); 
	model4 = translate (model4, vec3 (-60.0, 10.0, 10.0f));
	//model4= scale(model2,vec3(1.2,1.2,1.2));
	
	mat4 global4 = global1*model4;

	// update uniforms & draw
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model4.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);





	
		//the base
	mat4 persp_projsun = perspective(90.0, (float)width/(float)height, 0.1, 100.0);
	mat4 modelsun = identity_mat4 ();
	//modelsun = rotate_x_deg (modelsun, 0.0); 
	modelsun = translate (modelsun, vec3 (1.5, 1.5, 0.0f));
	modelsun= scale(modelsun,vec3(5.0,5.0,5.0));  // glScalef(0.5f, 0.5f, 0.5f);
	modelsun= scale(modelsun,vec3(2.0,2.0,2.0));
  modelsun= scale(modelsun,vec3(2.0,2.0,2.0));
	mat4 globalsun = modelsun;
	// update uniforms & draw
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projsun.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, modelsun.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	
	
	


    glutSwapBuffers();
}


void updateScene() {	

	// Placeholder code, if you want to work with framerate
	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;

	// rotate the model slowly around the y axis
	rotate_y+=0.2f;
	// Draw the next frame
	glutPostRedisplay();
}


void init()
{

	
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();
	// load mesh into a vertex buffer array
	generateObjectBufferMesh();
	
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	if(key=='x'){
		//Translate the base, etc.
	}

}

int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Hello Triangle");
	
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutSpecialFunc(keyPress);

	 // A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	getchar();
    return 0;
}












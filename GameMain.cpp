
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
#include "maths_funcs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <assert.h>

// Assimp includes

#define STBI_ASSERT(x)
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include "text.h"

/*----------------------------------------------------------------------------
                   MESH TO LOAD
  ----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
//#define MESH_NAME "monkeyhead.dae"
#define MESH_NAME6 "viper.obj"
#define MESH_NAME1 "turretbarrel.3ds"
#define MESH_NAME2 "asteroid 3DS.3ds"
#define MESH_NAME3 "monkeyhead.dae"
#define MESH_NAME4 "sun1.dae"
#define MESH_NAME7 "turret1.3ds"
/*----------------------------------------------------------------------------
  ----------------------------------------------------------------------------*/

std::vector<float> g_vp, g_vn, g_vt;
int g_point_count = 0;

// files to use for font. change path here
const char* atlas_image = "freemono.png";
const char* atlas_meta = "freemono.meta";

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define RED 1
#define GREEN 2
#define BLUE 3
#define ORANGE 4
#define SKIN1 5
#define SKIN2 6
#define SKIN3 7
#define MAP1 8
#define MAP2 9
#define MAP3 10
#define ShootingMode 1
#define RacingMode 2
#define RoamingCamera 1
#define SideView 2
#define FirstPersonView 3
#define ThirdPerson 4
// Pop up menu identifiers
int fillMenu, viewMenu, mainMenu, colorMenu,skinMenu, mapMenu;

using namespace std;
GLuint shaderProgramID;
GLuint stuff;
bool   gp;                      // G Pressed? ( New )
GLuint filter;                      // Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
GLuint fogfilter= 0;                    // Which Fog To Use
GLfloat fogColor[4]= {0.0f, 1.0f, 0.5f, 1.0f};      // Fog Color
bool showbullet = false;
bool showbullet2= false;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;
int g_point_count1 = 0;
int g_point_count2 = 0;
int g_point_count3 = 0;
int g_point_count5 = 0;
int g_point_count4 = 0;
int xyf =0;
int xyf3,xyf4,xyf5;
double rotate_by_key=0;  //variable to track rotation position
double rotate_by_key1=0;  //variable to track rotation position
double rotate_by_keymodel=0;
double scale_x=0.5;		//variable to track scaling position
double translate_y=0.5; //variable to track translation position
double translate_x=0.5;
double translate_z=0.5;
double lookupordowntheway = 0;
double lookupordownthewaymodel = 0;
float shoot=0;
float shoot2=0;
double camAngle=0;
double lx, lz;
int stopdraw,stopdraw1,stopdraw2,stopdraw3,stopdraw4,stopdraw5;
int stopdraw6=0;
int viewselect=4;
unsigned char * ship;
unsigned char * data;
int widthtex = 256;
int heighttex = 256;
int bitdepth = 4;
float red,green,blue,orange;
GLuint texture3,texture2,texture4,texture5,texture6,texture7,texture8,texture9;
float scale1;
int score=8;
int showscore;
float spin,spinreverse=0;
GLuint vao1,vao,vao2,vao5,vao4;
int reset = 0;
int mapsetting=0;
int gamemode=1;
GLuint loc1, loc2, loc3,loc4,loc5,loc6,loc7,loc8,loc9,loc10,loc11,loc12;
GLfloat rotate_y = 0.0f;
float movingforward=0;
bool forwardmoving=false;
int startover1,startover2,startover3,startover4,startover5,startover6,startover7;
int shipskin =1;


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
       rotate_by_key+=.5;
	rotate_by_key1+=.5;
    if(key==GLUT_KEY_LEFT)        //rotating to the left, turning left
        rotate_by_key-=.5;
	rotate_by_key1-=.5;
	if(key==GLUT_KEY_F7)       //rotating model to the right, turning right
       rotate_by_keymodel+=.5;
    if(key==GLUT_KEY_F8)        //rotating to the left, turning left
        rotate_by_keymodel-=.5;
	 if(key==GLUT_KEY_F1)       //rotating down, looking down
        lookupordowntheway+=.5;
    if(key==GLUT_KEY_F2)        //rotating up, looking up
        lookupordowntheway-=.5;
	if(key==GLUT_KEY_F12)       //rotating up, looking up
		forwardmoving=!forwardmoving;
	if(key==GLUT_KEY_F3)        //rotating model up, looking up
       // lookupordownthewaymodel-=.5;
		   forwardmoving=!forwardmoving;
	if(key==GLUT_KEY_F4)        //rotating model up, looking up
		//showscore=showscore+1;
        //lookupordownthewaymodel+=.5;
	if(key==GLUT_KEY_F5)        //rotating model up, looking up
      // showscore=showscore-1;
	showbullet = !showbullet;
	if(key==GLUT_KEY_F6)       //rotating model up, looking up
       
	showbullet2=!showbullet2;
	// shoot+=0.1;
	if(key==GLUT_KEY_F9)       //rotating model up, looking up
       reset=1;
	if(key==GLUT_KEY_F11){//rotating model up, looking up
		if(gamemode<1){
			gamemode++;
		}
		else{gamemode--;
		}
	}

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
    AddShader(shaderProgramID, "../Shaders/simpVertShader.txt", GL_VERTEX_SHADER);
  AddShader(shaderProgramID, "../Shaders/fragshade20.txt", GL_FRAGMENT_SHADER);
	

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













GLuint loadBMP_custom(const char * imagepath){
// Data read from the header of the BMP file
 unsigned char header[54]; // Each BMP file begins by a 54-bytes header
 unsigned int dataPos;     // Position in the file where the actual data begins
 unsigned int width, height;
 unsigned int imageSize;   // = width*height*3
 // Actual RGB data
 unsigned char * data;
// Open the file
 FILE * file = fopen(imagepath,"rb");
 if (!file){printf("Image could not be opened\n"); return 0;}
 if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
     return false;
 }
 if ( header[0]!='B' || header[1]!='M' ){
    printf("Not a correct BMP file\n");
     return 0;
 }
  // Read ints from the byte array
 dataPos    = *(int*)&(header[0x0A]);
 imageSize  = *(int*)&(header[0x22]);
 width      = *(int*)&(header[0x12]);
 height     = *(int*)&(header[0x16]);
// Some BMP files are misformatted, guess missing information
 if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
 if (dataPos==0)      dataPos=54; // The BMP header is done that way
// Create a buffer
 data = new unsigned char [imageSize];

 // Read the actual data from the file into the buffer
 fread(data,1,imageSize,file);
 
 //Everything is in memory now, the file can be closed
fclose(file);
}








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
	glBufferData (GL_ARRAY_BUFFER, g_point_count1 * 2 * sizeof (float), &g_vt[0], GL_STATIC_DRAW);
	
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
	glBufferData (GL_ARRAY_BUFFER, g_point_count2 * 2 * sizeof (float), &g_vt[0], GL_STATIC_DRAW);
	
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
	
	

	

	int xyf3;
	load_mesh (MESH_NAME7);

	xyf3 = g_point_count+g_point_count1;
	g_point_count3=g_point_count2+g_point_count;
	cout<<g_point_count2;
	cout<<xyf;
	cout<<g_point_count;
	unsigned int vp_vbo3 = 0;
	loc7 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc8 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc9 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glGenBuffers (1, &vp_vbo3);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo3);
	glBufferData (GL_ARRAY_BUFFER, g_point_count3 * 3 * sizeof (float), &g_vp[0], GL_STATIC_DRAW);
	unsigned int vn_vbo3 = 0;
	glGenBuffers (1, &vn_vbo3);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo3);
	glBufferData (GL_ARRAY_BUFFER, g_point_count3 * 3 * sizeof (float), &g_vn[0], GL_STATIC_DRAW);

//	This is for texture coordinates which you don't currently need, so I have commented it out

	unsigned int vt_vbo3 = 0;
	//GLuint textid= loadBMP_custom("texture12.bmp");
	glGenBuffers (1, &vt_vbo3);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo3);
	glBufferData (GL_ARRAY_BUFFER, g_point_count2 * 2 * sizeof (float), &g_vt[0], GL_STATIC_DRAW);
	
	 vao4 = 0;
	glBindVertexArray (vao4);

	glEnableVertexAttribArray (loc7);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo3);
	glVertexAttribPointer (loc7, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (loc8);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo3);
	glVertexAttribPointer (loc8, 3, GL_FLOAT, GL_FALSE, 0, NULL);

//	This is for texture coordinates which you don't currently need, so I have commented it out
	glEnableVertexAttribArray (loc9);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo3);
	glVertexAttribPointer (loc9, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	
}

void initialise_texture(GLuint* id, GLuint a, int x, int y, unsigned char* data) {
	glGenTextures(1, id);
	glActiveTexture(a);
	glBindTexture(GL_TEXTURE_2D,*id);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}







#pragma endregion VBO_FUNCTIONS




	
							



void display(){

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor (red, green, blue, 1.0f);
	glViewport (0, 0, 800, 600);
	glDepthRange(0.1, 1.0);
	//glViewport(0.0,0.0,400,300);
	//glDepthRange(0.0, 0.1);
    //draw_texts ();
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glUseProgram (shaderProgramID);
	//DemoLight();
	
	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation (shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation (shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation (shaderProgramID, "proj");
	

	if(reset==1){score=0;translate_x=0;translate_y=0;translate_z=0;rotate_by_key=0;lookupordowntheway=0;rotate_by_keymodel=0;
	stopdraw=0;stopdraw1=0;stopdraw2=0;stopdraw3=0;stopdraw4=0;stopdraw5=0;stopdraw6=0;shoot=0;shoot2=0;showbullet=false;showbullet2=false;movingforward=0.0;}
	
	if(gamemode==0){viewselect=4;}
	reset=0;
	
	if(gamemode==0){
		viewselect=4;
		mat4 view = identity_mat4 ();
	mat4 persp_proj = perspective(90.0, (float)width/(float)height, 0.1, 500.0);
	mat4 model = identity_mat4 ();

	if(forwardmoving){movingforward=movingforward+0.25;}
	//third person view
	if (viewselect==1){model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-movingforward));
	//model = rotate_x_deg(model,-lookupordowntheway);
	
	//model = rotate_y_deg(model,-rotate_by_key);
	
	view = look_at (vec3(rotate_by_key,lookupordowntheway,rotate_by_keymodel), vec3(-translate_x,-translate_y,-movingforward), vec3(0,1,0));}

	//side game view
	if (viewselect==2){model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-movingforward));
	//model = rotate_x_deg(model,-lookupordowntheway);
	
	//model = rotate_y_deg(model,-rotate_by_key);
	
	view = look_at (vec3(-20.0,20.0,40.0), vec3(-translate_x,-translate_y,-movingforward), vec3(0,1,0));}


		if (viewselect==4){view = rotate_x_deg(view,lookupordowntheway);
	view = rotate_y_deg(view,rotate_by_key);
	view = translate (view, vec3 (translate_x, translate_y, movingforward));
	view = translate (view, vec3 (-10.0, -10.0, -20.0));
	model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-movingforward));
	model = rotate_x_deg(model,-lookupordowntheway);
	model = rotate_y_deg(model,-rotate_by_key);}
		
		if (viewselect==3){view = rotate_x_deg(view,lookupordowntheway);
	view = rotate_y_deg(view,rotate_by_key);
	view = translate (view, vec3 (translate_x, translate_y, movingforward));
	view = translate (view, vec3 (0.0, 0.0, 2.0f));
	model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-movingforward));
	model = rotate_x_deg(model,-lookupordowntheway);
	model = rotate_y_deg(model,-rotate_by_key);}
	
		 //glBindTexture(GL_TEXTURE_2D, texture3);
		if(shipskin==3){glBindTexture(GL_TEXTURE_2D, texture9);}
		if(shipskin==2){glBindTexture(GL_TEXTURE_2D, texture7);}
		if(shipskin==1){glBindTexture(GL_TEXTURE_2D, texture2);}
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model.m);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);


	mat4 bullet1 = identity_mat4 ();
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	bullet1 = translate(bullet1,vec3(-2.5, 0.0, 0.0));
	//bullet1 = translate(bullet1,vec3(-translate_x,-translate_y,-translate_z));
	//modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	//modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	
	mat4 globalgun = model*bullet1;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	if(shipskin==3){glBindTexture(GL_TEXTURE_2D, texture9);}
		if(shipskin==2){glBindTexture(GL_TEXTURE_2D, texture7);}
		if(shipskin==1){glBindTexture(GL_TEXTURE_2D, texture2);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalgun.m);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);


	mat4 bulletx = identity_mat4 ();
	int shootreset;
	if(showbullet){

	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	//bulletx = scale(bulletx, vec3(scale1,scale1,scale1));
	bulletx = translate(bulletx,vec3(-5.0, 0.0, -shoot));
	bulletx = scale(bulletx,vec3(0.5,0.5,0.5));
	//bullet1 = translate(bullet1,vec3(-translate_x,-translate_y,-translate_z));
	//modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	//modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	
	shoot=shoot+0.5;
	
	mat4 globalbullet = model*bulletx;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalbullet.m);
	
	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	}
	if(shoot>80.0^!showbullet){
		shoot=0.0;}
//f(!showbullet){shoot=0.0;}100.0
	int saveddist2;
	
		if(shoot>40.0&&shoot<50.0){
		saveddist2=shoot;
		}
		if(shoot>80.0^!showbullet){
		shoot=0.0;}








	float bulletx2x = translate_x;
	float bulletx2y = translate_y;
	mat4 bulletx2= identity_mat4 ();
	
	bulletx2 = scale(bulletx2,vec3(0.5,0.5,0.5));
	//bulletx2 = translate(bulletx2,vec3(1.25,0.0,-shoot2));
	if(viewselect==3^viewselect==4){
	bulletx2 = translate(bulletx2,vec3(0.0,0.0,-shoot2/2));
	}
	if(viewselect==1^viewselect==2){
	bulletx2= translate (bulletx2, vec3 (1.25, -1.0, -2.0));
	bulletx2 = translate(bulletx2,vec3(0.0,0.0,-translate_z));
	bulletx2 = translate(bulletx2,vec3(-translate_x,-translate_y,-shoot2/2));
	}
	//bulletx2 = rotate_x_deg(bulletx2,-lookupordowntheway);
	//bulletx2 = rotate_y_deg(bulletx2,-rotate_by_key);
	if(showbullet2){

	
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	//bulletx2 = scale(bulletx2, vec3(0.5,0.5,0.5));
	
	
	//bullet1 = translate(bullet1,vec3(-translate_x,-translate_y,-translate_z));
	//modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	//modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	
	shoot2=shoot2+0.5;
	mat4 globalbullet2 = model*bulletx2;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	glBindTexture(GL_TEXTURE_2D, texture2);
	if(viewselect==3^viewselect==4){
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalbullet2.m);
	}
	if(viewselect==1^viewselect==2){
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, bulletx2.m);
	}
	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	}
	int saveddist;
	
		if(shoot2>40.0&&shoot2<50.0){
		saveddist=shoot2;
		}
		if(shoot2>80.0){
		shoot2=0.0;}


	
	mat4 shipx = identity_mat4 ();
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	shipx = scale(shipx, vec3(2.0,2.0,2.0));
	shipx = translate(shipx,vec3(-7.0,-1.25, -2.5));
	//shipx= translate (shipx, vec3 (1.25, -1.0, -3.5f));
	//shipx = translate(shipx,vec3(0.0,0.0,-translate_z));

	//shipx = translate(shipx,vec3(-bulletx2x,-bulletx2y,-shoot2));
	shipx = rotate_x_deg(shipx,180);
	shipx = rotate_y_deg(shipx,30.0);
	shipx = rotate_z_deg(shipx,90.0);
	mat4 globalship= model*shipx;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	if(shipskin==3){glBindTexture(GL_TEXTURE_2D, texture9);}
		if(shipskin==2){glBindTexture(GL_TEXTURE_2D, texture7);}
		if(shipskin==1){glBindTexture(GL_TEXTURE_2D, texture2);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalship.m);
	
	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);	

	mat4 model1 = identity_mat4 ();
	model1 = rotate_y_deg (model1, rotate_y); 
	model1 = translate (model1, vec3 (0.0, 0.0, -40.0f));
	//model1= scale(model1,vec3(1.0,1.0,1.0));  // glScalef(0.5f, 0.5f, 0.5f);
	mat4 global1 = model1;
	// update uniforms & draw
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj1.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	
	float zboundaryupper=40;
	float zboundarylower=50;
	float xboundaryupper=10;
	float xboundarylower=10;
	float yboundaryupper=10;
	float yboundarylower=10;
	if(movingforward>30&&movingforward<50&&translate_x<10&&translate_x>-10&&translate_y<10&&translate_y>-10){
	//&&(-translate_x>=-7.5&&-translate_x<=7.5)&&(-translate_y>=-7.5&&-translate_y<=7.5)){
		
		stopdraw=1;
		startover1=startover1+1;
		//score=score+1;
		if(startover1<=1){movingforward=0.0;}
		
	}
	if(stopdraw==0){
		glBindTexture(GL_TEXTURE_2D, texture4);
		glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model1.m);
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	
	}

	
	
	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model2 = identity_mat4 ();
	//model2 = rotate_z_deg (model2, rotate_y); 
	
	model2 = rotate_y_deg (model2, rotate_y); 
	model2 = translate (model2, vec3 (-20.0, -20.0, -100.0f));
	//model2= scale(model2,vec3(.5,.5,.5));
	mat4 global2 = global1*model2;

	//glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D,texture4);
	// update uniforms & draw
	
	//glBindTexture(GL_TEXTURE_2D,texturez1);
	float zboundaryupper1=40;
	float zboundarylower1=50;
	float xboundaryupper1=10;
	float xboundarylower1=30;
	//if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper1&&saveddist<zboundarylower1)&&(-bulletx2x>=-xboundarylower1&&-bulletx2x<=-xboundaryupper1)&&(-bulletx2y<=-10&&-bulletx2y>=-25)){
		
	//	if(showbullet2^showbullet){
	//	stopdraw1=1;
	//	score=score+1;
	//	}
	if(movingforward>90&&movingforward<110&&-translate_x<-10&&-translate_x>-30&&-translate_y<-10&&-translate_y>-30){
	//&&(-translate_x>=-7.5&&-translate_x<=7.5)&&(-translate_y>=-7.5&&-translate_y<=7.5)){
		
		stopdraw1=1;
		startover2=startover2+1;
		//score=score+1;
		if(startover2<=1){movingforward=0.0;}
		
	}
	if(stopdraw1==0){
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model2.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);

	}

	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model3 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0);
	model3 = rotate_y_deg (model3, rotate_y);
	model3 = translate (model3, vec3 (20.0, -20.0, -160.0f));
	//model2= scale(model2,vec3(.5,.5,.5));
	 
	mat4 global3 = global2*model3;
	float zboundaryupper2=40;
	float zboundarylower2=50;
	float xboundaryupper2=10;
	float xboundarylower2=30;
	float yboundaryupper2=10;
	float yboundarylower2=30;

		if(movingforward>150&&movingforward<170&&translate_x<-10&&translate_x>-30&&-translate_y<-10&&-translate_y>-30){
	//&&(-translate_x>=-7.5&&-translate_x<=7.5)&&(-translate_y>=-7.5&&-translate_y<=7.5)){
		
		stopdraw2=1;
		startover3=startover3+1;
		//score=score+1;
		if(startover3<=1){movingforward=0.0;}
		
	}
	if(stopdraw2==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model3.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}

	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model5 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	model5 = rotate_y_deg (model5, rotate_y); 
	model5 = translate (model5, vec3 (-20.0, 20.0, -220.0f));
	//model2= scale(model2,vec3(.5,.5,.5));
	
	mat4 global4 = global3*model5;

	float zboundaryupper4=40;
	float zboundarylower4=50;
	float xboundaryupper4=10;
	float xboundarylower4=30;
	float yboundaryupper4=10;
	float yboundarylower4=30;

		if(movingforward>210&&movingforward<230&&translate_y<-10&&translate_y>-30&&-translate_x<-10&&-translate_x>-30){
	//&&(-translate_x>=-7.5&&-translate_x<=7.5)&&(-translate_y>=-7.5&&-translate_y<=7.5)){
		
		stopdraw3=1;
		startover4=startover4+1;
		//score=score+1;
		if(startover4<=1){movingforward=0.0;}
		
	}
	if(stopdraw3==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model5.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}



	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model6 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	
	//model6= scale(model6,vec3(scale1,scale1,scale1));
	model6 = rotate_y_deg (model6, rotate_y); 
	model6 = translate (model6, vec3 (20.0, 20.0, -280.0f));
	mat4 global5 = global4*model5;

	if(movingforward>270&&movingforward<290&&-translate_x>=10&&-translate_x<=30&&-translate_y<=30&&-translate_y>=10){
		
		stopdraw4=1;
		startover5=startover5+1;
		//score=score+1;
		if(startover5<=1){movingforward=0.0;}
	}
	if(stopdraw4==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model6.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}


	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model7 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	model7 = rotate_y_deg (model7, rotate_y);
	model7 = translate (model7, vec3 (-20.0, 0.0, -340.0f));
	 
	//model4= scale(model2,vec3(1.2,1.2,1.2));
	
	mat4 global6 = global5*model7;
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,texture2);
	if(movingforward>330&&movingforward<350&&-translate_x<-10&&-translate_x>-30&&translate_y>-10&&translate_y<10){
		
		stopdraw5=1;
		startover6=startover6+1;
		//score=score+1;
		if(startover6<=1){movingforward=0.0;}
	}
	if(stopdraw5==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model7.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	


	mat4 model8 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	model8 = rotate_y_deg (model8, rotate_y);
	model8 = translate (model8, vec3 (20.0, 0.0, -400.0f));
	 
	//model4= scale(model2,vec3(1.2,1.2,1.2));
	
	mat4 global7 = global6*model8;
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,texture2);
	if(movingforward>390&&movingforward<410&&-translate_x>=10&&-translate_x<=30&&translate_y>-10&&translate_y<10){
		
		stopdraw6=1;
		startover7=startover7+1;
		//score=score+1;
		if(startover7<=1){movingforward=0.0;}
	}
	if(stopdraw6==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model8.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}

	if(movingforward>=425){reset=1;
	score=score+50;}
	}




















	if(gamemode==1){
	//monkey head
	// Root of the Hierarchy
	//mat4 spaceship = identity_mat4();

	mat4 view = identity_mat4 ();
	mat4 persp_proj = perspective(90.0, (float)width/(float)height, 0.1, 100.0);
	mat4 model = identity_mat4 ();
	//model = rotate_y_deg (model, rotate_y); 
	//model = translate(model,vec3(-15.0,lookupordowntheway,20.0));
	
	float test1,test2,test3,test4;
	//view = translate (view, vec3 (-translate_x, -translate_y, -translate_z));
	//view = rotate_x_deg(view,lookupordowntheway);
	//view = rotate_y_deg(view,rotate_by_key1);
	//view = translate (view, vec3 (translate_x, translate_y, translate_z));
	//view = rotate_x_deg(view,lookupordowntheway);
	//view = rotate_y_deg(view,rotate_by_key);
	//view = translate(view, vec3(10.0,0.0,0.0));
	//spaceship =view;
	//view= scale(view,vec3(1.0,1.0,1.0));
	//view = look_at (vec3(translate_x,translate_y,translate_z), vec3(test1,test2,20.0), vec3(0,1,0));
	/*
	model = translate(model,vec3(rotate_by_key,lookupordowntheway,0.0));
	test1 = rotate_by_key;
	test2 = lookupordowntheway;*/
	
	//test3 = test1-19.5;
	//test3 = test2-19.5;
	//view = look_at (vec3(translate_x+test1,translate_y+test2,translate_z), vec3(test1,test2,0.0), vec3(0,1,0));
	/*
	model = rotate_x_deg(model,lookupordownthewaymodel);
	model = rotate_y_deg(model,rotate_by_keymodel);
	model = translate(model,vec3(translate_x,translate_y,translate_z));
	*/
	//test1 = translate_x;
	//test2 = translate_y;
	//test3 = translate_z;
	//model = translate(model, vec3(10.0,0.0,0.0));
	//view = look_at (vec3(translate_x+5.0,translate_y+5.0,translate_z+5.0), vec3(translate_x,translate_y,translate_z), vec3(0,1,0));
	
	//view = translate(view, vec3(10.0,0.0,0.0));
	//view = look_at (vec3(translate_x+lookupordowntheway,translate_y+rotate_by_key,translate_z+5.0), vec3(test1,test2,test3), vec3(0,1,0));
	

	//model = rotate_x_deg(model,lookupordowntheway);
	//model = rotate_y_deg(model,rotate_by_key);
	/*model = translate(model,vec3(translate_x,translate_y,translate_z));
	test1 =translate_x;
	test2 =translate_y;
	test3 =translate_z;*/
	
	//view = look_at (vec3(translate_x+0.0,translate_y+0.0,translate_z+5.0), vec3(test1,test2,test3), vec3(0,1,0));
	//view = look_at (vec3(translate_x,translate_y,translate_z), vec3(translate_x+5.0,translate_y+5.0,translate_z+5.0), vec3(0,1,0));
	//view = translate (view, vec3 (translate_x, translate_y, translate_z));
	//model = rotate_y_deg(model,10);
	//model = rotate_x_deg(model,30);
	//view = rotate_x_deg(view,lookupordowntheway);
	//view = rotate_y_deg(view,rotate_by_key);
	//view = translate (view, vec3 (translate_x, translate_y, translate_z));
	//view = translate (view, vec3 (-20.0, -20.0, -40.0));
	/*model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-translate_z));
	//model = rotate_x_deg(model,-lookupordowntheway);
	
	//model = rotate_y_deg(model,-rotate_by_key);
	
	view = look_at (vec3(rotate_by_key,lookupordowntheway,rotate_by_keymodel), vec3(-translate_x,-translate_y,-translate_z), vec3(0,1,0));*/


	if (viewselect==4){view = rotate_x_deg(view,lookupordowntheway);
	view = rotate_y_deg(view,rotate_by_key);
	view = translate (view, vec3 (translate_x, translate_y, translate_z));
	view = translate (view, vec3 (-10.0, -10.0, -20.0));
	model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-translate_z));
	model = rotate_x_deg(model,-lookupordowntheway);
	model = rotate_y_deg(model,-rotate_by_key);}

	//third person view
	if (viewselect==1){model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-translate_z));
	//model = rotate_x_deg(model,-lookupordowntheway);
	
	//model = rotate_y_deg(model,-rotate_by_key);
	
	view = look_at (vec3(rotate_by_key,lookupordowntheway,rotate_by_keymodel), vec3(-translate_x,-translate_y,-translate_z), vec3(0,1,0));}

	//side game view
	if (viewselect==2){model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-translate_z));
	//model = rotate_x_deg(model,-lookupordowntheway);
	
	//model = rotate_y_deg(model,-rotate_by_key);
	
	view = look_at (vec3(-20.0,20.0,40.0), vec3(-translate_x,-translate_y,-translate_z), vec3(0,1,0));}

	//first-person view
	if (viewselect==3){view = rotate_x_deg(view,lookupordowntheway);
	/* glEnable(GL_LIGHT0); 
	GLfloat light_position[] = {-translate_x,-translate_y,-translate_z, 1.0 };
        gluLookAt (-translate_x-5.0,-translate_y-5.0,-translate_z-5.0, -translate_x,-translate_y,-translate_z, 0.0, 1.0, 0.0);
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/
	view = rotate_y_deg(view,rotate_by_key);
	view = translate (view, vec3 (translate_x, translate_y, translate_z));
	view = translate (view, vec3 (0.0, 0.0, 2.0f));
	model = translate (model, vec3 (1.25, -1.0, -3.5f));
	model = translate(model,vec3(-translate_x,-translate_y,-translate_z));
	model = rotate_x_deg(model,-lookupordowntheway);
	model = rotate_y_deg(model,-rotate_by_key);}
	//view = look_at (vec3(translate_x,translate_y,translate_z), vec3(translate_x+5.0,translate_y+5.0,translate_z+5.0), vec3(0,1,0));
	//model = rotate_x_deg(model,lookupordowntheway);
	//model = rotate_y_deg(model,rotate_by_key);
	//glBindVertexArray(vao1);
	// update uniforms & draw
	// Start texture load
  
    //glGenTextures(1, &texture3);
    //glBindTexture(GL_TEXTURE_2D, texture3);
	if(shipskin==3){glBindTexture(GL_TEXTURE_2D, texture9);}
	if(shipskin==2){glBindTexture(GL_TEXTURE_2D, texture7);}
	if(shipskin==1){glBindTexture(GL_TEXTURE_2D, texture2);}
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model.m);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
 
	/*mat4 modelgun1 = identity_mat4 ();
	
	modelgun1 = rotate_y_deg(modelgun1,10);
	modelgun1 = rotate_x_deg(modelgun1,30);
	//modelgun1 = translate (modelgun1, vec3 (-3.0, -3.0, -1.5));
	modelgun1 = translate(modelgun1,vec3(-translate_x,-translate_y,-translate_z));
	modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	mat4 globalgun = model*modelgun1;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalgun.m);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);*/


	mat4 bullet1 = identity_mat4 ();
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	bullet1 = translate(bullet1,vec3(-2.5, 0.0, 0.0));
	//bullet1 = translate(bullet1,vec3(-translate_x,-translate_y,-translate_z));
	//modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	//modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	
	mat4 globalgun = model*bullet1;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	if(shipskin==3){glBindTexture(GL_TEXTURE_2D, texture9);}
		if(shipskin==2){glBindTexture(GL_TEXTURE_2D, texture7);}
		if(shipskin==1){glBindTexture(GL_TEXTURE_2D, texture2);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalgun.m);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);


	mat4 bulletx = identity_mat4 ();
	int shootreset;
	if(showbullet){

	
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
		//bulletx = scale(bulletx, vec3(scale1,scale1,scale1));
	bulletx = translate(bulletx,vec3(-5.0, 0.0, -shoot));
	bulletx = scale(bulletx,vec3(0.5,0.5,0.5));
	//bullet1 = translate(bullet1,vec3(-translate_x,-translate_y,-translate_z));
	//modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	//modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	
	shoot=shoot+0.5;
	
	mat4 globalbullet = model*bulletx;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalbullet.m);
	
	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	}
	if(shoot>80.0^!showbullet){
		shoot=0.0;}
//f(!showbullet){shoot=0.0;}100.0
	int saveddist2;
	
		if(shoot>40.0&&shoot<50.0){
		saveddist2=shoot;
		}
		if(shoot>80.0^!showbullet){
		shoot=0.0;}








	float bulletx2x = translate_x;
	float bulletx2y = translate_y;
	mat4 bulletx2= identity_mat4 ();
	
	bulletx2 = scale(bulletx2,vec3(0.5,0.5,0.5));
	//bulletx2 = translate(bulletx2,vec3(1.25,0.0,-shoot2));
	if(viewselect==3^viewselect==4){
	bulletx2 = translate(bulletx2,vec3(0.0,0.0,-shoot2/2));
	}
	if(viewselect==1^viewselect==2){
	bulletx2= translate (bulletx2, vec3 (1.25, -1.0, -2.0));
	bulletx2 = translate(bulletx2,vec3(0.0,0.0,-translate_z));
	bulletx2 = translate(bulletx2,vec3(-translate_x,-translate_y,-shoot2/2));
	}
	//bulletx2 = rotate_x_deg(bulletx2,-lookupordowntheway);
	//bulletx2 = rotate_y_deg(bulletx2,-rotate_by_key);
	if(showbullet2){

	
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
		//bulletx2 = scale(bulletx2, vec3(0.5,0.5,0.5));
	
	
	//bullet1 = translate(bullet1,vec3(-translate_x,-translate_y,-translate_z));
	//modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	//modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	
	shoot2=shoot2+0.5;
	mat4 globalbullet2 = model*bulletx2;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	glBindTexture(GL_TEXTURE_2D, texture2);
	if(viewselect==3^viewselect==4){
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalbullet2.m);
	}
	if(viewselect==1^viewselect==2){
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, bulletx2.m);
	}
	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	}
	int saveddist;
	
		if(shoot2>40.0&&shoot2<50.0){
		saveddist=shoot2;
		}
		if(shoot2>80.0^!showbullet2){
		shoot2=0.0;}
//f(!showbullet){shoot=0.0;}100.0




	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
/*
	if(showbullet){
	mat4 bullet2 = identity_mat4 ();
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	//
	//bullet2 = translate(bullet2,vec3(-2.5, 0.0, shoot));
	//bullet2 = translate(bullet1,vec3(0.5,0.0,0.0));
	bullet2 = translate(bullet1,vec3(-translate_x,-translate_y,-shoot));
	//bullet2 = scale(bullet2,vec3(2.0,2.0,2.0));
	//modelgun1 = rotate_x_deg(modelgun1,-lookupordowntheway);
	//modelgun1 = rotate_y_deg(modelgun1,-rotate_by_key);
	//bullet2
	shoot+=0.1;
	int shoot2=0;
	shoot2 = shoot;
	
		
	//if(showbullet){bullet2 = translate(bullet2,vec3(0.0,0.0,+shoot));}
	mat4 globalgun1 = model*bullet2;
	//bullet2 = translate(bullet1,vec3(0.0,0.0,0.0));
	
	//bullet2 = translate(bullet2,vec3(-2.5, 0.0, 0.0));
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalgun1.m);
	glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	/*if(showbullet){
			//shoot=shoot+0.1;
		//bullet2 = translate(bullet2,vec3(-translate_x,-translate_y,-translate_z));
			glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
		
	}		
	
	//shoot=shoot+0.01;
	

*/



	//start of asteroid, asteroid 1
	//glBindVertexArray(vao);
	// Root of the Hierarchy
	//mat4 view1 = identity_mat4 ();
	//mat4 persp_proj1 = perspective(120.0, (float)width/(float)height, 0.1, 1000.0);
	mat4 model1 = identity_mat4 ();
	model1 = rotate_y_deg (model1, rotate_y); 
	model1 = translate (model1, vec3 (0.0, 0.0, -40.0f));
	//model1= scale(model1,vec3(1.0,1.0,1.0));  // glScalef(0.5f, 0.5f, 0.5f);
	mat4 global1 = model1;
	// update uniforms & draw
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj1.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	
	float zboundaryupper=40;
	float zboundarylower=50;
	float xboundaryupper=10;
	float xboundarylower=10;
	float yboundaryupper=10;
	float yboundarylower=10;
	if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper&&saveddist<zboundarylower)&&(-bulletx2x>=-7.5&&-bulletx2x<=7.5)&&(-bulletx2y>=-7.5&&-bulletx2y<=7.5)){
		
		if((showbullet2^showbullet)^(showbullet2&&showbullet)){
		stopdraw=1;
		score=score-1;
		}
	}
	if(stopdraw==0){
		glBindTexture(GL_TEXTURE_2D, texture4);
		glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model1.m);
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	
	}


	
	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model2 = identity_mat4 ();
	//model2 = rotate_z_deg (model2, rotate_y); 
	
	model2 = rotate_y_deg (model2, rotate_y); 
	model2 = translate (model2, vec3 (-20.0, -20.0, -40.0f));
	//model2= scale(model2,vec3(.5,.5,.5));
	mat4 global2 = global1*model2;

	//glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D,texture4);
	// update uniforms & draw
	
	//glBindTexture(GL_TEXTURE_2D,texturez1);
	float zboundaryupper1=40;
	float zboundarylower1=50;
	float xboundaryupper1=10;
	float xboundarylower1=30;
	if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper1&&saveddist<zboundarylower1)&&(-bulletx2x>=-xboundarylower1&&-bulletx2x<=-xboundaryupper1)&&(-bulletx2y<=-10&&-bulletx2y>=-25)){
		
		if((showbullet2^showbullet)^(showbullet2&&showbullet)){
		stopdraw1=1;
		score=score-1;
		}
	}
	if(stopdraw1==0){
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model2.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);

	}

	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model3 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0);
	model3 = rotate_y_deg (model3, rotate_y);
	model3 = translate (model3, vec3 (20.0, -20.0, -40.0f));
	//model2= scale(model2,vec3(.5,.5,.5));
	 
	mat4 global3 = global2*model3;
	float zboundaryupper2=40;
	float zboundarylower2=50;
	float xboundaryupper2=10;
	float xboundarylower2=30;
	float yboundaryupper2=10;
	float yboundarylower2=30;
	if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper2&&saveddist<zboundarylower2)&&(-bulletx2x<=30&&-bulletx2x>=10)&&(-bulletx2y<=-10&&-bulletx2y>=-30)){
		
		if((showbullet2^showbullet)^(showbullet2&&showbullet)){
		stopdraw2=1;
		score=score-1;
		}
	}
	if(stopdraw2==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model3.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}

	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model5 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	model5 = rotate_y_deg (model5, rotate_y); 
	model5 = translate (model5, vec3 (-20.0, 20.0, -40.0f));
	//model2= scale(model2,vec3(.5,.5,.5));
	
	mat4 global4 = global3*model5;

	float zboundaryupper4=40;
	float zboundarylower4=50;
	float xboundaryupper4=10;
	float xboundarylower4=30;
	float yboundaryupper4=10;
	float yboundarylower4=30;
	if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper2&&saveddist<zboundarylower2)&&(-bulletx2x<=-10&&-bulletx2x>=-30)&&(-bulletx2y<=30&&-bulletx2y>=10)){
		
		if((showbullet2^showbullet)^(showbullet2&&showbullet)){
		stopdraw3=1;
		score=score-1;
		}
	}
	if(stopdraw3==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model5.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}



	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model6 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	
	//model6= scale(model6,vec3(scale1,scale1,scale1));
	model6 = rotate_y_deg (model6, rotate_y); 
	model6 = translate (model6, vec3 (20.0, 20.0, -40.0f));
	mat4 global5 = global4*model5;

	if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper2&&saveddist<zboundarylower2)&&(-bulletx2x>=10&&-bulletx2x<=30)&&(-bulletx2y<=30&&-bulletx2y>=10)){
		
		if((showbullet2^showbullet)^(showbullet2&&showbullet)){
		stopdraw4=1;
		score=score-1;
		}
	}
	if(stopdraw4==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model6.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}


	//glBindVertexArray(vao);
	// Root of the Hierarchy
	mat4 model7 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	model7 = rotate_y_deg (model7, rotate_y);
	model7 = translate (model7, vec3 (-40.0, 0.0, -40.0f));
	 
	//model4= scale(model2,vec3(1.2,1.2,1.2));
	
	mat4 global6 = global5*model7;
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,texture2);
	if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper2&&saveddist<zboundarylower2)&&(-bulletx2x<=-30&&-bulletx2x>=-50)&&(-bulletx2y>=-10&&bulletx2y<=10)){
		
		if((showbullet2^showbullet)^(showbullet2&&showbullet)){
		stopdraw5=1;
		score=score-1;
		}
	}
	if(stopdraw5==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model7.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	


	mat4 model8 = identity_mat4 ();
	//view2 = rotate_x_deg (model1, 0.0); 
	model8 = rotate_y_deg (model8, rotate_y);
	model8 = translate (model8, vec3 (40.0, 0.0, -40.0f));
	 
	//model4= scale(model2,vec3(1.2,1.2,1.2));
	
	mat4 global7 = global6*model8;
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,texture2);
	if((saveddist2>zboundaryupper&&saveddist2<zboundarylower)^(saveddist>zboundaryupper2&&saveddist<zboundarylower2)&&(-bulletx2x>=35&&-bulletx2x<=50)&&(-bulletx2y>=-10&&bulletx2y<=10)){
		
		if((showbullet2^showbullet)^(showbullet2&&showbullet)){
		stopdraw6=1;
		score=score-1;
		}
	}
	if(stopdraw6==0){
	// update uniforms & draw
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model8.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}



	//glColor3d(1,0,0); 
 
        
	/*
		//the sun
	//glBindVertexArray(vao);
	// Root of the Hierarchy
	//mat4 view2 = identity_mat4 ();
	//mat4 persp_projsun = perspective(90.0, (float)width/(float)height, 0.1, 100.0);
	mat4 modelsun = identity_mat4 ();
	//modelsun = rotate_x_deg (modelsun, 0.0); 
	modelsun = translate (modelsun, vec3 (2.0, 2.0, -2.0f));
	modelsun= scale(modelsun,vec3(5.0,5.0,5.0));  // glScalef(0.5f, 0.5f, 0.5f);
	modelsun= scale(modelsun,vec3(2.0,2.0,2.0));
    modelsun= scale(modelsun,vec3(2.0,2.0,2.0));
	mat4 globalsun = modelsun;
	// update uniforms & draw
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projsun.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalsun.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count4);
	*/
	
	
	/*
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, ship);
	*/
	
	//
	//enemy ships
	//mat4 view2 = identity_mat4 ();
	//mat4 persp_enemyship = perspective(90.0, (float)width/(float)height, 0.1, 100.0);
	
	
	//glViewport(1.0,0.5,400,300);
	//glDepthRange(0.0, 0.1);


	mat4 shipx = identity_mat4 ();
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	shipx = scale(shipx, vec3(2.0,2.0,2.0));
	shipx = translate(shipx,vec3(-7.0,-1.25, -2.5));
	//shipx= translate (shipx, vec3 (1.25, -1.0, -3.5f));
	//shipx = translate(shipx,vec3(0.0,0.0,-translate_z));

	//shipx = translate(shipx,vec3(-bulletx2x,-bulletx2y,-shoot2));
	shipx = rotate_x_deg(shipx,180);
	shipx = rotate_y_deg(shipx,30.0);
	shipx = rotate_z_deg(shipx,90.0);
	mat4 globalship= model*shipx;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	if(shipskin==3){glBindTexture(GL_TEXTURE_2D, texture9);}
	if(shipskin==1){glBindTexture(GL_TEXTURE_2D, texture2);}
	if(shipskin==2){glBindTexture(GL_TEXTURE_2D, texture7);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalship.m);
	
	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	//enemy ships
	//mat4 view2 = identity_mat4 ();
	//mat4 persp_enemyship = perspective(90.0, (float)width/(float)height, 0.1, 100.0);
	mat4 enemyship1 = identity_mat4 ();
	//enemyship = rotate_x_deg (enemyship, 0.0); 
	enemyship1 = translate (enemyship1, vec3 (12.0,-10.0, -50.0f));
	enemyship1 = translate (enemyship1, vec3 (0.0,-spin,0.0));
	if(spinreverse==0){
		spin=spin+0.05;
		if(spin>=21.0){spinreverse=1;}
	}
	if (spinreverse==1){spin=spin-0.05;
	if(spin<=-18.0){spinreverse=0;}
	}
	enemyship1 = rotate_x_deg(enemyship1,0);
	
	enemyship1 = rotate_z_deg(enemyship1,90.0);
	enemyship1 = rotate_y_deg(enemyship1,10.0);
	
	enemyship1= scale(enemyship1,vec3(1.5,1.5,1.5));
	mat4 globalenemyship1 = enemyship1;
	// update uniforms & draw
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,texture4);
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_enemyship.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalenemyship1.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	


	mat4 enemyshipx = identity_mat4 ();
	//int slide;
	//enemyship = rotate_x_deg (enemyship, 0.0); 
	enemyshipx = translate (enemyshipx, vec3 (-40.0, 10.0, 0.0f));
	//enemyshipx = translate (enemyshipx, vec3 (-slide, 0.0,0.0));
	//enemyshipx = rotate_x_deg(enemyshipx,90);
	
	//enemyshipx = rotate_z_deg(enemyshipx,0.0);
	//enemyshipx = rotate_y_deg(enemyshipx,90.0);
		//enemyshipx = translate (enemyshipx, vec3 (-slide, 0.0,0.0));
	//slide++;
	//enemyship= scale(enemyship,vec3(5.0,5.0,5.0));
	mat4 globalenemyshipx = enemyship1*enemyshipx;
	// update uniforms & draw
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_enemyship.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	//glGenTextures(1, &texture3);
	//glActiveTexture(GL_TEXTURE0);

	
	//glBindTexture(GL_TEXTURE_2D,texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalenemyshipx.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	
	if(stopdraw&&stopdraw1&&stopdraw2&&stopdraw3&&stopdraw4&&stopdraw5&&stopdraw6){
	reset=1;
	}



	/*
	mat4 shipxz = identity_mat4 ();
	//bullet1 = rotate_y_deg(bullet1,-10);
	//bullet1 = rotate_x_deg(bullet1,30);
	shipxz = scale(shipxz, vec3(2.0,2.0,2.0));
	shipxz = translate(shipxz,vec3(-7.0,-1.25, -2.5));
	//shipx= translate (shipx, vec3 (1.25, -1.0, -3.5f));
	//shipx = translate(shipx,vec3(0.0,0.0,-translate_z));

	//shipx = translate(shipx,vec3(-bulletx2x,-bulletx2y,-shoot2));
	shipxz = rotate_x_deg(shipxz,180);
	shipxz = rotate_y_deg(shipxz,30.0);
	shipxz = rotate_z_deg(shipxz,90.0);
	mat4 globalshipz= model*shipxz;
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_projgun.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalshipz.m);
	
	//glDrawArrays (GL_TRIANGLES, 0, g_point_count1);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	//enemy ships
	//mat4 view2 = identity_mat4 ();
	//mat4 persp_enemyship = perspective(90.0, (float)width/(float)height, 0.1, 100.0);
	mat4 enemyship1z = identity_mat4 ();
	//enemyship = rotate_x_deg (enemyship, 0.0); 
	
	enemyship1z = rotate_x_deg(enemyship1z,0);
	
	enemyship1z = rotate_z_deg(enemyship1z,90.0);
	enemyship1z = rotate_y_deg(enemyship1z,10.0);
	enemyship1z = translate (enemyship1z, vec3 (0.0, 0.0, -10.0f));
	enemyship1z= scale(enemyship1z,vec3(1.5,1.5,1.5));
	mat4 globalenemyship1z = enemyship1z;
	// update uniforms & draw
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_enemyship.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,texture4);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalenemyship1z.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	
	*//*
		float text_x = -0.95f;
	float text_y = -0.85f;
	float size_px = 100.0f;
	float text_r = 0.0f;
	float text_g = 0.75f;
	float text_b = 0.0f;
	float text_a = 1.0f;
	int score_id = add_text ("Score: 0", text_x, text_y, size_px, text_r, text_g, text_b, text_a);
	int score =12;
	//char tmp[256];
	//	sprintf (tmp, "Score: %d\n", score);
	//update_text (score_id, tmp);*/
	
	
	glViewport(400.0,0.0,400,300);
	glDepthRange(0.0, 0.1);
	//mat4 globalenemyshipx = enemyship1*enemyshipx;
	// update uniforms & draw
	//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_enemyship.m);
	//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view1.m);
	//glGenTextures(1, &texture3);
	//glActiveTexture(GL_TEXTURE0);

	//int x1 = 1024;
	//int y1 = 1024;
	//int n1 = 4;
   // unsigned char *man = stbi_load("meteor2.PNG", &x, &y, &n, 0);
	//glGenTextures(1, &stuff);
	//glActiveTexture (GL_TEXTURE0);
   // glBindTexture(GL_TEXTURE_2D, stuff);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,man);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D,texture3);
	mat4 view4 = translate (identity_mat4 (), vec3 (0.0, 0.0, 0.0));
	mat4 ortho_proj4 = Ortho(0.0,300.0,400.0,0.0,0.2,100);
	if(gamemode==1){
	if(mapsetting==1^mapsetting==2){
	if(stopdraw==0){
		if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
		if(mapsetting==1){glBindTexture(GL_TEXTURE_2D, texture5);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model1.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	if(stopdraw2==0){
		if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
		if(mapsetting==1){glBindTexture(GL_TEXTURE_2D, texture5);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model3.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	if(stopdraw1==0){
		if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
		if(mapsetting==1){glBindTexture(GL_TEXTURE_2D, texture5);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model2.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	if(stopdraw3==0){
		if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
		if(mapsetting==1){glBindTexture(GL_TEXTURE_2D, texture5);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model5.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	if(stopdraw4==0){
		if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
		if(mapsetting==1){glBindTexture(GL_TEXTURE_2D, texture5);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model6.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	if(stopdraw5==0){
		if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
		if(mapsetting==1){glBindTexture(GL_TEXTURE_2D, texture5);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model7.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	if(stopdraw6==0){
		if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
		if(mapsetting==1){glBindTexture(GL_TEXTURE_2D, texture5);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model8.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count1,g_point_count2);
	}
	}
	}
	//globalship = scale(globalship,vec3(0.5,0.5,0.5));
	if(shipskin==3){glBindTexture(GL_TEXTURE_2D, texture9);}
	if(shipskin==1){glBindTexture(GL_TEXTURE_2D, texture2);}
	if(shipskin==2){glBindTexture(GL_TEXTURE_2D, texture7);}
	if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalship.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);

	if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
	if(mapsetting==1^mapsetting==0){glBindTexture(GL_TEXTURE_2D, texture6);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalenemyship1.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	
	//globalenemyshipx = scale(globalenemyshipx,vec3(2.0,2.0,2.0));
	if(mapsetting==2){glBindTexture(GL_TEXTURE_2D, texture8);}
	if(mapsetting==1^mapsetting==0){glBindTexture(GL_TEXTURE_2D, texture6);}
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, globalenemyshipx.m);
	
	glDrawArrays (GL_TRIANGLES, g_point_count2,g_point_count3);
	
	
	
	}
	
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
	//DemoLight();
	generateObjectBufferMesh();
	//ship = stbi_load("img_test1.png", &widthtex, &heighttex, &bitdepth, 0);
	//generateObjectBufferMesh1();
	
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	if(key=='x'){
		//Translate the base, etc.
	}

}
// -----------------------------------
//             MENUS
// -----------------------------------



void processMainMenu(int option) {

	// nothing to do in here
	// all actions are for submenus
}

void processFillMenu(int option) {

	switch (option) {

		case ShootingMode: gamemode=1;reset=1; break;
		case RacingMode: gamemode=0;reset=1; break;
	}
}

void processSkinMenu(int option) {

	switch (option) {

	case SKIN1: shipskin=1; break;
	case SKIN2: shipskin=2; break;
	case SKIN3: shipskin=3; break;
	}
}

void processMapType(int option) {

	switch (option) {

	case MAP1: mapsetting=1; break;
	case MAP2: mapsetting=2; break;
	case MAP3: mapsetting=0; break;
	}
}

void processViewMenu(int option) {

	switch (option) {

		case RoamingCamera: viewselect = 1; break;
		case SideView: viewselect = 2; break;
		case FirstPersonView: viewselect = 3; break;
		case ThirdPerson: viewselect = 4; break;
	}
}
void processColorMenu(int option) {

	switch (option) {
		case RED :
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f; break;
		case GREEN :
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f; break;
		case BLUE :
			red = 0.0f;
			green = 0.0f;
			blue = 1.0f; break;
		case ORANGE :
			red = 1.0f;
			green = 0.5f;
			blue = 0.5f; break;
	}
}

void createPopupMenus() {

	viewMenu = glutCreateMenu(processViewMenu);

	glutAddMenuEntry("Roaming Camera",RoamingCamera);
	glutAddMenuEntry("Side View",SideView);
	glutAddMenuEntry("FirstPersonView",FirstPersonView);
	glutAddMenuEntry("ThirdPerson",ThirdPerson);

	fillMenu = glutCreateMenu(processFillMenu);

	glutAddMenuEntry("Shooting Mode",ShootingMode);
	glutAddMenuEntry("Racing Mode",RacingMode);

	skinMenu = glutCreateMenu(processSkinMenu);

	glutAddMenuEntry("Skin 1",SKIN1);
	glutAddMenuEntry("Skin 2",SKIN2);
	glutAddMenuEntry("Skin 3",SKIN3);

	mapMenu = glutCreateMenu(processMapType);

	glutAddMenuEntry("Regular",MAP1);
	glutAddMenuEntry("Radar",MAP2);
	glutAddMenuEntry("Enemies",MAP3);

	colorMenu = glutCreateMenu(processColorMenu);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Blue",BLUE);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("Orange",ORANGE);

	mainMenu = glutCreateMenu(processMainMenu);

	glutAddSubMenu("Game Mode", fillMenu);
	//glutAddSubMenu("Color", colorMenu);
	glutAddSubMenu("MiniMap Type", mapMenu);
	glutAddSubMenu("View", viewMenu);
	glutAddSubMenu("Ship Skins", skinMenu);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// this will allow us to know if the menu is active
	//glutMenuStatusFunc(processMenuStatus);
}



int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Hello Triangle");
	




	/*
	// start the sound engine with default parameters
   ISoundEngine* engine = createIrrKlangDevice();

   if (!engine)
      return 0; // error starting up the engine
   // play some sound stream, looped
   
   engine->play2D("../../media/getout.ogg", true);
   std::cout << "\nHello World!\n";

   char i = 0;

   while(i != 'q')
   {
      std::cout << "Press any key to play some sound, press 'q' to quit.\n";

      // play a single sound
      engine->play2D("../../media/bell.wav");		

      std::cin >> i; // wait for user to press some key
   }
   engine->drop(); // delete engine
   */
	/*
	assert (init_text_rendering (atlas_image, atlas_meta, width, height));
	printf ("adding text...\n");
	int hello_id = add_text (
		"Hello\nis it me you're looking for?",
		-0.9f, 0.5f, 35.0f, 0.5f, 0.5f, 1.0f, 1.0f);
	
	int time_id = add_text (
		"The time is:",
		-1.0f, 1.0f, 40.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	move_text (time_id, -1.0f, 1.0f);*/
	// Tell glut where the display function is
	glutDisplayFunc(display);

	glutIdleFunc(updateScene);
	glutSpecialFunc(keyPress);
	createPopupMenus();

	 // A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	const GLubyte* renderer = glGetString (GL_RENDERER);
	const GLubyte* version = glGetString (GL_VERSION);
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version: %s\n", version);
	//assert (init_text_rendering (atlas_image, atlas_meta, width, height));
	printf ("adding text...\n");
	/*int hello_id = add_text (
		"Hello\nis it me you're looking for?",
		-0.9f, 0.5f, 35.0f, 0.5f, 0.5f, 1.0f, 1.0f);
	
	int time_id = add_text (
		"The time is:",
		-1.0f, 1.0f, 40.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	move_text (time_id, -1.0f, 1.0f);
	
	*/
	
	
	printf("score:%f",score);
	 int x = 1024;
	int y = 1024;
	int n = 4;
    unsigned char *Enemyspaceships = stbi_load("enemyships.PNG", &x, &y, &n, 0);
    unsigned char *spaceship = stbi_load("textureships.PNG", &x, &y, &n, 0);
	unsigned char *asteroidtexture = stbi_load("AsteroidsTexture.PNG", &x, &y, &n, 0);
	unsigned char *shipmap = stbi_load("texture1234.PNG", &x, &y, &n, 0);
	unsigned char *asteroidsminimap = stbi_load("minimaptexture1.PNG", &x, &y, &n, 0);
	unsigned char *minimapradar = stbi_load("Radar1.PNG", &x, &y, &n, 0);
	unsigned char *minimapenemies = stbi_load("minimapenemies.PNG", &x, &y, &n, 0);
	unsigned char *shipskin2 = stbi_load("ship2.PNG", &x, &y, &n, 0);
	unsigned char *shipskin3 = stbi_load("Shipskin2.PNG", &x, &y, &n, 0);
	//unsigned char *shipskin = stbi_load("shipskin1.PNG", &x, &y, &n, 0);
	initialise_texture(&texture3, GL_TEXTURE0, x, y, Enemyspaceships);
	initialise_texture(&texture2, GL_TEXTURE0, x, y, spaceship);
	initialise_texture(&texture4, GL_TEXTURE0, x, y, asteroidtexture);
	initialise_texture(&texture5, GL_TEXTURE0, x, y, asteroidsminimap);
	initialise_texture(&texture8, GL_TEXTURE0, x, y, minimapradar);
	initialise_texture(&texture6, GL_TEXTURE0, x, y, minimapenemies);
	initialise_texture(&texture7, GL_TEXTURE0, x, y, shipskin2);
	initialise_texture(&texture9, GL_TEXTURE0, x, y, shipskin3);
	//initialise_texture(&texture5, GL_TEXTURE0, x, y, shipskin);
	
	//draw_texts ();
	// Check for any errors
	//PlaySound("Audio/music.mp3", NULL, SND_SYNC);
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	 /* if (score==0)           
        {        
        PlaySound("Audio/AphexTwin-Xtal.mp3", NULL, SND_SYNC); 
	  }*/
	// Set up your objects and shaders
	init();
	// Begin infinite event loop

	
	glutMainLoop();

    return 0;
}












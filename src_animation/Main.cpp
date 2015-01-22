/////////////////////////////////////////////
//
// Skeletal Animation Tutorial
//
// (C) by Sven Forstmann in 2014
//
// License : MIT
// http://opensource.org/licenses/MIT
/////////////////////////////////////////////
// Mathlib included from 
// http://sourceforge.net/projects/nebuladevice/
/////////////////////////////////////////////
#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h>
#include <glew.h>
#include <wglew.h>
#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
#include <strsafe.h>

using namespace std;
#pragma comment(lib,"winmm.lib")
///////////////////////////////////////////
#include "core.h"
#include "Bmp.h"
#include "ogl.h"
#include "glsl.h"
///////////////////////////////////////////
vec4f lightvec(0.5,0.5,0.5,0);
#include "Mesh.h"

///////////////////////////////////////////
static int times = 20;

float scale = 1;
GLint x = 1;
GLint y = 1;
GLint z = 1;

GLfloat roat_x, roat_y = 0;

int foot_id = -1;
//static CColorBasics* ctest = new CColorBasics();
GLuint kinect_texture_id = 7;
GLfloat background_point1[] = { -1. * times, -1.  * times }, background_point2[] = { 1. * times, -1.  * times }, background_point3[] = { -1. * times, 1. * times }, background_point4[] = { 1.* times, 1. * times };
///////////////////////////////////////////
/*
void update()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &kinect_texture_id);
	glBindTexture(GL_TEXTURE_2D, kinect_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	NUI_LOCKED_RECT* test_rect = ctest->getPixls();
	if (test_rect != NULL) {

		byte* test_p = test_rect->pBits;

		byte k = 0;

		for (int i = 0; i < test_rect->size; i += 4)
		{
			k = test_p[i];
			test_p[i] = test_p[i + 2];
			test_p[i + 2] = k;
			//printf("data:%d\n", test_p[i + 3]);
			test_p[i + 3] = 255;
		}

		//ctest->SaveBitmapToFile(static_cast<BYTE *> (test_p), 640, 480, 32, screenshotPath);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 960, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, test_p);
	}
}
*/

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &kinect_texture_id);
	glBindTexture(GL_TEXTURE_2D, kinect_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	Bmp bmp("H:/Í¼±ê/ai.bmp", true);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp.width, bmp.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
}

void DrawScene()
{
	if ( GetAsyncKeyState(VK_ESCAPE) )  exit(0);

	// mouse pointer position
	POINT cursor;
	GetCursorPos(&cursor); 

	// camera orientation
	float	viewangle_x = float(cursor.x-1280/2)/4.0;
	float	viewangle_y = float(cursor.y-768/2)/4.0;

	//time
	static uint t0=timeGetTime(); 
	double time_elapsed=double(timeGetTime()-t0)/1000;

	// clear and basic
	glClearDepth(1);
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// projection
	int vp[4];
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, vp);
	gluPerspective(90.0,float(vp[2])/float(vp[3]) , 0.01, 100);

	// modelview
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	glRotatef( viewangle_y,1,0,0);		// set rotation
	glRotatef( viewangle_x,0,1,0);		// set rotation

	// select level of detail for rendering
	// (lods are generated automatically by the ogre xml converter )

	int lod=0; 
	if(GetAsyncKeyState(VK_F1)) lod=1;
	if(GetAsyncKeyState(VK_F2)) lod=2;
	if(GetAsyncKeyState(VK_F3)) lod=3;
	if(GetAsyncKeyState(VK_F4)) lod=4;
	if(GetAsyncKeyState(VK_F5)) lod=5;

	char* material = "../data/halo/halo.material";
	char* mesh = "../data/halo/halo.mesh.xml";
	char* skeleton = "../data/jean/jean.skeleton.xml";

	static Mesh pants(material,
		mesh
		);//	

	if (foot_id >= 0)
	{


		foot_id = -1;
	}
		
	pants.Draw(
		vec3f(-5, -5, -5),		  		// position
		vec3f(
		roat_x,			// rotation
		roat_y,
		1),
		vec3f(
		scale, 
		scale,
		scale)
		); 

	// Swap


	///////////////////////////////////////////////////////////////////
	//update();
	/*
	glPushMatrix();
	glTranslatef(0, 0, -times);
	glBindTexture(GL_TEXTURE_2D, kinect_texture_id);
	glBegin(GL_QUAD_STRIP);

	glTexCoord2f(0., 1.);
	glVertex2fv(background_point1);

	glTexCoord2f(1., 1.);
	glVertex2fv(background_point2);

	glTexCoord2f(0., 0.);
	glVertex2fv(background_point3);

	glTexCoord2f(1., 0.);
	glVertex2fv(background_point4);

	glEnd();
	glPopMatrix();
	*/
	/////////////////////////////////////////////////////////////////


	glutSwapBuffers();
}
///////////////////////////////////////////

void keyboard(unsigned char key, int x1, int y1)
{
	switch (key)
	{
	case '0':
		foot_id = 0;
		break;
	case '1':
		foot_id = 1;
		break;
	case '2':
		foot_id = 2;
		break;
	case '3':
		foot_id = 3;
		break;
	case '4':
		foot_id = 4;
		break;
	case '5':
		foot_id = 5;
		break;
	case '6':
		foot_id = 6;
		break;
	case '7':
		foot_id = 7;
		break;
	case '8':
		foot_id = 8;
		break;
	case '9':
		foot_id = 9;
		break;
	case 'w':
		printf("up");
		scale *= 2;
		break;
	case 's':
		printf("down");
		scale /= 2;
		break;
	case 'i':
		roat_y++;
		break;
	case 'j':
		roat_x++;
		break;
	case 'l':
		roat_x--;
		break;
	case 'k':
		roat_y--;
		break;
	default:
		break;
	}
}


int main(int argc, char **argv) 
{ 
  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
  glutInitWindowSize(1280, 768);  
  glutInitWindowPosition(0, 0);  
  glutCreateWindow("Skinned Skeletal Animation Sample (c) Sven Forstmann in 2014");

  init();
  glutDisplayFunc(DrawScene);
  glutIdleFunc(DrawScene);
  glutKeyboardFunc(keyboard);
  glewInit();
  wglSwapIntervalEXT(0);
  glutMainLoop();  
  return 0;
}
///////////////////////////////////////////

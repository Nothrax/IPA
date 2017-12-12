/*
*Student login:xtrubk00
*/
#include <cstdio>

#ifdef DLLDIR_EX
#define DLLDIR  __declspec(dllexport) 
#else
#define DLLDIR  __declspec(dllimport) 
#endif 

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_image.h>

HINSTANCE hInstLibraryZajic = LoadLibrary("Student_DLL.dll");
typedef void(*Ipa_zajochSet)(int i);
Ipa_zajochSet chngStamAssembly = (Ipa_zajochSet)GetProcAddress(hInstLibraryZajic, "chngStam");

typedef int(*Ipa_zajochGet)();
Ipa_zajochGet getBoostJAssembly = (Ipa_zajochGet)GetProcAddress(hInstLibraryZajic, "getBoostJ");
Ipa_zajochGet getBoostSAssembly = (Ipa_zajochGet)GetProcAddress(hInstLibraryZajic, "getBoostS");


extern "C" DLLDIR void chngStam(int i) {
	chngStamAssembly(i);
	return;
}

extern "C" DLLDIR int getBoostJ() {
	int i = getBoostJAssembly();
	return i;
}

extern "C" DLLDIR int getBoostS() {
	int i = getBoostSAssembly();
	return i;
}

extern "C" DLLDIR void  printcube()
	{

		glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
										  // Top face (y = 1.0f)
										  // Define vertices in counter-clockwise (CCW) order with normal pointing out
		glColor3f(0.0f, 1.0f, 0.0f);     // Green
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		// Bottom face (y = -1.0f)
		glColor3f(1.0f, 0.5f, 0.0f);     // Orange
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		// Front face  (z = 1.0f)
		glColor3f(1.0f, 0.0f, 0.0f);     // Red
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		// Back face (z = -1.0f)
		glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);

		// Left face (x = -1.0f)
		glColor3f(0.0f, 0.0f, 1.0f);     // Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);

		// Right face (x = 1.0f)
		glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glEnd();  // End of drawing color-cube
	}


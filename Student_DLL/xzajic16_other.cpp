/*
*Student login:xzajic16
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

#define W_WIDTH 900
#define W_HEIGHT 500

const Uint8 *keystates = SDL_GetKeyboardState(NULL);
SDL_Event Event;

static int active = 0;

HINSTANCE hInstLibrary = LoadLibrary("Student_DLL.dll");

typedef int(*int_int)(int c);
int_int getType = (int_int)GetProcAddress(hInstLibrary, "getType");
int_int getCount = (int_int)GetProcAddress(hInstLibrary, "getCount");

typedef int(*geter)();
geter getActive = (geter)GetProcAddress(hInstLibrary, "getActive");
geter getStam = (geter)GetProcAddress(hInstLibrary, "getStam");
geter getBoostS = (geter)GetProcAddress(hInstLibrary, "getBoostS");
geter getBoostJ = (geter)GetProcAddress(hInstLibrary, "getBoostJ");
geter getRange = (geter)GetProcAddress(hInstLibrary, "getRange");

typedef void(*setr)(int c);
setr setActive = (setr)GetProcAddress(hInstLibrary, "setActive");
setr insertItem = (setr)GetProcAddress(hInstLibrary, "insertItem");
setr removeItem = (setr)GetProcAddress(hInstLibrary, "insertItem");

typedef void(*flagclear)();
flagclear setUnpressed = (flagclear)GetProcAddress(hInstLibrary, "setUnpressed");

extern "C" DLLDIR void  printcube(int type)
	{
		
		if (type == 0) return;

		glEnable(GL_COLOR_MATERIAL);

		glBegin(GL_QUADS);

		if (type == 1) glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
		if (type == 2) glColor3f(1.0f, 0.0f, 0.0f);     // Red
		if (type == 3) glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
		if (type == 4) glColor3f(0.0f, 0.0f, 1.0f);     // Blue

		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);

		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glEnd();  // End of drawing color-cube
	}

extern "C" DLLDIR void printInv() {
	printf("\n******* Inventory *******\n");
	printf("Active item: %d\n", getActive());
	printf("Stamina: %d\n", getStam());
	printf("BoostS: %d \n", getBoostS());
	printf("BoostJ: %d \n", getBoostJ());
	printf("In range: %d \n", getRange());
	for (int i = 0; i < 10; i++) {
		printf("%d -> type: %d, count: %d\n", i, getType(i), getCount(i));
	}
	printf("*************************\n");
}

extern "C" DLLDIR void  drawInv() {
	int inRange = getRange();
	if (inRange) {
		glBegin(GL_QUADS);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(W_WIDTH / 2 + 15.0f, W_HEIGHT);
		glVertex2f(W_WIDTH / 2 + 15.0f, W_HEIGHT - 30.0f);
		glVertex2f(W_WIDTH / 2 - 15.0f, W_HEIGHT - 30.0f);
		glVertex2f(W_WIDTH / 2 - 15.0f, W_HEIGHT);

		glEnd();

		if (keystates[SDL_SCANCODE_E]) insertItem(inRange);
	}

	if (!keystates[SDL_SCANCODE_E]) setUnpressed();

	for (int i = 0; i < 10; i++) {
		int j = i + 1;
		if (keystates[SDL_SCANCODE_1 + i]) active = i;

		if (SDL_PollEvent(&Event)) {
			if (Event.wheel.y == 1 || Event.wheel.y == -1) {
				if (Event.wheel.y == 1) active--;
				else if (Event.wheel.y == -1) active++;

				if (active < 0) active = 9;
				if (active > 9) active = 0;
			}
			setActive(active);
		}


		glBegin(GL_QUADS);
		
		glColor3f(0.05f, 0.05f, 0.05f);  // Grey
		if (getType(i) == 1) glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
		if (getType(i) == 2) glColor3f(1.0f, 0.0f, 0.0f);     // Red
		if (getType(i) == 3) glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
		if (getType(i) == 4) glColor3f(0.0f, 0.0f, 1.0f);     // Blue

		if (i == getActive()) {
			glVertex2f(60, 30 + j * 40);
			glVertex2f(60, 0.0 + j * 40);			
		} else {
			glVertex2f(30, 30 + j * 40);
			glVertex2f(30, 0.0 + j * 40);
		}

		glVertex2f(0.0, 0.0 + j * 40);
		glVertex2f(0.0, 30 + j * 40);

		glEnd();

	}

}

extern "C" DLLDIR void  drawStam() {
	glBegin(GL_QUADS);

	glColor3f(1.0f, 0.6f, 0.0f);
	glVertex2f(getStam(), 30.0f);
	glVertex2f(getStam(), 10.0f);
	glVertex2f(0.0f, 10.0f);
	glVertex2f(0.0f, 30.0f);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(0.05f, 0.05f, 0.05f);
	glVertex2f(500.f, 30.0f);
	glVertex2f(500.f, 10.0f);
	glVertex2f(0.0f, 10.0f);
	glVertex2f(0.0f, 30.0f);

	glEnd();
}
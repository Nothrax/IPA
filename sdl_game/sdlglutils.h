#ifndef SDLGLUTILS_H
#define SDLGLUTILS_H


#include <GL/gl.h>
#include <SDL.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

GLuint loadTexture(const char * filename,bool useMipMap);
int takeScreenshot(const char * filename);
void drawAxis(double scale);
int XPMFromImage(const char * imagefile, const char * XPMfile);
SDL_Cursor * cursorFromXPM(const char * xpm[]);

#endif //SDLGLUTILS_H

/*Template for IPA project, 3D game
*Author: Tomas Goldmann, igoldmann@fit.vutbr.cz, most source code from Anton Claes (https://www.youtube.com/watch?v=a3MACw5hB2Q)
*
*STUDENT LOGIN:xzajic16
*/
#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath>
#include <time.h>


#include "obj.h"
#include "get_pixel.h"
#include "collision.h"
#include "geometry.h"
#include "randspawn.h"
#include "sound.h"
#include "ipa_tool.h"

#define ROBOTS 10

#define SPEED_COEF 3
#define EYE_HEIGHT 1.75 //hauteur des yeux au dessus du sol
#define PITCH .05

enum Act_Terrain
{
	LOAD, RETURN, UNLOAD
};
typedef enum Act_Terrain Act_Terrain;

Point defaut = { -1,0,0 };

void init_ogl();
void proceed(SDL_Event event);
double get_height(double x, double y, bool interpolated = true);//get terrain height in te actual point
Vector place_cam(SDL_Event event); 
objet gest_terrain(Act_Terrain action, char filename[] = "");
void disp_fling(objet fling, Vector camera);
robot *gest_robot(int r, Point posPlayer = defaut); //robot managment function
void shot(Vector camera);//
void painter(objet fling);


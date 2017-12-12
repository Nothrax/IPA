/*Template for IPA project, 3D game
*Anton Claes (https://www.youtube.com/watch?v=a3MACw5hB2Q)
*
*/

#include <windows.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <iostream>
#include "collision.h"

using namespace std;

bool point_ds_AABB3D(Point ptATester, AABB3D box)
{
    if( //the min coordinates
        ptATester.x >= box.coordMin.x &&
        ptATester.y >= box.coordMin.y &&
        ptATester.z >= box.coordMin.z &&
		//max coordinates
        ptATester.x <= box.coordMax.x &&
        ptATester.y <= box.coordMax.y &&
        ptATester.z <= box.coordMax.z)

        return true;
    else
        return false;

}

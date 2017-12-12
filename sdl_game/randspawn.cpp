/*Template for IPA project, 3D game
*Anton Claes (https://www.youtube.com/watch?v=a3MACw5hB2Q)
*
*/

#include "randspawn.h"

Point randSpawn()
{
    bool loaded=false;
    Point Pos;
    static int x=0;
    srand((unsigned int) time(NULL)+x);//init random
    x+=10;

    Pos.x = (rand() % 100);
    Pos.y = (rand() % 100);
    return Pos;
}

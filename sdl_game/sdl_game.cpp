/*Template for IPA project, 3D game
*Author: Tomas Goldmann, igoldmann@fit.vutbr.cz, most source code from Anton Claes (https://www.youtube.com/watch?v=a3MACw5hB2Q)
*
*STUDENT LOGIN: xzajic16, xtrubk00
*/

#include "sdl_game.h"



using namespace std;

SDL_Window *screen;

typedef void(*Printcube)(int type);
Printcube printcube;
/* xtrubk00 *********************************************************************************************/
typedef void(*Ipa_algorithm_aim_at)(static Point *aim, static Point *position, double angleZ, double angleY);
typedef void(*Ipa_algorithm_full_movement)(const Uint8 *keys, static Point *position, double angleZ, double angleY);
typedef void(*Ipa_algorithm_jump)(static Point *actualPos, double *lastPosZ, double speedCoef, double *jumpTime, bool *isJump, const Uint8 *keys, double terrain);
Ipa_algorithm_aim_at aimAtPosition;
Ipa_algorithm_full_movement fullMovement;
Ipa_algorithm_jump jumpMovement;
double jumpTime = 0;
bool isJump = false;
/* xtrubk00 *********************************************************************************************/


/* xzajic16 *********************************************************************************************/
#define W_WIDTH 900
#define W_HEIGHT 500

typedef void(*Draw)();
Draw drawInv;
Draw drawStam;

typedef void(*Colision)(static Point *pos);
Colision itemDetection;

typedef void(*Print)();
Print printInv;

typedef void(*setr)(int c);
setr removeItem;

typedef int(*geter)();
geter getActiveCount;

int pulses = 0;
/* xzajic16 *********************************************************************************************/

HINSTANCE hInstLibraryZajic;
HINSTANCE hInstLibraryTrubka;

//Funkce, ktera se vola v danem intervalu (muzete zde provadet vykreslovani)
Uint32 timer_callbackfunc(Uint32 interval, void *param)
{
	//printf("Callback called back with message: %s\n", (char*)param);

	/* xzajic16 *********************************************************************************************/
	//Pricitam si jednotlive pulsy (10ms kazda) pokud jsem na maximální hodnote resetuju
	pulses++;
	if (pulses == INT_MAX) pulses = 0;
	/* xzajic16 *********************************************************************************************/

	/* xtrubk00 *********************************************************************************************/
	if (isJump) {
		jumpTime += 0.08;
	}
	/* xtrubk00 *********************************************************************************************/
	return interval;
}

/* xzajic16*********************************************************************************************/
//Pri volani teto funkce kontroluji jestli ubehl dany interval, vyhodou je ze pomoci jednoho timeru mohu
//hlidat libovolny cas do INT_MAX
bool timeUp(int i) {
	i /= 10;

	return (pulses % i == i - 1) ? true : false;
}
/* xzajic16 *********************************************************************************************/

int main(int argc, char** argv)
{
	SDL_Event event;
	bool continuer = true;

	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);

	screen = SDL_CreateWindow("IPA - game 2017", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 900, 500, SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(screen);
	init_ogl();

	//IPA-TODO
	//V pripade, ze budete vyuzivat SDL timer pro casovani, tak si tady nastavte delay
	Uint32 delay = 10; 
	SDL_TimerID my_timer_id = SDL_AddTimer(delay, timer_callbackfunc, "parametr");

	HINSTANCE hInstLibraryZajic = LoadLibrary("Student_DLL.dll");
	HINSTANCE hInstLibraryTrubka = LoadLibrary("Student_DLL2.dll");

	if (!hInstLibraryZajic)
	{
		std::cout << "DLL Failed To Load!" << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		/* xzajic16 *********************************************************************************************/
		itemDetection = (Colision)GetProcAddress(hInstLibraryZajic, "itemDetection");
		printcube = (Printcube)GetProcAddress(hInstLibraryZajic, "printcube");
		drawInv = (Draw)GetProcAddress(hInstLibraryZajic, "drawInv");
		drawStam = (Draw)GetProcAddress(hInstLibraryZajic, "drawStam");
		printInv = (Print)GetProcAddress(hInstLibraryZajic, "printInv");

		removeItem = (setr)GetProcAddress(hInstLibraryZajic, "removeItem");
		getActiveCount = (geter)GetProcAddress(hInstLibraryZajic, "getActiveCount");
		/* xzajic16 *********************************************************************************************/
	}
	if (!hInstLibraryTrubka)
	{
		std::cout << "DLL2 Failed To Load!" << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		//IPA-TODO
		//Zde nacitejte ukazatele na funkce z DLL knihovny
		/* xtrubk00 *********************************************************************************************/
		aimAtPosition = (Ipa_algorithm_aim_at)GetProcAddress(hInstLibraryTrubka, "look_at");
		fullMovement = (Ipa_algorithm_full_movement)GetProcAddress(hInstLibraryTrubka, "full_movement");
		jumpMovement = (Ipa_algorithm_jump)GetProcAddress(hInstLibraryTrubka, "jump_movement");
		/* xtrubk00 *********************************************************************************************/
	}

	while (continuer)
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				continuer = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					continuer = false;
					break;

				default:
					break;
				}
				break;
			}
		}
		proceed(event);
		/* xzajic16 *********************************************************************************************/
		//Kazdou sekundu vypise aktualni obsah inventare a pomocne hodnoty
		if (timeUp(1000)) printInv();
		/* xzajic16 *********************************************************************************************/
	}

	quit_audio();
	FreeLibrary(hInstLibraryZajic);
	FreeLibrary(hInstLibraryTrubka);
	return 0;
}

void init_ogl()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (double)SCREEN_WIDTH / SCREEN_HEIGHT, .1, 1000);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glClearColor(0, 0, 0, 0);

	glEnable(GL_LIGHT0);
	GLfloat ambient[] = { 5,5,5,1 };
	GLfloat diffuse[] = { 1,3,3,5 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient);

	int MatSpec[4] = { 1,1,1,1 };
	glMaterialiv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpec);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 100);

	SDL_WarpMouseGlobal(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void proceed(SDL_Event event)
{
	static objet terrain;
	static objet fling;
	static objet grass;
	static objet skybox;
	static bool loaded = false;
	Vector camera;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!loaded)
	{
		fling.load("fling.obj");
		painter(fling);
		play_shot();

		gest_terrain(LOAD, (char *)"terrain.obj");

		skybox.load("sky.obj");
		loaded = true;
	}


	/* xzajic16 *********************************************************************************************/
	//Setup pro 2D vykreslovani
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, W_WIDTH, W_HEIGHT, 0.0, -1.0, 10.0);


	glClear(GL_DEPTH_BUFFER_BIT);

	//Vykreslovavi inventare a stamina baru ve 2D
	if(drawInv) drawInv();
	if(drawStam) drawStam();
	
	//Uklizeni po 2d vykreslovani
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	/* xzajic16 *********************************************************************************************/


	camera = place_cam(event);
	gest_terrain(RETURN).disp();

	/* xzajic16 *********************************************************************************************/
	//Vykreslovani itemu v hraci plose
	glPushMatrix();
	glTranslatef(170.0f, 170.0f, 1.0f);
	glScaled(5.0f, 5.0f, 5.0f);
	printcube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250.0f, 350.0f, 1.0f);
	glScaled(5.0f, 5.0f, 5.0f);
	printcube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(400.0f, 150.0f, 1.0f);
	glScaled(5.0f, 5.0f, 5.0f);
	printcube(3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(350.0f, 350.0f, 1.0f);
	glScaled(5.0f, 5.0f, 5.0f);
	printcube(4);
	glPopMatrix();
	/* xzajic16 *********************************************************************************************/

	glPushMatrix();
	disp_fling(fling, camera);
	glPopMatrix();

	glPushMatrix();
	skybox.disp();
	glPopMatrix();

	//gest_robot(0, camera.origine);
	glFlush();
	SDL_GL_SwapWindow(screen);
}

double get_height(double x, double y, bool interpolated)
{
	static SDL_Surface *heightmap = NULL;
	static bool loaded = false;
	static double tailleZ, tailleX, tailleY;
	double hauteur = 0;
	double minimum = 0;

	int ix = 0, iy = 0;
	double dx = 0, dy = 0;

	Uint32 pxColor1 = 0, pxColor2 = 0, pxColor3 = 0, pxColor4;

	if (!loaded)
	{
		heightmap = IMG_Load("heightmap.png");
		tailleZ = gest_terrain(RETURN).relBoundingBox.coordMax.z - gest_terrain(RETURN).relBoundingBox.coordMin.z;
		/* On part du principe que le terrain commence a 0;0 , donc que la taille X et Y sont max X et max Y*/
		tailleX = gest_terrain(RETURN).relBoundingBox.coordMax.x;
		tailleY = gest_terrain(RETURN).relBoundingBox.coordMax.y;

		loaded = true;
	}

	else//chargement deja fait
	{

		if (x >= 0 && x <= tailleX && y >= 0 && y <= tailleY)//position camera dans le terrain
		{
			x = (x / tailleX) * heightmap->w;           // on cherche la position relative par rapport au terrain, que l'on multiplie par la taille de
			y = (1 - (y / tailleY)) * heightmap->h;     // la heightmap ensuite
														// (1 - .. ) permet de faire une inversion verticale de la heightmap car blender l'inverse

			if (interpolated)//s'il faut interpoler la hauteur en fct des autres sommets, ce qui n'est pas toujours le cas
			{
				/* Pour l'interpolation bilineaire qui permet de connaitre la hauteur sans avoir de pas dus a la heightmap, on procede comme suit :
				-on recupere les quatres pixels qui encadrent la position, et leur valeur en hauteur
				-on recupere les coordonnees entieres de la position, et leurs restes decimaux
				-on applique la formule d'interpolation bilineaire :
				hauteur = ((1-dx) * pxColor1 + dx * pxColor2) * (1-dy) + ((1-dx) * pxColor3 + dx * pxColor4) * dy*/

				ix = (int)x;//position entiere
				iy = (int)y;
				dx = x - ix;//position decimale relative
				dy = y - iy;


				SDL_LockSurface(heightmap);

				/* Pour l'interpolation lineaire de la position, on recupere la position des quatres pixels autours de notre position
				on travaille ac un img en nb, donc les 4 couleurs ont la meme valeur, donc on ne prend qu'une d'entres elles, soit le 1er octet*/
				pxColor1 = getpixel(heightmap, ix, iy);//recup des couleurs
				pxColor2 = getpixel(heightmap, ix + 1, iy);
				pxColor3 = getpixel(heightmap, ix, iy + 1);
				pxColor4 = getpixel(heightmap, ix + 1, iy + 1);
				//recup du 1er octet
				pxColor1 = (*(unsigned char *)(&pxColor1));
				pxColor2 = (*(unsigned char *)(&pxColor2));
				pxColor3 = (*(unsigned char *)(&pxColor3));
				pxColor4 = (*(unsigned char *)(&pxColor4));

				/* On empeche l'interpolation lineaire de nous faire des siennes, c'est a dire de faire monter en trop haut. Pour cela, on definit
				une limite de hauteur, et si elle est depassee par la hauteur d'un des 4 pts, on retourne la valeur minimale des autres.
				Cela pourra eventuellement amener a certains bugs graphiques, a regler plus tard si besoin. */
				//on recupere le minimum
				minimum = pxColor1*tailleZ / 255;
				if (pxColor2*tailleZ / 255 < minimum)//peu etre un peu basic comme methode, mais simple et fonctionnelle
					minimum = pxColor2*tailleZ / 255;
				if (pxColor3*tailleZ / 255 < minimum)
					minimum = pxColor3*tailleZ / 255;
				if (pxColor4*tailleZ / 255 < minimum)
					minimum = pxColor4*tailleZ / 255;
				//maintenant, on verifie que la hauteur max depasse pas la limite
				if (pxColor1*tailleZ / 255 > minimum + .5 ||
					pxColor2*tailleZ / 255 > minimum + .5 ||
					pxColor3*tailleZ / 255 > minimum + .5 ||
					pxColor4*tailleZ / 255 > minimum + .5)
				{
					hauteur = minimum*tailleZ / 255;
				}
				else//ok pr l'interpolation lineaire, les pts ne sont pas trop ecartes en hauteur
				{
					hauteur = ((1 - dx) * pxColor1 + dx * pxColor2) * (1 - dy) + ((1 - dx) * pxColor3 + dx * pxColor4) * dy;//calcul de la hauteur interpolee
				}
			}
			else
			{
				/* Il est possible qu'il ne faille pas interpoler, par exemple pour tester la hauteur pour voir si un mvt est possible ou pas. */
				pxColor1 = getpixel(heightmap, (int)x, (int)y);
				pxColor1 = (*(unsigned char *)(&pxColor1));//recup du 1er octet
				hauteur = (double)pxColor1;
			}

			hauteur *= tailleZ / 255;//on adapte la hauteur retournee a la hauteur de la map

			return hauteur + gest_terrain(RETURN).relBoundingBox.coordMin.z;
		}

	}
	return 0;
}


Vector place_cam(SDL_Event event)
{


	//IPA-TODO - v teot casti se resi ovladani postavy
	static Point position = { 0,0,0 };
	Point positionVoulue = { 60,62,0 };
	static Point oldPosition = { 60,62,0 }; //sauvgarde temporaire de l'ancienne position au cas d'un mvt refuse(hauteur trop haute) //dočasné uložení staré pozice v případě odmítnutí mvt (příliš vysoká výška)
	static Point aimAt = { 0,0,0 };
	static int mouseX = 0, mouseY = 0;
	static int oldMouseX = 0, oldMouseY = 0, newMouseX = 0, newMouseY = 0;
	static double angleY = 0, angleZ = 15;//on ne n'oriente la camera que sur l'axe X et Y (par rapport a la cam.) sinon, cela inclinerait l'img //orientujeme fotoaparát pouze na ose X a Y (relativně k fotoaparátu), jinak by se naklonil img
	static double zPosition = 0.0;
	int tpsEcoule = 0;//temps ecoule depuis le dernier appel de la fonction //čas od posledního volání funkce
	static double jumpCoef = 36.0;


	SDL_GetMouseState(&newMouseX, &newMouseY);

	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	positionVoulue = oldPosition;

	SDL_GetRelativeMouseState(&newMouseX, &newMouseY);
	angleY += -newMouseY;
	angleZ += -newMouseX;

	if (angleY >= 80)
		angleY = 80;
	if (angleY <= -80)
		angleY = -80;
	/* xtrubk00 *********************************************************************************************/
	//reseni pohybu
	fullMovement(keystates, &positionVoulue, angleZ, angleY);
	/* xtrubk00 *********************************************************************************************/

	Point min, max;
	min = gest_terrain(RETURN).relBoundingBox.coordMin;
	max = gest_terrain(RETURN).relBoundingBox.coordMax;

	if (positionVoulue.x > min.x + .5&& positionVoulue.y > min.y + .5&& positionVoulue.y < max.y - .5 && positionVoulue.x < max.x - .5)//dans le terrain
		position = positionVoulue;

	// exit game on ESC
	if (keystates[SDL_SCANCODE_ESCAPE])
		ExitProcess(0);

	/* xzajic16 *********************************************************************************************/
	//detekce kolize s itemem - nastaveni flagu na zvednuti itemu
	itemDetection(&position);
	/* xzajic16 *********************************************************************************************/

	/* xtrubk00 *********************************************************************************************/
	//reseni skoku
	jumpMovement(&position, &zPosition, jumpCoef, &jumpTime, &isJump, keystates, get_height(position.x, position.y, false));

	//natoceni kamery
	aimAtPosition(&aimAt, &position, angleY, angleZ);
	/* xtrubk00 *********************************************************************************************/

	gluLookAt(position.x, position.y, position.z, aimAt.x, aimAt.y, aimAt.z, 0, 0, 1);

	oldMouseX = newMouseX;
	oldMouseY = newMouseY;
	oldPosition = position;

	Vector camera; //vecteur associe a la camera
	camera.origine = position;
	camera.angleY = angleY;
	camera.angleZ = angleZ;
	return camera;
}

objet gest_terrain(Act_Terrain action, char filename[])
{
	static objet objdefaut;
	if (action == LOAD)
		objdefaut.load(filename);

	return objdefaut;
}


void disp_fling(objet fling, Vector camera)
{
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);

	/* Petite animation du fling : mvt de droite a gauche */
	static double decalageY;    //decalage du fling par rapport a la camera
	static double recul = 0;
	static bool croissant = true;      // si le decalage augmente ou diminue

	if (keystates[SDL_SCANCODE_W] ||
		keystates[SDL_SCANCODE_S] ||
		keystates[SDL_SCANCODE_A] ||
		keystates[SDL_SCANCODE_D])
	{
		if (croissant)
		{
			decalageY += .002;
			if (decalageY >= .07)
				croissant = false;
		}
		else
		{
			decalageY -= .002;
			if (decalageY <= 0)
				croissant = true;
		}
	}
	else
	{
		if (decalageY >= 0)
			decalageY -= .002;
	}

	/* xzajic16 *********************************************************************************************/
	//Pridani podminky obsazeni itemu v inventari pro povoleni strelby
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1) && recul <= 0 && getActiveCount() > 0)// click gauche de souris
	{
		recul = .08;
		//shot(camera);
		//Odeber itemy z inventare
		removeItem(2);
	}
	/* xzajic16 *********************************************************************************************/
	else
	{
		if (recul >= 0)
			recul -= .004;
	}

	glPushMatrix();
	glTranslated(camera.origine.x, camera.origine.y, camera.origine.z);
	glRotated(camera.angleZ, 0, 0, 1);
	glRotated(-camera.angleY, 0, 1, 0);
	glTranslated(.4 - recul, -.16 + decalageY, -.25 + (-16 * decalageY*decalageY) + .6*decalageY);

	glScaled(.1, .1, .1);
	fling.disp();
	glPopMatrix();
}

robot *gest_robot(int r, Point posPlayer)
{
	static robot robots[ROBOTS];
	static bool loaded = false;
	if (!loaded)
	{
		loaded = true;
		robots[0].shape.load("robot.obj");
		for (int i = 0; i<ROBOTS; i++)
		{
			robots[i].shape = robots[0].shape;
			robots[i].position = randSpawn();
		}
	}

	Point pos;
	bool collision;
	double dist;
	if (posPlayer.x != -1)//si la position du joueur a été donnée en arguments, x != -1
	{
		for (int i = 0; i<ROBOTS; i++)
		{
			collision = false;
			pos = robots[i].position;
			pos.x += -((pos.x - posPlayer.x) / (fabs(pos.x - posPlayer.x) + 1)) / 30;
			pos.y += -((pos.y - posPlayer.y) / (fabs(pos.y - posPlayer.y) + 1)) / 30;

			for (int a = 0; a<i; a++)//detection de collision entre robots
			{

				dist = sqrt((robots[a].position.x - robots[i].position.x)*
					(robots[a].position.x - robots[i].position.x) +
					(robots[a].position.y - robots[i].position.y)*
					(robots[a].position.y - robots[i].position.y));
				if (dist<3)
					collision = true;

			}
			if (collision != true)
				robots[i].position = pos;
		}

		for (int i = 0; i<ROBOTS; i++)
		{
			robots[i].position.z = get_height(robots[i].position.x, robots[i].position.y);
			glPushMatrix();
			glTranslated(robots[i].position.x, robots[i].position.y, robots[i].position.z+10);
			glRotated(robots[i].z, 0, 0, 1);
			robots[i].z++;
			robots[i].shape.disp();

			glPopMatrix();
		}
	}

	return &(robots[r]);
}



void shot(Vector camera)
{
	/* Pour savoir si la balle a touché un des robots, on effectue un test point par point. Il serait
	possible de le faire en direct, en calculant la trajectoire de la balle, mais dans ce cas, on ne pourrait
	pas savoir si la balle traverse le sol préalablement. Pour eviter de calculer deux points qui pourraient etre de part et d'autre
	d'un robot, on prend des petits intervales. */

	Point test; // point en cours de test
	test = camera.origine;
	robot *bot;
	double dist;


	for (int i = 0; i<500; i++)    //500 points
	{
		test.x += cos(camera.angleZ * (M_PI / 180)) * cos(camera.angleY * (M_PI / 180)) * PITCH;
		test.y += sin(camera.angleZ * (M_PI / 180)) * cos(camera.angleY * (M_PI / 180)) * PITCH;
		test.z += sin(camera.angleY * (M_PI / 180)) * PITCH;


		if (test.z<get_height(test.x, test.y, true))//balle dans le sol
			i = 500;//on arrete la boucle, si la balle passe dans le sol, elle s'arrete, donc la boucle aussi

					/* Maintenant, la boucle qui permet de tester si le point est dans un robot */
	for (int j = 0; j<5; j++)
		{
			bot = gest_robot(j);
			dist = sqrt((bot->position.x - test.x)*(bot->position.x - test.x) +
				(bot->position.y - test.y)*(bot->position.y - test.y));

			if (dist<1 && test.z>(bot->shape.relBoundingBox.coordMin.z + bot->position.z) && test.z<(bot->shape.relBoundingBox.coordMax.z + bot->position.z))//le point est dans le robot
			{
				gest_robot(j)->kill();
				j = 5;
				i = 500;
			}
		}
 	}
	play_shot();
}

void painter(objet fling)
{
	gluLookAt(0, 0, 0, 1, 0, 0, 0, 0, 1);
	glPushMatrix();
	glTranslated(1, 0, 0);
	glScaled(.15, .15, .15);
	glRotated(120, 0, 0, 1);
	glRotated(20, -1, 1, 0);
	fling.disp();
	glPopMatrix();
	glFlush();
	SDL_GL_SwapWindow(screen);
}

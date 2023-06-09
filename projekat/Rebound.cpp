/*
 * Rebound.cpp
 *
 * KONTROLE
 * Crveni igrac: [A,D]-> pomeranje
 * Plavi igrac: [LEFTARROW, RIGHTARROW]-> pomeranje
 * r/R-> reset trenutne partije
 * [ESC] -> Nazad na glavni meni
 */

#include <stdlib.h>
#include <stdio.h>
#include "glut.h"	
#include "Rebound.h"
#include <math.h>
#include "Player.h"
#include <string>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "irrklang/includes/irrKlang.h"
using namespace irrklang;
ISoundEngine* SoundEngine = createIrrKlangDevice();
ISoundEngine* SoundEngine2 = createIrrKlangDevice();

const double Xmin = 0.0, Xmax = 7.0;
const double Ymin = 0.0, Ymax = 5.0;

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[256];
float xj = Xmax/4;
//float xd = 1.8;
float yj = 0.1;

float x11 = 3*Xmax/4;
//float x22 = 3.2;
float y11 = 0.1;


float l = 0.8;
float h = 0.1;
float factor = 0.0005;

float mp = 0.000075;
float netHeight = 0.8;
float r = 0.075;
float xBall = 3 * Xmax / 4;
float yBall = Ymax/2;
float yFactor = -mp;
float xFactor = 0.0000;
float gravity = -9.81;
float dt = mp;
float colorShift = 0.5;

bool play = false;
bool run = true;
bool who = false;
bool isNet = false;
bool hero = false;
bool villain = false;
bool pt1 = false;
bool pt2 = false;
bool reset = false;
bool filter = false;
bool menuMusic = false;

int howmuch = 0;
int init = false;
int pts1, pts2;
int wincond=11;
int sec = 4;


char buf[100] = { 0 };

Player p1(xj,yj,l,h,false);
Player p2(x11, y11, l, h,true);
void renderBitmap(float x, float y, void* font, char* string) {
	char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

float DegToRadians(float ang) {

	return ang * (3.14159265358979 / 180);
}

void Initialization() {
	for (int i = 0; i < 256; i++) {
		keyStates[i] = false;
		keySpecialStates[i] = false;
	}
	init = true;
	pts1 = 0;
	pts2 = 0;
}

void displayTimer() {

}

void resetTerrain() {
	float xT = xBall;
	float yT = yBall;
	xBall = Xmax / 2;
	yBall = Ymax / 2;
	yFactor = -mp;
	int a = rand() % 2;
	if (pt1) {
			xFactor = -mp * (1 + (rand() % 5));
	}
	else {
		if(!isNet)
			xFactor = mp * (1 + (rand() % 5));
	}
	isNet = false;
	//pt1 = false;
	//pt2 = false;
	//xFactor = -mp*abs(-a);
	hero = true;
}




void keyPressed(unsigned char key, int x, int y) 
{
	keyStates[key] = true; 
}


void keyUp(unsigned char key, int x, int y) 
{
	keyStates[key] = false; 
}

void menuKeyOperations(void) {
	if(keyStates[13]){
		play = true;
		init = false;
		pts1 = 0;
		pts2 = 0;
		p1.setLength(l);
		p2.setLength(l);
		xBall = 3 * Xmax / 4;
		yBall = Ymax / 2;
		yFactor = -mp;
		xFactor = 0.0000;
		glutDisplayFunc(drawScene);

	}
	if (keyStates['p']) {
		
		wincond = (wincond==11) ? 16 : 11;
		keyStates['p'] = false;

	}
	if (keyStates[27]) {
		glutDestroyWindow(glutGetWindow());
		keyStates[27] = false;
		exit(0);
	}
}

void keyOperations(void) 
{
	if (keyStates['a']) 
	{ 
		//run = true;
		if(run)
			p1.setX(p1.getX1() - factor);
		//printf("1L");
	}
	if (keyStates['d']) 
	{
		//run = true;
		if (run)
			p1.setX(p1.getX1() + factor);
		//printf("1D");
	}
	if (keyStates['r'])
	{
		xBall = 3 * Xmax / 4;
		yBall = Ymax/2;
		yFactor = -mp;
		xFactor = 0.0000;
		//run = true;
		pts1 = 0;
		pts2 = 0;
		reset = true;
		resetTerrain();
	}
	if (keyStates[' ']) {
		run = !run;
		keyStates[' '] = false;
	}
	if (keyStates[27]) {
		pts1 = 0;
		pts2 = 0;
		filter = true;
		SoundEngine2->stopAllSounds();
		menuMusic = false;
		keyStates[27] = false;
	}
	if (keyStates['h']) {
		pts1 = 10;
	}
}

void keySpecial(int key, int x, int y) 
{
	keySpecialStates[key] = true;
}

void keySpecialUp(int key, int x, int y) 
{
	keySpecialStates[key] = false;
}

void menuSpecialKeyOperations() {
	if (keySpecialStates[GLUT_KEY_DOWN])
	{
		if(netHeight>0.4)
			netHeight -= 0.0005;
		//printf("2L");
	}
	if (keySpecialStates[GLUT_KEY_UP])
	{
		if(netHeight<1.2)
			netHeight += 0.0005;
	}
	if (keySpecialStates[GLUT_KEY_LEFT])
	{
		if (l > 0.4)
			l -= 0.0005;
		//printf("2L");
	}
	if (keySpecialStates[GLUT_KEY_RIGHT])
	{
		if (l < 1.0)
			l += 0.0005;
	}
}

void keySpecialOperations(void) {
	if (keySpecialStates[GLUT_KEY_LEFT])
	{  
		//run = true;
		if (run)
			p2.setX(p2.getX1() - factor);
		//printf("2L");
	}
	if (keySpecialStates[GLUT_KEY_RIGHT])
	{ 
		//run = true;
		if (run)
			p2.setX(p2.getX1() + factor);
		//printf("2R");
	}
}

void drawNet() {
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0,1.0,1.0);
	glVertex2f(Xmax/2-0.05, 0.0);
	glVertex2f(Xmax / 2 + 0.05, 0.0);
	glVertex2f(Xmax / 2 + 0.05, netHeight);
	glVertex2f(Xmax / 2 - 0.05, netHeight);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(1.0,1.0,1.0);
	glVertex2f(Xmax / 2 - 0.05,netHeight);
	float temp = netHeight;
	float tempX = Xmax / 2 + 0.05;
	while (temp > 0.0) {
		glVertex2f(tempX, temp - 0.1);
		temp = temp - 0.1;
		if (tempX > Xmax / 2)
			tempX = Xmax / 2 - 0.05;
		else
			tempX = Xmax / 2 + 0.05;
		
	}
	glEnd();


	glBegin(GL_LINE_STRIP);
	glColor3f(1.0,1.0,1.0);
	glVertex2f(Xmax / 2 + 0.05, netHeight);
	temp = netHeight;
	tempX = Xmax / 2 - 0.05;
	while (temp > 0.0) {
		glVertex2f(tempX, temp - 0.1);
		temp = temp - 0.1;
		if (tempX > Xmax / 2)
			tempX = Xmax / 2 - 0.05;
		else
			tempX = Xmax / 2 + 0.05;

	}
	glEnd();
}

void drawBall() {

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 0.0);
	float dPI = 2.0 * 3.1415926535;
	int n = 30;
	for (int i = 0; i < n; i++) {
		glVertex2f(
			(xBall + (r * cos(i * dPI / n))), (yBall + (r * sin(i * dPI / n))));
	}
	glEnd();
}

void drawPlayer(Player p) {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	if (p.Side()) {
		glColor3f(0.0, 0.0, 1.0);
	}
	p.setLength(l);
	glVertex2f(p.getX1(), p.getY1());
	glVertex2f(p.getX2(), p.getY1());
	glVertex2f(p.getX2(), p.getY2());
	glVertex2f(p.getX1(), p.getY2());
	glEnd();
}





void paddleCollision(Player p) {
	if (yBall < p.getY2()) {
		if (p.Side()) {
			if (xBall > p.getX2() && xBall < p.getX1()) {
				SoundEngine->play2D("audio/solid.wav");
				float ang = 45 + 45 * (p.getX1()-xBall);

				yFactor = sin(DegToRadians(ang))*mp*6;
				xFactor = -cos(DegToRadians(ang)) * mp*6;
				if (who) {
					howmuch = 0;
					who = false;

				}
				howmuch += 1;
				if (howmuch == 4) {
					resetTerrain();
					pts1 += 1;
					pt1 = true;
				}
			}
		}
		else {
			if (xBall > p.getX1() && xBall < p.getX2()) {
				SoundEngine->play2D("audio/solid.wav");
				float ang = 45 + 45 * (xBall- p.getX1());

				yFactor = sin(DegToRadians(ang)) * mp * 6;
				xFactor = cos(DegToRadians(ang)) * mp * 6;

				if (!who) {
					howmuch = 0;
					who = true;
				}
				howmuch += 1;
				if (howmuch == 4) {
					resetTerrain();
					pts2 += 1;
					pt2 = true;
				}
				
			}
		}
		}
}

void netCollision() {

	if (((xBall >= Xmax / 2 - 0.05) && (xBall <= Xmax / 2 + 0.05)) && ((yBall >= 0.0) && (yBall <= netHeight))) {
		SoundEngine->play2D("audio/powerup.wav");
		if (who) {
			pts2 += 1;
			pt2 = true;
		}
		else {
			pts1 += 1;
			pt1 = true;
		}
		isNet = true;
		resetTerrain();
	}

}
void groundCollision() {
	if (yBall < 0.0) {
		SoundEngine->play2D("audio/powerup.wav");
		if (who) {
			if (xBall <= Xmax / 2) {
				pts2 += 1;
				pt2 = true;
			}
			else {
				pts1 += 1;
				pt1 = true;
			}
		}
		else {
			if (xBall >= Xmax / 2) {
				pts1 += 1;
				pt1 = true;
			}
			else {
				pts2 += 1;
				pt2 = true;
			}
		}
		resetTerrain();
	}
	if (xBall < 0.0 || xBall>7.0) {
		if (who) {
			pts1 += 1;
			pt1 = true;
		}
		else {
			pts2 += 1;
			pt2 = true;
		}
		resetTerrain();
	}
}

void Writer(float x,float y,std::string what,float offset) {
	std::string sh = what;
	char const* fin = sh.c_str();
	sprintf_s(buf, fin);
	renderBitmap(x-offset,y, GLUT_BITMAP_TIMES_ROMAN_24, buf);
}


void drawSideArrows(void) {


	glBegin(GL_LINE_LOOP);
	glVertex2f(2.0, 2.8);
	glVertex2f(2.0, 3.2);
	glVertex2f(2.4, 3.2);
	glVertex2f(2.4, 2.8);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(2.05, 2.98);
	glVertex2f(2.35, 2.98);
	glVertex2f(2.35, 3.02);
	glVertex2f(2.05, 3.02);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(2.2, 3.1);
	glVertex2f(2.18, 3.08);
	glVertex2f(2.33, 3.00);
	glVertex2f(2.35, 3.02);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(2.2, 2.9);
	glVertex2f(2.22, 2.88);
	glVertex2f(2.35, 2.98);
	glVertex2f(2.33, 3.00);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(1.2, 2.8);
	glVertex2f(1.2, 3.2);
	glVertex2f(1.6, 3.2);
	glVertex2f(1.6, 2.8);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.25, 2.98);
	glVertex2f(1.55, 2.98);
	glVertex2f(1.55, 3.02);
	glVertex2f(1.25, 3.02);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.4, 3.1);
	glVertex2f(1.38, 3.12);
	glVertex2f(1.25, 3.02);
	glVertex2f(1.27, 3.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.4, 2.9);
	glVertex2f(1.38, 2.88);
	glVertex2f(1.25, 2.98);
	glVertex2f(1.27, 3.00);
	glEnd();
}

void drawMainArrows(void) {

	glBegin(GL_LINE_LOOP);
	glVertex2f(1.2, 2.0);
	glVertex2f(1.2, 2.4);
	glVertex2f(1.6, 2.4);
	glVertex2f(1.6, 2.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.38, 2.05);
	glVertex2f(1.42, 2.05);
	glVertex2f(1.42, 2.35);
	glVertex2f(1.38, 2.35);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90, 0.0,0.0,1.0);
	glTranslatef(0.8,-4.4,0.0);
	

	glBegin(GL_LINE_LOOP);
	glVertex2f(1.2, 2.8);
	glVertex2f(1.2, 3.2);
	glVertex2f(1.6, 3.2);
	glVertex2f(1.6, 2.8);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.25, 2.98);
	glVertex2f(1.55, 2.98);
	glVertex2f(1.55, 3.02);
	glVertex2f(1.25, 3.02);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.4, 3.1);
	glVertex2f(1.38, 3.12);
	glVertex2f(1.25, 3.02);
	glVertex2f(1.27, 3.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.4, 2.9);
	glVertex2f(1.38, 2.88);
	glVertex2f(1.25, 2.98);
	glVertex2f(1.27, 3.00);
	glEnd();

	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(270, 0.0, 0.0, 1.0);
	glTranslatef(-3.6, -0.8, 0.0);


	glBegin(GL_LINE_LOOP);
	glVertex2f(1.2, 2.8);
	glVertex2f(1.2, 3.2);
	glVertex2f(1.6, 3.2);
	glVertex2f(1.6, 2.8);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.25, 2.98);
	glVertex2f(1.55, 2.98);
	glVertex2f(1.55, 3.02);
	glVertex2f(1.25, 3.02);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.4, 3.1);
	glVertex2f(1.38, 3.12);
	glVertex2f(1.25, 3.02);
	glVertex2f(1.27, 3.0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(1.4, 2.9);
	glVertex2f(1.38, 2.88);
	glVertex2f(1.25, 2.98);
	glVertex2f(1.27, 3.00);
	glEnd();

	glPopMatrix();

}

void drawP(void) {
	glBegin(GL_LINE_LOOP);
	glVertex2f(1.2, 1.2);
	glVertex2f(1.2, 1.6);
	glVertex2f(1.6, 1.6);
	glVertex2f(1.6, 1.2);
	glEnd();
	Writer(1.33,1.33,"P",0.0);
}
void drawEnter(void) {
	glBegin(GL_LINE_LOOP);
	glVertex2f(1.2, 3.6);
	glVertex2f(1.2, 4.0);
	glVertex2f(1.0, 4.0);
	glVertex2f(1.0, 4.4);
	glVertex2f(1.2, 4.4);
	glVertex2f(1.8, 4.4);
	glVertex2f(1.8, 3.6);
	glEnd();
	Writer(1.2, 4.1, "Enter", 0.0);
}
void drawEsc(void) {
	glBegin(GL_LINE_LOOP);
	glVertex2f(2.0, 3.6);
	glVertex2f(2.0, 4.0);
	glVertex2f(2.4, 4.0);
	glVertex2f(2.4, 3.6);
	glEnd();
	Writer(2.0, 3.7, "Esc", 0.0);
}

void drawMenu(void) {
	if (!menuMusic) {
		SoundEngine2->setSoundVolume(0.1);
		SoundEngine2->play2D("audio/MF-W-90.XM", true);
		menuMusic = true;
	}
	xj = Xmax / 4;
	yj = 0.1;

	x11 = 3 * Xmax / 4;
	y11 = 0.1;
	p1.setX(xj);
	p2.setX(x11);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (!init)
		Initialization();
	menuKeyOperations();
	menuSpecialKeyOperations();
	drawEnter();
	Writer(3.5,3.7,"Pocetak igre/Izlaz iz igre",0.6);

	drawMainArrows();
	drawSideArrows();
	drawP();
	drawEsc();
	drawPlayer(p1);
	drawPlayer(p2);
	glColor3f(1.0, 1.0, 1.0);
	Writer(3.5, 2.9, "Sirina igraca", 0.6);
	Writer(3.5, 2.1, "Visina mreze", 0.6);
	Writer(3.5, 1.3, "Prvi do ", 0.6);
	glColor3f(1.0, 1.0, 0.0);
	Writer(3.5, 1.3, std::to_string(wincond),-0.15);
	glColor3f(1.0, 1.0, 1.0);
	drawNet();
	colorShift += 0.0003;
	if (colorShift >= 1.0)
		colorShift = 0.5;
	glColor3f(colorShift, 0.2, 0.2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(4.0, 4.6);
	glVertex2f(4.0, 5.0);
	glVertex2f(2.9, 5.0);
	glVertex2f(2.9, 4.6);
	glEnd();
	Writer(3.0, 4.8, "Rebound", 0.0);
	glColor3f(1.0, 0.2, 0.2);
	Writer(5.5, 4.8, "Djordje Karisic", 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutPostRedisplay();
	glEnd();
	// Flush the pipeline, swap the buffers
	glFlush();
	glutSwapBuffers();

}

void drawScene(void)
{
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	if (!init)
		Initialization();

	keyOperations();
	keySpecialOperations();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	drawPlayer(p1);
	drawPlayer(p2);
	drawNet();
	drawBall();

	if (run) {
		xBall += xFactor;
		yBall += yFactor;
		yFactor -= 9.8 * dt * mp;
		paddleCollision(p1);
		paddleCollision(p2);
		groundCollision();
		netCollision();
	}

	glColor3f(1.0, 1.0, 1.0);
	char buf[100] = { 0 };
	std::string res = "Pauzirano";
	float offset = 0.35;
	std::string sh;
	if (hero && !villain && run) {
		char buf2[100] = { 0 };
		if (reset) {
			sh = "Restartovana partija";
		}
		else {
			sh = "Poen ";
			sh += (pt1) ? "za crvenog igraca!" : "za plavog igraca!";
		}
		Writer(2.5, 2.0, sh, 0.0);
		pt1 = false;
		pt2 = false;
	}
	else if (villain && run) {
		std::string sh;
		glColor3f(1.0, 1.0, 1.0);
		sec -= 1;
			sh = std::to_string(sec);
			Writer(3.45, 2.0, sh, 0.0);
			if (!sec) {
				sec = 4;
				hero = false;
			}
			Sleep(1000);
	}
	if (pts1 == wincond || pts2 == wincond) {
		//SoundEngine->play2D("audio/explosion.wav");
		offset = 1.0;
		res = (pts1 >pts2) ? "Crveni igrac je pobednik" : "Plavi igrac je pobednik";
		run = false;
		menuMusic = false;
	}
	else if (run) {
		res = std::to_string(pts1) + " : " + std::to_string(pts2);
		offset = 0.0;
	}
	Writer(3.35,3.0,res,offset);


	glutPostRedisplay();
	glEnd();
	// Flush the pipeline, swap the buffers
	glFlush();
	glutSwapBuffers();

	if (hero) {
		villain = true;
	}
	else {
		villain = false;
	}
	reset = false;

	if (filter) {
		glutDisplayFunc(drawMenu);
		filter = false;
	}

	// proveriti da li loptica udata u neku od cigli tako sto ce postojati niz koordinara svih cigli
}

void initRendering()
{

}

void resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	glViewport(0, 0, w, h);

	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;
	if ((Xmax - Xmin) / w < (Ymax - Ymin) / h) {
		scale = ((Ymax - Ymin) / h) / ((Xmax - Xmin) / w);
		center = (Xmax + Xmin) / 2;
		windowXmin = center - (center - Xmin) * scale;
		windowXmax = center + (Xmax - center) * scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax - Xmin) / w) / ((Ymax - Ymin) / h);
		center = (Ymax + Ymin) / 2;
		windowYmin = center - (center - Ymin) * scale;
		windowYmax = center + (Ymax - center) * scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(windowXmin, windowXmax, windowYmin, windowYmax, -1, 1);

}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(10, 60);
	glutInitWindowSize(700,500);

	//preimenovati u Kolokvijum_ime_prezime (npr. Kolokvijum_Tijana_Sustersic)
	glutCreateWindow("Rebound_Djordje_Karisic");

	initRendering();

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp); 

	glutSpecialFunc(keySpecial);  
	glutSpecialUpFunc(keySpecialUp); 


	glutReshapeFunc(resizeWindow);
	printf("%d", play);
	glutDisplayFunc(drawMenu);

	glutMainLoop();

	return(0);
}
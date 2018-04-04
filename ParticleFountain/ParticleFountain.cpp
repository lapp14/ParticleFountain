/*
 *	ParticleFountain
 *		ParticleFountain.cpp
 *
 *	Cosc 3P98 Assignment 3
 *	Dec 5, 2013
 *	Dan Lapp, 4276804
 */
#include "ParticleFountain.h"

/*	
 *	This draws a particle as a cube. Updates all required parameters for each		
 *	frame redraw.
 *
 *	@param particle a pointer to the particle to be drawn
 *	@returns true if the particle should be killed for whatever reason, like becoming
 *		stationary or going outside bounds. False if the particle is valid.
 */
bool drawCube(Particle *particle)
{
	//if particle exceeds lifecycle, mark for deletion. Deletes all particles at same time for fireworks mode
	if(mode.fireworksMode && particle->lifeCycle > MAX_FIREWORKS_LIFECYCLE)
		return true;

	//if particle isnt moving, mark for deletion
	if(particle->speed <= 0.00001)
		return true;

	//if the object is outside the boundaries, mark for deletion
	if(particle->position[0] < -GROUND_SIZE || particle->position[0] > GROUND_SIZE
		|| particle->position[2] < -GROUND_SIZE || particle->position[2] > GROUND_SIZE
		|| particle->position[1] > MAX_CLIPPING)
			return true;

	//if the object hits the ground make it bounce
	if(particle->position[1] <= 0.0 + (particle->scaleFactor/2))
	{
		particle->direction[1] *= -1.0;
		particle->direction[1] += 2.0;
		particle->direction[1] *= global.friction;

		if(mode.useFriction)
			particle->speed *= global.friction;
	}
	
	particle->direction[1] -= global.gravity; //adjust y direction each iteration

	//update new position of object
	particle->position[0] += particle->speed * particle->direction[0];
	particle->position[1] += particle->speed * particle->direction[1];

	//apply transformations and draw
	glPushMatrix();
	glRotatef( static_cast<GLfloat>(particle->rotationAngle), 0, 1, 0);

	if(!mode.vertexMode) //translate only if not in GL_POINTS mode
		glTranslatef(particle->position[0], particle->position[1], particle->position[2]);

	//if particles are spinning
	if(mode.randomSpin && !mode.vertexMode)
	{
		particle->spin[0] += particle->spinMultiplier[0];
		particle->spin[1] += particle->spinMultiplier[1];
		particle->spin[2] += particle->spinMultiplier[2];

		//handle overflow
		if(particle->spin[0] >= 360)
			particle->spin[0] -= 360;
		
		if(particle->spin[1] >= 360)
			particle->spin[1] -= 360;
		
		if(particle->spin[2] >= 360)
			particle->spin[2] -= 360;

		glRotatef(particle->spin[0], 1.0, 0.0, 0.0);
		glRotatef(particle->spin[1], 0.0, 1.0, 0.0);
		glRotatef(particle->spin[2], 0.0, 0.0, 1.0);
	}
		
	//if random color mode is enabled, draw to the particle's unique color
	if(mode.randomColour)
		glColor3fv(particle->colour);
	else
		glColor3f(0.3f, 0.3f, 0.3f);

	//if fountain is not in GL_POINTS mode draw the cube, otherwise just draw the vertex
	if(!mode.vertexMode)
		glutSolidCube(particle->scaleFactor);
	else
	{
		glBegin(GL_POINTS);
		glVertex3fv(particle->position);
		glEnd();
	}

	glPopMatrix();
	particle->lifeCycle++;

	//particle is valid, should not be deleted
	return false;
}

/*	Draws the environment. Draws the ground as a square plane and the
 *	fountain as a rectangular solid
 */
void drawEnvironment()
{
	/* draw ground */
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor3f(0.3f, 0.9f, 0.5f);
		glVertex3i(GROUND_SIZE, 0,  GROUND_SIZE);
		glColor3f(0.3f, 0.7f, 0.3f);
		glVertex3i(GROUND_SIZE, 0, -GROUND_SIZE);
		glColor3f(0.4f, 0.7f, 0.4f);
		glVertex3i(-GROUND_SIZE, 0, -GROUND_SIZE);	
		glColor3f(0.1f, 0.9f, 0.4f);
		glVertex3i(-GROUND_SIZE, 0, GROUND_SIZE);
	glEnd();

	/*draw fountain */
	glColor3f(0.3f, 0.3f, 0.3f);
	glPushMatrix();
	glTranslatef(0.0f, 15.0f, 0.0f);
	glScalef(1.0f, 3.0f, 1.0f);
	glutSolidCube(10);
	glPopMatrix();
}

/*	Glut display callback method. Called every time the screen refreshes and draws
 *	a new frame. Draws the scene, then decides what cubes to draw based on the
 *	selected fountain mode, and whether or not its running
 */
void display()
{
	glLoadIdentity();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	if(global.rotateFactor != 0.0)
	{
		global.rotateAngle[global.rotateAxis] += global.rotateFactor;

		if(global.rotateAngle[global.rotateAxis] >= 360)
			global.rotateAngle[global.rotateAxis] -= 360;
		if(global.rotateAngle[global.rotateAxis] < 0)
			global.rotateAngle[global.rotateAxis] += 360;
	}
	
	//rotate scene about center
	glRotatef(global.rotateAngle[0], 1, 0, 0);
	glRotatef(global.rotateAngle[1], 0, 1, 0);
	glRotatef(global.rotateAngle[2], 0, 0, 1);
	
	glMaterialf(GL_FRONT, GL_AMBIENT, 1.0);
	glMaterialf(GL_FRONT, GL_DIFFUSE, 1.0);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

	drawEnvironment();

	if(mode.manualFire)
	{
		if(global.particles.size() == 0)
			mode.manualFire = false;
		else
			for(std::size_t i = 0; i < global.particles.size(); i++)
			{
				if(global.particles.at(i).removeParticle)
					global.particles.erase(global.particles.begin() + i);
				else
					global.particles.at(i).removeParticle = drawCube(&global.particles.at(i));
			}
	}

	if(global.runFountain)
	{
		if(mode.fireworksMode)
		{
			if(global.particles.empty())
			{
				for(int i = 0; i < MAX_PARTICLES; i++)
				{
					Particle p;
					p.randomizeParticle(&mode);
					global.particles.push_back(p);	
				}
			}
		}
		else
		{
			if(global.particles.size() < MAX_PARTICLES)
			{
				Particle p;
				p.randomizeParticle(&mode);
				global.particles.push_back(p);	
			}
		}

		for(std::size_t i = 0; i < global.particles.size(); i++)
		{
			if(global.particles.at(i).removeParticle)
				global.particles.erase(global.particles.begin() + i);
			else
				global.particles.at(i).removeParticle = drawCube(&global.particles.at(i));
		}
	}

	glutSwapBuffers();
	glFlush();
}

/*	Glut keyboard callback function for non ASCII keys such as arrow keys
 *
 *	@param key the key pressed
 *	@param x the x position of the mouse (not used)
 *	@param y the y position of the mouse (not used)
 */
void specialKeys(int key, int x, int y)
{
	switch (key)
	{
		/* Fountain rotate keys */
		case GLUT_KEY_LEFT: //left key
			if(global.rotateFactor < 0.5)
				global.rotateFactor += 0.01f;
			break;

		case GLUT_KEY_RIGHT: //right key
			if(global.rotateFactor > -0.5)
				global.rotateFactor -= 0.01f;
			break;

	}
}

/*	Glut keyboard callback function. Called when the user presses a key on the keyboard.
 *
 *	@param key the key pressed
 *	@param x the x position of the mouse (not used)
 *	@param y the y position of the mouse (not used)
 */
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		/* FOUNTAIN FIRE MODES */
		case 32: //space, run continuous 
			if(global.runFountain)
			{
				global.particles.clear();
				global.runFountain = false;
			}
			else
				global.runFountain = true;
		
		case 'f':
		case 'F': //manual fire mode
			if(!global.runFountain)
			{
				mode.manualFire = true;
				Particle p;
				p.randomizeParticle(&mode);
				global.particles.push_back(p);
			}
			break;

		case 'w': //fireworks mode
		case 'W':
			if(mode.fireworksMode)
				mode.fireworksMode = false;
			else
				mode.fireworksMode = true;
			break;

		case 13: //enter, fire 1 particle at a time
			if(!global.runFountain)
			{
				if(global.particles.size() > 0)
					global.particles.clear();

				mode.manualFire = true;
				Particle p;
				p.randomizeParticle(&mode);
				global.particles.push_back(p);	
			}
			break;

		case 'x':
		case 'X':
			global.rotateAxis = 0;
			break;

		case 'y':
		case 'Y':
			global.rotateAxis = 1;
			break;
			
		case 'z':
		case 'Z':
			global.rotateAxis = 2;
			break;
	}
}

/*	Initializes the global variables in the struct to their default values
 */
void initializeGlobalStruct()
{
	global.runFountain = false;
	global.rotateAngle[0] = 0;
	global.rotateAngle[1] = 0;
	global.rotateAngle[2] = 0;
	global.rotateAxis = 1; //set to y axis initially
	global.rotateFactor = 0;
	global.friction = 0.8f;
	global.gravity = 0.013f;
	global.particles.resize(MAX_PARTICLES);
	mode.reset();
}

/*	Initializes the lighting and materials for the scene. Uses ambient, specular
 *	and diffuse lighting. Positions the light source above the fountain, slightly
 *	away from the origin.
 */
void initializeLighting()
{
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);

	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat diffuseLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat specularLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat shininess[] = {50.0f};
	GLfloat position[] = {40.0f, 250.0f, 40.0f, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
		
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

/*	Defines behaviour for the each glut menu entry. The glut menus are bound
 *	to the right mouse button.
 *
 *	@value the value of the selected menu item
 */
void menuChoice(int value)
{
	switch(value)
	{
		case 0:
			exit(0);
			break;

		case -1: //reset
			initializeGlobalStruct();
			global.particles.clear();
			mode.reset();
			break;

		/* FOUNTAIN SETTINGS */
		case 1: //randomize speed
			if(mode.randomSpeed)
				mode.randomSpeed = false;
			else
				mode.randomSpeed = true;
			break;

		case 2: //randomize size
			if(mode.randomSize)
				mode.randomSize = false;
			else
				mode.randomSize = true;
			break;

		case 7: //spin particles
			if(mode.randomSpin)
				mode.randomSpin = false;
			else
				mode.randomSpin = true;
			break;

		case 3: // use friction
			if(mode.useFriction)
				mode.useFriction = false;
			else
				mode.useFriction = true;
			break;

		case 4: //fireworks mode
			if(mode.fireworksMode)
				mode.fireworksMode = false;
			else
				mode.fireworksMode = true;
			break;

		case 5: //spray mode
			if(mode.sprayMode)
				mode.sprayMode = false;
			else
				mode.sprayMode = true;
			break;

		case 9: //random colour
			if(mode.randomColour)
				mode.randomColour = false;
			else
				mode.randomColour = true;
			break;

		case 10: //toggle backface culling
			if(mode.backfaceCulling)
			{
				mode.backfaceCulling = false;
				glDisable(GL_CULL_FACE);
			}
			else
			{
				mode.backfaceCulling = true;
				glEnable(GL_CULL_FACE);
			}
			break;

		/* Polygon mode menu */
		case 20: //polygon mode
			mode.vertexMode = false;
			glPolygonMode(GL_FRONT, GL_FILL);
			break;
		
		case 21: //wireframe mode
			mode.vertexMode = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;

		case 22: //glPoints
			mode.vertexMode = true;
			break;		

		/* Polygon shading menu */
		case 30: //flat shading
			mode.smoothShading = false;
			glShadeModel(GL_FLAT);
			break;

		case 31: //gouraud shading
			mode.smoothShading = true;
			glShadeModel(GL_SMOOTH);
			break;

	}
}

/*	Creates the glut menus. Adds all the glut menu entries and assigns a unique value
 *	to each menu entry. Binds the menu to the right mouse button
 */
void createMenus()
{
	polygonMode = glutCreateMenu(menuChoice);
	glutAddMenuEntry("Polygon Mode", 20);
	glutAddMenuEntry("Wireframe Mode", 21);
	glutAddMenuEntry("Points Mode", 22);

	polygonShading = glutCreateMenu(menuChoice);
	glutAddMenuEntry("Flat", 30);
	glutAddMenuEntry("Gouraud", 31);

	glutCreateMenu(menuChoice);
	glutAddMenuEntry("Random Speed", 1);
	glutAddMenuEntry("Random Size", 2);
	glutAddMenuEntry("Random Spin", 7);
	glutAddMenuEntry("Random Colour", 9);
	glutAddMenuEntry("Use Friction", 3);
	glutAddMenuEntry("Fireworks Mode", 4);
	glutAddMenuEntry("Spray Mode", 5);
	glutAddMenuEntry("Backface Culling", 10);
	glutAddSubMenu("Polygon Shading", polygonShading);
	glutAddSubMenu("Polygon Mode", polygonMode);
	glutAddMenuEntry("Reset", -1);
	glutAddMenuEntry("Quit", 0);

	

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/*	Program entry point. Initializes glut and window. Sets up camera location and scene
 *	perspective. Builds glut mouse menus and sets callback methods.
 *
 *	@returns program exit code
 */
int main(int argc, char* argv[])
{
	initializeGlobalStruct();
	
	srand(unsigned(time(NULL)));

	std::cout << "Particle Fountain\n\n";
	std::cout << "Spacebar: Start/Stop constant stream\n";
	std::cout << "Enter: Fire one particle\n";
	std::cout << "f: Hold for manual fire option\n";
	std::cout << "w: Fireworks mode\n\n";

	std::cout << "Modes:\nRight click to toggle modes on and off\n\n";
	std::cout << "Scene Rotation:\nLeft Arrow: Rotate left\n";
	std::cout << "Right Arrow: Rotate right\n";
	std::cout << "x: Rotate about x axis\n";
	std::cout << "y: Rotate about y axis\n";
	std::cout << "z: Rotate about z axis\n\n";

	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Particle Fountain");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutDisplayFunc(display);
	glutIdleFunc(display); 
	glPointSize(3);

	createMenus();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, WINDOW_WIDTH/WINDOW_HEIGHT, 0.1, MAX_CLIPPING);
	gluLookAt(280, 200, 310, 0, 60, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	initializeLighting();
	glutMainLoop();

	return 0;
}

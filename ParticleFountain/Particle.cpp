/*
 *	ParticleFountain
 *		Particle.c
 *
 *	Cosc 3P98 Assignment 3
 *	Dec 5, 2013
 *	Dan Lapp, 4276804
 */

#include "Particle.h"
#include <iostream>
#include <vector>

/*
 *	The particle class. This class represents an instance of a single particle, and
 *	stores all of the data for that particle.
 */
Particle::Particle()
{
	removeParticle = false;
	lifeCycle = 0;
};

/*
*	Initializes a particle with randomized values to give each particle different
*	behaviour.
*
*	@param mode the program mode passed from ParticleFountain, specifying the selected
*		modes in the fountain
*/
void Particle::randomizeParticle(ProgramMode* mode)
{
	removeParticle = false;

	if((*mode).randomSize)
		//scale between 3 and 5
		scaleFactor = (rand() % 3) + 3;
	else
		scaleFactor = 3;

	//rotation angle between 0 - 359 degrees
	rotationAngle = rand() % 360;

	if((*mode).randomSpeed)
		//speed between 0.002-0.006
		speed = (float) (((rand() % 5) + 2) * 0.001);
	else
		speed = DEFAULT_SPEED;

	//x direction 2-5
	direction[0] = static_cast<float>((rand() % 4) + 3);

	//y direction 18-30
	direction[1] = static_cast<float>((rand() % 13) + 18);

	//set starting position to top of fountain
	position[0] = 0;
	position[1] = 25;
	position[2] = 0;

	/* Random Spin */
	//randomize spin multiplier
	spinMultiplier[0] = static_cast<float>(1.0 - randomDecimal());
	spinMultiplier[1] = static_cast<float>(1.0 - spinMultiplier[0] - randomDecimal());

	if(spinMultiplier[1] < 0.0)
		spinMultiplier[1] = 0.0;

	spinMultiplier[2] = static_cast<float>(1.0 - spinMultiplier[0] - spinMultiplier[1]);

	//set starting spin angles to 0
	spin[0] = 0;
	spin[1] = 0;
	spin[2] = 0;

	// increase direction and speed if in Spray mode
	if((*mode).sprayMode)
	{
		direction[1] *= 1.4f;
		speed *= 2;
	}

	//randomize colour
	colour[0] = randomDecimal();
	colour[1] = randomDecimal();
	colour[2] = randomDecimal();
};

/*
*	Generates a random floating point decimal between 0 and 1
*	@returns a random floating point decimal
*/
float Particle::randomDecimal()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
};
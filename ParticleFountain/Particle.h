/*
 *	ParticleFountain
 *		Particle.h
 *
 *	Cosc 3P98 Assignment 3
 *	Dec 5, 2013
 *	Dan Lapp, 4276804
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include "ProgramMode.h"
#include "Particle.h"

//constants
const float DEFAULT_SPEED = 0.004f; //0.004f


/*
 *	The particle class. This class represents an instance of a single particle, and
 *	stores all of the data for that particle.
 */
class Particle
{
public:
	float position[3]; //x, y, z position

	float direction[2]; //x, y direction, z direction always 0
	int rotationAngle; //rotates particle about the fountain
	float speed; //speed of particle

	/* Random Spin */
	float spinMultiplier[3]; //x, y, z multiplier for particle spin
	float spin[3]; //current rotation of the particle

	float colour[3];
	int scaleFactor;	
	bool removeParticle; //true if the particle should be removed next time cycle
	int lifeCycle;


public:
	Particle();

	/*
	 *	Initializes a particle with randomized values to give each particle different
	 *	behaviour.
	 *
	 *	@param mode the program mode passed from ParticleFountain, specifying the selected
	 *		modes in the fountain
	 */
	void randomizeParticle(ProgramMode* mode);

	/*
	 *	Generates a random floating point decimal between 0 and 1
	 *
	 *	@returns a random floating point decimal
	 */
	float randomDecimal();
};
#endif
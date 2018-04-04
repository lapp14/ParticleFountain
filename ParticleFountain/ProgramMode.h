/*
 *	ParticleFountain
 *		ProgramMode.h
 *
 *	Cosc 3P98 Assignment 3
 *	Dec 5, 2013
 *	Dan Lapp, 4276804
 */

#ifndef PROGRAMMODE_H
#define PROGRAMMODE_H

/*
 *	This class acts as a storage structure for all particle and fountain modes.
 */
class ProgramMode
{
public:
	/* Fountain Settings*/
	bool randomSpeed;
	bool randomSpin;
	bool randomSize;
	bool randomColour;
	bool useFriction;
	bool backfaceCulling;
	bool smoothShading;
	bool vertexMode;

	/* Fountain Fire Modes */
	bool manualFire;
	bool fireworksMode;
	bool sprayMode;

	ProgramMode()
	{
		reset();
	}

	/*
	 *	Sets all program modes to their default values
	 */
	void reset()
	{
		randomSpeed = false;
		randomSpin = false;
		randomSize = false;
		randomColour = false;
		useFriction = false;
		backfaceCulling = false;
		smoothShading = false;
		vertexMode = false;

		fireworksMode = false;
		sprayMode = false;
		manualFire = false;
	}
};

#endif
/*
 *	ParticleFountain
 *		ParticleFountain.h
 *
 *	Cosc 3P98 Assignment 3
 *	Dec 5, 2013
 *	Dan Lapp, 4276804
 */

#ifndef PARTICLEFOUNTAIN_H
#define PARTICLEFOUNTAIN_H

#include <iostream>
#include <vector>
#include <time.h>
#include "glut.h"
#include "Particle.h"
#include "ProgramMode.h"


/* Global constants */
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int GROUND_SIZE = 120;
const int MAX_CLIPPING = 750;
const int MAX_PARTICLES = 250;
const int MAX_FIREWORKS_LIFECYCLE = 3000;


/* global variable structure */
struct glob{
	bool runFountain;
	float gravity;
	float friction;
	float rotateAngle[3];
	float rotateFactor;
	int rotateAxis; //0 = x, 1 = y, 2 = z
	std::vector<Particle> particles;
};


glob global; //global variables
ProgramMode mode; //stores user choices for particle modes

/* Glut menu variables */ 
static int polygonMode;
static int polygonShading;

#endif
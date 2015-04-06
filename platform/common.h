#ifndef __COMMON_H
#define __COMMON_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_opengl.h>

#include "shape2d.h"
#include "shape3d.h"

#define EPSILON 0.0001
#define M_PI 3.14159265

using namespace std;

float clamp(float x, float a, float b);
#define signum(X) (((X) > 0) - ((X) < 0))

int randint(int a, int b);
float randdbl(float a, float b);
float random();

int rand_value_dict(int *count, int n);
int avg_value(int *vals, int n);
int avg_theta(int *theta, int n);

/* generate continuous approximation
   of geometric distribution of p
*/
float geometric(float p);

#endif

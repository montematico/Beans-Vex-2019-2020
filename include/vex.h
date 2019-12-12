/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//
#include <math.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
#include "v5.h"
#include "v5_vcs.h"
#include <strings.h>
#include "robot-config.h"
//dont have to put std:: infront of all console commmands
using std::cout;
using std::endl;
using std::string;


#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)

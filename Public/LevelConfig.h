// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define MARIO_MODE
/**
 * 
 */
class UDIVE_API LevelConfig
{
public:

#ifndef MARIO_MODE
	static inline const int FM_PL_Col = 4;
	static inline const float FM_PL_Config[] = 
	{
	    /* Vx    Vy    Vz   Dur*/
	       0.f,  0.f,  -1.f,   55.f,
	       0.f,  0.f,  -1.f,   5.f,

	       0.f,  1.f,  -1.f,   20.f,
	       0.f,  1.f,  -1.f,   5.f,

	       -1.f,  0.f,  -1.f,   20.f,
	       -1.f,  0.f,  -1.f,   10.f,

		   0.f,  0.f,  -1.5f,   15.f,
		   0.f,  0.f,  -1.5f,   3.f,

		   0.f,  0.f,  -0.0001f,  1000.f
	};

	static inline const int SPN_GATE_Col = 11;
	static inline const float SPN_GATE_0_Config[] = 
	{
	    /*  T     Dur      TGap    AngGap       GapMin                  XGapMa       Dev  */ 
	        40.f,  10.f,    1.5,    90.f,     0.f,  0.f,  0.f,    0.f, 0.f, 0.f,     20.f,
			100.f, 10.f,    1.5,    90.f,     0.f,  0.f,  0.f,    0.f, 0.f, 0.f,     20.f
	};


	/*                  RINGS               */
	static inline const int SPN_RING_Col = 10;
	static inline const float SPN_RING_0_Config[] = 
	{
	    /*  T     Dur    TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
	        15.,   5.,    0.3,       0.,     0.,      0.,    0.0,      10.,     10.,       1.,
			21,    5,     0.1,       0.,     10.,     0.,    0.0,      0.,      5.,       1.,
			63,    8,    1.25,       0.,     30.,     0.,    0.0,      50.,     15.,       1.
	};

	// Man
	static inline const float SPN_RING_1_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
	        50.,  5.1,      1.0,       0.,      0.,      0.,       0.0,   0.,        3.,        1.,
	};

	// L-Ring
	static inline const float SPN_RING_2_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
	        85,   5.,        0.1,      0.,     10.,      0.,       0.0,   0.,        8.,        1.,
			91.,  10.,       0.1,      0.,     -6.,    10.,       0.0,   0.,        8.,        1.
	};

	// Arrow
	static inline const float SPN_RING_3_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
			80.,  4.,        0.65,      0.,     0.,       00.,       0.0,   0.,        10.,        1.
	};

	// Grid
	static inline const float SPN_RING_4_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
			133.1,  0.15,     0.2,      0.,     0.,       0.,       0.0,   0.,        1.,        1.
	};


	static inline const int SPN_MBLCK_0_Col = 5;
	static inline const float SPN_MBLCK_0_Config[] = 
	{
	    /*  T     Dur      TGap  AngGap   dev*/ 
	    10.f,    200.f,    0.5,   4.f,   20.f
	};

	static inline const int SPN_VBLCK_Col = 13;
	static inline const float SPN_VBLCK_0_Config[] = 
	{
	    /*                        |      MinScale       |    MaxScaleX     |             |    	     | SpawnRage  */ 
	    /*  T     Dur     TGap    |  X       Y     Z    |  X      Y     Z  |  X-Margine  | Y-Margine | X      Y   */ 
			30.f,  10.f,  1.0f,      1.f,   1.f,  20.0f,     1.f,   1.f,  20.0f,    700.f,    700.f ,      1500.f,  1500.f,
			115.f, 30.f,  1.0f,      1.f,   1.f,  20.0f,     1.f,   1.f,  20.0f,    700.f,    700.f ,      1500.f,  1500.f
	};

	// random coins
	static inline const float SPN_VBLCK_1_Config[] = 
	{
	    /*                        |      MinScale       |    MaxScaleX     |             |    	     | SpawnRage  */ 
	    /*  T     Dur     TGap    |  X       Y     Z    |  X      Y     Z  |  X-Margine  | Y-Margine | X      Y   */ 
			-1.f,  0.f,  1.0f,      1.f,   1.f,  1.f,     1.f,   1.f,  1.f,    40.f,      40.f ,       200.f,  200.f

	};

	static inline const int SPN_SBLCK_Col = 8;
	static inline const float SPN_SBLCK_0_Config[] = 
	{
	    /*  T     Dur     TGap    MinScale   MaxScale  0-Margine  SpawnRage  SpinRange  */ 
	       0.f,   120.f,  2.0f,      10.f,        10.f,     3000.f,    3000.f,    1.0
	};

	static inline const int SPN_COIN_Col = 8;
	static inline const float SPN_COIN_0_Config[] = 
	{
	    /*  T     Dur     TGap    startOffsetX  startOffsetY endOffsetX  endOffsetY SpinRange  */ 
	       2.f,   0.3f,   0.01,        -300.f, 			-300.f, 		300.f, 		300.f, 	   0.0,
		   4.f,   0.3f,   0.01,        300.f, 			-300.f, 		-300.f,		300.f, 	   0.0
	};
#else
































	static inline const int FM_PL_Col = 4;
	static inline const float FM_PL_Config[] = 
	{
	    /* Vx    Vy    Vz   Dur*/
	       0.f,  0.f,  -1.f,   30.f,
	       0.f,  0.f,  -1.f,   30.f,

		   0.f,  0.f,  -1.5f,  30.f,
	       0.f,  0.f,  -1.5f,  30.f,
	};

	static inline const int SPN_GATE_Col = 11;
	static inline const float SPN_GATE_0_Config[] = 
	{
	    /*  T     Dur      TGap    AngGap       GapMin                  XGapMa       Dev  */ 
	        40.f,  10.f,    1.5,    90.f,     0.f,  0.f,  0.f,    0.f, 0.f, 0.f,     20.f,
	};

	/*                  RINGS               */
	static inline const int SPN_RING_Col = 10;
	static inline const float SPN_RING_0_Config[] = 
	{
	    /*  T     Dur    TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
	        5.,   15.,    1.0,       0.,     0.,      0.,    0.0,      10.,     1.,       1.,
	};

	// Rind 180
	static inline const float SPN_RING_1_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
	        10.,  10.,      2.5,       0.,      180.,   0.,      0.0,     0.,        2.,        1.,
			22.,  8.,       2.5,       90.,      40.,   0.,      0.0,     0.,       2.,        1.,
	};

	// Ring D
	static inline const float SPN_RING_2_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
			1.,  5.,       0.3,      0.,     0.,     0.,       0.0,    1.2,        1.,        1.
	};

	// Arrow
	static inline const float SPN_RING_3_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
			70.,  4.,        0.65,      0.,     0.,       00.,       0.0,   0.,        10.,        1.
	};

	static inline const float SPN_RING_4_Config[] = 
	{
	    /*  T     Dur       TGap    startRoll dRoll  startSpin  dSpin  randSpin  startScale  ratioScale*/ 
			133.1,  0.15,     0.2,      0.,     0.,       0.,       0.0,   0.,        1.,        1.
	};


	static inline const int SPN_MBLCK_0_Col = 5;
	static inline const float SPN_MBLCK_0_Config[] = 
	{
	    /*  T     Dur      TGap  AngGap   dev*/ 
	    55.f,    20.f,    0.1,   40.f,   20.f
	};

	static inline const int SPN_VBLCK_Col = 13;
	static inline const float SPN_VBLCK_0_Config[] = 
	{
	    /*                        |      MinScale       |    MaxScaleX        |             |    	     | SpawnRage  */ 
	    /*  T     Dur     TGap    |  X       Y     Z    |  X      Y     Z     |  X-Margine  | Y-Margine | X      Y   */ 
			32,  13,  1.0,      0.1,   0.1,  1,     	  0.1,   0.1,  1.0,    0,      0 ,       0,  0,
			66,  40, 1.0,      0.1,   0.1,  1,     	  0.1,   0.1,  1.0,    0,      0 ,       0,  0

	};
	// random coins
	static inline const float SPN_VBLCK_1_Config[] = 
	{
	    /*                        |      MinScale       |    MaxScaleX     |             |    	     | SpawnRage  */ 
	    /*  T     Dur     TGap    |  X       Y     Z    |  X      Y     Z  |  X-Margine  | Y-Margine | X      Y   */ 
			75.f,  40.f,  0.3f,      1.f,   1.f,  1.f,     1.f,   1.f,  1.f,    50.f,      50.f ,       100.f,  100.f

	};

	static inline const int SPN_SBLCK_Col = 8;
	static inline const float SPN_SBLCK_0_Config[] = 
	{
	    /*  T     Dur     TGap    MinScale   MaxScale  0-Margine  SpawnRage  SpinRange  */ 
	       0.f,   120.f,  2.0f,      10.f,        10.f,     3000.f,    3000.f,    1.0
	};

	static inline const int SPN_COIN_Col = 8;
	static inline const float SPN_COIN_0_Config[] = 
	{
	    /*  T     Dur     TGap    startOffsetX  startOffsetY endOffsetX  endOffsetY SpinRange  */ 
	       36.f,   2.f,   0.3,        0.f, 			200.f, 		173.f, 		-100.f,    2.f,
		   38.1f,  2.f,   0.3,        173.f, 	   -100.f, 		-173.f,		-100.f,    2.f,
		   40.2f,   2.f,   0.3,        -173.f,		-100.f,     0.f,         200.f,    2.f,

		   66.0f,  2.f,   0.5,        -200.f,		-200.f,     200.f,      -200.f,    2.f,
		   68.1f,  2.f,   0.5,        200.f,		-200.f,     200.f,      200.f,    2.f,
		   70.2f,  2.f,   0.5,        200.f,		200.f,     -200.f,      200.f,    2.f,
		   72.3f,  2.f,   0.5,        -200.f,		200.f,     -200.f,      -200.f,    2.f,
		   74.4f,  2.f,   0.5,        -200.f,		-200.f,     -100.f,      200.f,    2.f,
	};

#endif // MARIO_MODE

	LevelConfig();
	~LevelConfig();
};

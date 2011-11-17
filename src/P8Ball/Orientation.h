/**
 * @file
 * @author		Thaddeus Diamond
 * @version		0.1
 *
 * @section DESCRIPTION
 * This is a utilities header for standard things we might want to do repeatedly
 */

#ifndef _P8BALL_P8BALL_ORIENTATION_H_
#define _P8BALL_P8BALL_ORIENTATION_H_

#include <math.h>

/** Define the thee axes in space as an enum **/
enum {
	X_AXIS = 0,
	Y_AXIS = 1,
	Z_AXIS = 2,
};

/** Create constants for upper and lower bounds on gravitational force **/
#define UPPER_GRAVITY 1.15
#define LOWER_GRAVITY 0.75

/** The gyroscope is really noise so we clamp what we consider as change **/
#define GYROMETER_NOISE		20

/** In order to avoid jarring movement we have to smooth the color transition **/
#define REFRESH_INTERVAL	2
#define STABLE				(3 * JARRING)
#define JARRING				0.02

/** We create some standard templated functions because abs(), etc. don't operate on floats **/
#define ABS(a)				((a) > 0 ? (a) : (-1 * (a)))
#define MIN(a,b)			((a) < (b) ? (a) : (b))
#define MAX(a,b)			((a) > (b) ? (a) : (b))

/**
 * The GravityAccountsFor method takes an acceleration in a specified dimension and
 * determines whether or not gravity is solely accounting for that acceleration
 *
 * @param		acceleration		An acceleration reading for three directions
 * @returns		A set of booleans describing which way gravity is pointing (or all false if unknown)
 */
static inline bool* GravitationalTruths(Windows::Devices::Sensors::AccelerometerReading^ accel_reading) {
	// Make something to put on the heap via new
	bool* truths = new bool[3];
	truths[0] = truths[1] = truths[2] = false;

	// Calculate if the total magnitude is approximately .98
	double magnitude = sqrt(pow(accel_reading->AccelerationX, 2) +
							pow(accel_reading->AccelerationY, 2) +
							pow(accel_reading->AccelerationZ, 2));
	if (magnitude > UPPER_GRAVITY || magnitude < LOWER_GRAVITY)
		return truths;

	// Test each direction individually
	if (ABS(accel_reading->AccelerationX) < UPPER_GRAVITY && ABS(accel_reading->AccelerationX) > LOWER_GRAVITY)
		truths[X_AXIS] = true;
	else if (ABS(accel_reading->AccelerationY) < UPPER_GRAVITY && ABS(accel_reading->AccelerationY) > LOWER_GRAVITY)
		truths[Y_AXIS] = true;
	else if (ABS(accel_reading->AccelerationZ) < UPPER_GRAVITY && ABS(accel_reading->AccelerationZ) > LOWER_GRAVITY)
		truths[Z_AXIS] = true;
	return truths;
}

#endif  // _P8BALL_P8BALL_ORIENTATION_H_
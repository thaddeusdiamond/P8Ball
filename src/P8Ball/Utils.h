/**
 * @file
 * @author		Thaddeus Diamond
 * @version		0.1
 *
 * @section DESCRIPTION
 * This is a utilities header for standard things we might want to do repeatedly
 */

/** @todo Make sure you reference the fact that you got help from the dev forum **/

#ifndef _P8BALL_P8BALL_UTILS_H_
#define _P8BALL_P8BALL_UTILS_H_

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

/**
 * We use die as a more powerful replacement for a typical DIE macro, allowing
 * us to exit with a message and status code from anywhere in our application.
 *
 * @param	error_code		The error code we make a call to exit() with
 * @param	format			Format of the message we are printing to stderr
 * @param	...				Arguments for the message we are printing
 */
static inline void DIE(unsigned int error_code, const char* format, ...) {
	va_list arguments;
	va_start(arguments, format);

	fprintf(stderr, format, arguments);
	va_end(arguments);
	exit(error_code);
}

#endif  // _P8BALL_P8BALL_UTILS_H_
/** \file
 * \author Ryan-David Reyes
 *
 * \brief This file contains useful numerical methods.
 */

#ifndef NUMERICAL_H_
#define NUMERICAL_H_
#include <ring_buffer.h>

/**
 * \brief This function takes a 5th order approximation of the first derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_fdFirstDer(ring_buffer_t r_buff, float dt);


/**
 * \brief This function takes a 4th order finite difference approximation of the
 * second derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_fdSecondDer(ring_buffer_t r_buff, float dt);

/**
 * \brief This function takes a window size 5 savitsky golay approximation of
 * the second derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_sgSecondDer(ring_buffer_t r_buff, float dt);

/**
 * \brief This function takes a 6th order savitsky golay approximation of the
 * second derivative.
 *
 * \param r_buff Ring buffer of size 7
 * \param dt Sample time between the points.
 */
float nm_sgSecondDer6(ring_buffer_t r_buff, float dt);


/**
 * \brief This function mallocs an array of floats between start and end, in
 * increments of increment.
 *
 * \param start First element of the array value
 * \param increment Space between values
 * \param end End value of the array
 * \param array Pointer which will hold the array when done.
 *
 * \return Returns the size of the array.
 */
size_t nm_genRange(float start, float increment, float end, float **array);

/**
 * \brief This function simulates a recursive first order exponential process:
 * 1-e^(-t/tau)
 *
 * \param prev Previous value of the process
 * \param setpoint Where the process is heading to
 * \param sample_time Time between the samples
 * \param tau Time constant of the exponential process. Should be roughly
 * .25*rise time of simulated system.
 *
 * \return Returns the current value of the process
 */
float nm_expProcess(float prev, float setpoint, float sample_time, float tau);
#endif

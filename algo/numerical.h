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

#endif

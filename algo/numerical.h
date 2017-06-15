/** \file
 * \author Ryan-David Reyes
 *
 * \brief This file contains useful numerical methods.
 */

#ifndef NUMERICAL_H_
#define NUMERICAL_H_
#include <ring_buffer.h>

/**
 * \brief This function takes a 6th order approximation of the first derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_fdFirstDer(ring_buffer_t r_buff, float dt);


/**
 * \brief This function takes a 6th order approximation of the second
 * derivative.
 *
 * \param r_buff Ring buffer of size 6
 * \param dt Sample time between the points.
 */
float nm_fdSecondDer(ring_buffer_t r_buff, float dt);

#endif

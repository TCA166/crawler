
#pragma once

#define SCALE 1e-4

#define INITIAL_VELOCITY(mass, distance)                                       \
    sqrt(gravitational_constant *mass / distance)

const double gravitational_constant = 6.67430e-11;

const double moon_mass = 7.34767309e22 * SCALE;
const double moon_radius = 1737.4 * SCALE;
const double moon_rot = 27.322; // days

const double earth_mass = 5.972e24 * SCALE;
const double earth_radius = 6378.0 * SCALE;

const double sun_mass = 1.988416e30 * SCALE;
const double sun_radius = 6.957e5 * SCALE;

const double earth_moon_distance = 384400.0 * SCALE;
const double earth_sun_distance = 149.6e6 * SCALE;

inline double calculate_gravity(double mass1, double mass2, double distance) {
    return (gravitational_constant * mass1 * mass2) / (distance * distance);
}

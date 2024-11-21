
#pragma once

#define SCALE 1e-4

#define INITIAL_VELOCITY(mass, distance) sqrt(gravitational_constant * mass / distance)

const double gravitational_constant = 6.67430e-11;

const double moon_mass = 7.34767309e22 * SCALE;
const double moon_radius = 1737.4 * SCALE;

const double earth_mass = 5.972e24 * SCALE;
const double earth_radius = 6378.0 * SCALE;

const double sun_mass = 1.988416e30 * SCALE;
const double sun_radius = 6.957e5 * SCALE;

const double earth_moon_distance = 384400.0 * SCALE;
const double earth_sun_distance = 149.6e6 * SCALE;

//FIXME separate this out

inline double calculate_gravity(double mass1, double mass2, double distance) {
    return (gravitational_constant * mass1 * mass2) / (distance * distance);
}

inline double calculate_initial_velocity(double earth_mass, double moon_mass, double sun_mass, double earth_moon_distance, double earth_sun_distance) {
    // Orbital velocity around the Earth
    double v_earth_moon = INITIAL_VELOCITY(earth_mass, earth_moon_distance);

    // Gravitational force from the Sun
    double F_sun = calculate_gravity(sun_mass, moon_mass, earth_sun_distance + earth_moon_distance);

    // Effective acceleration due to the Sun's gravity
    double a_sun = F_sun / moon_mass;

    // Adjust the velocity to account for the Sun's gravity
    double v_adjusted = v_earth_moon + a_sun * earth_moon_distance / v_earth_moon;

    return v_adjusted;
}

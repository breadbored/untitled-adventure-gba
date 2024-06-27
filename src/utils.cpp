#include <math.h>
#include "utils.hpp"
#include "globals.h"
#include "bn_math.h"
#include "bn_fixed.h"

static uint32_t rand_state = 8888;
uint32_t rand_bc(void) {
	uint32_t x = rand_state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 5;
	return rand_state = x;
}
int random_int(int lower, int upper)
{
    return (rand_bc() % (upper - lower + 1)) + lower;
}

// // I use this Quick Sqrt from Quake III and left in the comments for fun
// // https://en.wikipedia.org/wiki/Fast_inverse_square_root
// // enjoy the fuckery that is bit manipulation
// bn::fixed Q_rsqrt(bn::fixed numberFixed)
// {
//   long i;
//   float x2, y;
//   const float threehalfs = 1.5F;
//   float number = numberFixed.to_float();

//   x2 = number * 0.5F;
//   y  = number;
//   i  = * ( long * ) &y;                       // evil floating point bit level hacking
//   i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
//   y  = * ( float * ) &i;
//   y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//   // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

//   return y;
// }

bn::fixed Q_rsqrt(bn::fixed numberFixed) {
    float number = numberFixed.to_float();
    if (number <= 0.0f) return 0.0f;
    float approx = number * 0.5f;
    for (int i = 0; i < 2; ++i) { // only 2 iterations for low precision
        approx = (approx + number / approx) * 0.5f;
    }
    return approx;
}

bn::fixed lerpf( bn::fixed a, bn::fixed b, bn::fixed t )
{
    return (1.0f - t) * a + t * b;
}

int lerp( int a, int b, bn::fixed t )
{
    return (int)lerpf( (bn::fixed)a, (bn::fixed)b, t );
}

vector2f_t lerp2f( vector2f_t a, vector2f_t b, bn::fixed t )
{
    vector2f_t result;
    result.x = lerpf(a.x, b.x, t);
    result.y = lerpf(a.y, b.y, t);
    return result;
}

vector2_t lerp2( vector2_t a, vector2_t b, bn::fixed t )
{
    vector2_t result;
    result.x = lerp(a.x, b.x, t);
    result.y = lerp(a.y, b.y, t);
    return result;
}

bool are_same(bn::fixed x, bn::fixed y)
{
    return bn::abs(x - y) < 0.01f;
}

vector2_t get_screen_coordinates(vector2_t position) {
    vector2_t screen_coordinates;
    screen_coordinates.x = position.x - screen_relative_position.x.round_integer() + ((screen_size.x / 2) - 16);
    screen_coordinates.y = position.y - screen_relative_position.y.round_integer() + ((screen_size.y / 2) - 16);
    return screen_coordinates;
}

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

vector2f_t normalize(vector2f_t vector, bn::fixed length) {
    if (vector.x == 0 && vector.y == 0) return vector;
    if (vector.x == 0) return {0, length * (vector.y < 0 ? -1 : 1)};
    if (vector.y == 0) return {length * (vector.x < 0 ? -1 : 1), 0};
    bn::fixed magnitude = Q_rsqrt(vector.x * vector.x + vector.y * vector.y) * length;
    vector2f_t result;
    result.x = vector.x / magnitude * length;
    result.y = vector.y / magnitude * length;
    return result;
}

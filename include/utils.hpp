#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BIT32_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c'%c%c%c%c%c%c%c%c'%c%c%c%c%c%c%c%c'%c%c%c%c%c%c%c%c"
#define BIT_REP(bit)           (bit ? '1' : '0')
#define BYTE_TO_BINARY(byte)   BIT_REP(byte & 0x80), BIT_REP(byte & 0x40), BIT_REP(byte & 0x20), BIT_REP(byte & 0x10), BIT_REP(byte & 0x08), BIT_REP(byte & 0x04), BIT_REP(byte & 0x02), BIT_REP(byte & 0x01) 
#define U32_TO_BINARY(final)   BYTE_TO_BINARY((final >> 24)), BYTE_TO_BINARY((final >> 16)), BYTE_TO_BINARY((final >> 8)), BYTE_TO_BINARY((final & 0xFF))

#define npc_t uint8_t // TODO Remove this

int random_int(int lower, int upper);
uint32_t rand_bc(void);

struct vector2f_t
{
    double x;
    double y;
};

struct vector2_t
{
    int x;
    int y;
};

double lerpf( double a, double b, double t );

int lerp( int a, int b, double t );

vector2f_t lerp2f( vector2f_t a, vector2f_t b, double t );

vector2_t lerp2( vector2_t a, vector2_t b, double t );

bool are_same(double x, double y);

vector2_t get_screen_coordinates(vector2_t position);

int clamp(int value, int min, int max);

vector2f_t normalize(vector2f_t vector, double length);

#endif /* __UTILS_H */
/*
File:   math.h
Author: Taylor Robbins
Date:   09\23\2023
*/

#ifndef _MATH_H
#define _MATH_H

// +--------------------------------------------------------------+
// |                           Defines                            |
// +--------------------------------------------------------------+
#define NAN       __builtin_nanf("")
#define INFINITY  __builtin_inff()

// +--------------------------------------------------------------+
// |                          Functions                           |
// +--------------------------------------------------------------+
float  fminf(float value1, float value2);
double fmin(double value1, double value2);

float  fmaxf(float value1, float value2);
double fmax(double value1, double value2);

float       fabsf(float value);
double      fabs(double value);
long double fabsl(long double value);

float       fmodf(float numer, float denom);
double      fmod(double numer, double denom);
long double fmodl(long double numer, long double denom);

float  roundf(float value);
double round(double value);

float  floorf(float value);
double floor(double value);

float  ceilf(float value);
double ceil(double value);

float  sinf(float value);
double sin(double value);

float  asinf(float value);
double asin(double value);

float  cosf(float value);
double cos(double value);

float  acosf(float value);
double acos(double value);

float  tanf(float value);
double tan(double value);

float  atanf(float value);
double atan(double value);

float  atan2f(float numer, float denom);
double atan2(double numer, double denom);

float  powf(float value, float exponent);
double pow(double value, double exponent);

float  sqrtf(float value);
double sqrt(double value);

float  cbrtf(float value);
double cbrt(double value);

float  logf(float value);
double log(double value);

float  log2f(float value);
double log2(double value);

float  log10f(float value);
double log10(double value);

double ldexp(double value, int exponent);
float  ldexpf(float value, int exponent);

float       scalbnf(float value, int power);
double      scalbn(double value, int power);
long double scalbnl(long double value, int power);

float       copysignf(float magnitude, float sign);
double      copysign(double magnitude, double sign);
long double copysignl(long double magnitude, long double sign);

#endif //  _MATH_H

// +--------------------------------------------------------------+
// |                   Autocomplete Dictionary                    |
// +--------------------------------------------------------------+
/*
@Defines
NAN
INFINITY
@Functions
float  fminf(float value1, float value2)
double fmin(double value1, double value2)
float  fmaxf(float value1, float value2)
double fmax(double value1, double value2)
float       fabsf(float value)
double      fabs(double value)
long double fabsl(long double value)
float       fmodf(float numer, float denom)
double      fmod(double numer, double denom)
long double fmodl(long double numer, long double denom)
float  roundf(float value)
double round(double value)
float  floorf(float value)
double floor(double value)
float  ceilf(float value)
double ceil(double value)
float  sinf(float value)
double sin(double value)
float  asinf(float value)
double asin(double value)
float  cosf(float value)
double cos(double value)
float  acosf(float value)
double acos(double value)
float  tanf(float value)
double tan(double value)
float  atanf(float value)
double atan(double value)
float  atan2f(float numer, float denom)
double atan2(double numer, double denom)
float  powf(float value, float exponent)
double pow(double value, double exponent)
float  sqrtf(float value)
double sqrt(double value)
float  cbrtf(float value)
double cbrt(double value)
float  logf(float value)
double log(double value)
float  log2f(float value)
double log2(double value)
float  log10f(float value)
double log10(double value)
double ldexp(double value, int exponent)
float  ldexpf(float value, int exponent)
float       scalbnf(float value, int power)
double      scalbn(double value, int power)
long double scalbnl(long double value, int power)
float       copysignf(float magnitude, float sign)
double      copysign(double magnitude, double sign)
long double copysignl(long double magnitude, long double sign)
*/

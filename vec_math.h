#pragma once
#include<windows.h>
#include"animation.h"

#define PI           3.14159265358979323846
#define G            6.68e-11
#define GRAVITY      9.8

double deg_to_rad(double a);
double rad_to_deg(double a);

typedef struct
{
	vec a, b, c;
} rect_t;

typedef struct
{
	rect_t screen;
	rect_t math;
} viewport_t;

typedef struct
{
	vec r, v, a, f1;
	double t;
} object_t;

typedef struct timer_tag
{
	LARGE_INTEGER freq, last;

	// функции
	void(*start)(struct timer_tag*);
	double(*step)(struct timer_tag*);
} timer_t;

void rect_set(rect_t* r, double x1, double y1, double z1, double x2, double y2, double z2);

vec vec_create(double x, double y, double z);

vec vec_add(vec a, vec b);

vec vec_sub(vec a, vec b);

vec vec_add_mul(vec a, vec b, double t);

vec vec_mul_double(vec a, double t);

vec vec_transform(vec p, rect_t const* from, rect_t const* to);

void real_timer_start(struct timer_tag* timer);

double real_timer_step(struct timer_tag* timer);

vec force(vec r, double mass);

void runge_kutta_step(object_t* obj, double dt, double mass);

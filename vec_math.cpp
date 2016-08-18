#include "vec_math.h"

double deg_to_rad(double a)
{
	return a * PI / 180.0;
}

double rad_to_deg(double a)
{
	return a * 180.0 / PI;
}

void rect_set(rect_t* r, double x1, double y1, double z1, double x2, double y2, double z2)
{
	r->a.x = x1;
	r->a.y = y1;
	r->a.z = z1;
	r->b.x = x2;
	r->b.y = y2;
	r->b.z = z2;
}

vec vec_create(double x, double y, double z)
{
	vec res = { x, y, z };
	return res;
}

vec vec_add(vec a, vec b)
{
	vec res = { a.x + b.x, a.y + b.y , a.z + b.z};
	return res;
}

vec vec_sub(vec a, vec b)
{
	vec res = { a.x - b.x, a.y - b.y, a.z - b.z };
	return res;
}

vec vec_add_mul(vec a, vec b, double t)
{
	vec res = { a.x + b.x * t, a.y + b.y * t, a.z + b.z * t };
	return res;
}

vec vec_mul_double(vec a, double t)
{
	vec res = { a.x * t, a.y * t, a.z * t };
	return res;
}

vec vec_transform(vec p, rect_t const* from, rect_t const* to)
{
	vec res = {
		to->a.x + (p.x - from->a.x) * (to->b.x - to->a.x) / (from->b.x - from->a.x),
		to->a.y + (p.y - from->a.y) * (to->b.y - to->a.y) / (from->b.y - from->a.y),
		to->a.z + (p.z - from->a.z) * (to->b.z - to->a.z) / (from->b.z - from->a.z),
	};
	return res;
}

// Таймер

void real_timer_start(struct timer_tag* timer)
{
	QueryPerformanceFrequency(&timer->freq);
	QueryPerformanceCounter(&timer->last);

}

double real_timer_step(struct timer_tag* timer)
{
	double dt = 0.5;
	LARGE_INTEGER begin;
	begin = timer->last;
	QueryPerformanceCounter(&timer->last);
	dt = (double)(timer->last.QuadPart - begin.QuadPart) / (double)timer->freq.QuadPart;
	return dt * 3600;
}

vec force(vec r, double mass)
{
	double res = 0;
	res = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
	return vec_mul_double(r, -G * mass / pow(res, 3));
}

void runge_kutta_step(object_t* obj, double dt, double mass)
{
	vec a1, a2, a3, a4,     //ускорение
		v1, v2, v3, v4;           //скорость   

	v1 = obj->v;
	a1 = force(obj->r, mass);
	v2 = vec_add(obj->v, vec_mul_double(a1, dt / 2));
	a2 = force(vec_add(obj->r, vec_mul_double(v1, dt / 2)), mass);
	v3 = vec_add(obj->v, vec_mul_double(a2, dt / 2));
	a3 = force(vec_add(obj->r, vec_mul_double(v2, dt / 2)), mass);
	v4 = vec_add(obj->v, vec_mul_double(a3, dt));
	a4 = force(vec_add(obj->r, vec_mul_double(v3, dt)), mass);

	obj->r = vec_add(obj->r, vec_mul_double(vec_add(vec_add(v1, vec_mul_double(v2, 2)), vec_add(v4, vec_mul_double(v3, 2))), dt / 6));
	obj->v = vec_add(obj->v, vec_mul_double(vec_add(vec_add(a1, vec_mul_double(a2, 2)), vec_add(a4, vec_mul_double(a3, 2))), dt / 6));
	obj->a = a4;
}

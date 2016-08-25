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
	double dt = 0.2;
	LARGE_INTEGER begin;
	begin = timer->last;
	QueryPerformanceCounter(&timer->last);
	dt = (double)(timer->last.QuadPart - begin.QuadPart) / (double)timer->freq.QuadPart;
	return dt * 3600 ;
}

vec force(vec planet_coord, vec sat_coord, double planet_mass, double sat_mass)
{
	double len = sqrt(pow(planet_coord.x - sat_coord.x, 2) + pow(planet_coord.y - sat_coord.y, 2) + pow(planet_coord.z - sat_coord.z, 2));
	double F = G * planet_mass * sat_mass / pow(len,2);
	vec res = { (planet_coord.x - sat_coord.x) / len * F,
				(planet_coord.y - sat_coord.y) / len * F ,
				(planet_coord.z - sat_coord.z) / len * F };
	return res;
}

vec A1(vec r, double mass)
{
	double res = 0;
	res = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
	return vec_mul_double(r, -G * mass / pow(res, 3));
}

void euler_step(object_t* obj, double dt)
{
	obj->v = vec_add_mul(obj->v, obj->a, dt);
	obj->r = vec_add_mul(obj->r, obj->v, dt);

}

void euler_step_advanced(object_t* obj, double dt)
{
	double t;
	for (t = 0; t <= dt; t += MAX_DT)
	{
		obj->v = vec_add_mul(obj->v, obj->a, MAX_DT);
		obj->r = vec_add_mul(obj->r, obj->v, MAX_DT);
	}
	obj->v = vec_add_mul(obj->v, obj->a, dt - t + MAX_DT);
	obj->r = vec_add_mul(obj->r, obj->v, dt - t + MAX_DT);
}

void runge_kutta_step(object_t* obj, double dt, double mass_planet, double mass_sat)
{
	double mass = mass_planet * mass_sat;
	vec a1, a2, a3, a4,     //ускорение
		v1, v2, v3, v4;           //скорость   

	v1 = obj->v;
	a1 = A1(obj->r, mass);
	v2 = vec_add(obj->v, vec_mul_double(a1, dt / 2));
	a2 = A1(vec_add(obj->r, vec_mul_double(v1, dt / 2)), mass);
	v3 = vec_add(obj->v, vec_mul_double(a2, dt / 2));
	a3 = A1(vec_add(obj->r, vec_mul_double(v2, dt / 2)), mass);
	v4 = vec_add(obj->v, vec_mul_double(a3, dt));
	a4 = A1(vec_add(obj->r, vec_mul_double(v3, dt)), mass);

	obj->r = vec_add(obj->r, vec_mul_double(vec_add(vec_add(v1, vec_mul_double(v2, 2)), vec_add(v4, vec_mul_double(v3, 2))), dt / 6));
	obj->v = vec_add(obj->v, vec_mul_double(vec_add(vec_add(a1, vec_mul_double(a2, 2)), vec_add(a4, vec_mul_double(a3, 2))), dt / 6));
	obj->a = a4;
}

double range(vec a, vec b)
{
	double res;
	res = pow(a.x - b.x, 2) +
		  pow(a.y - b.y, 2) +
		  pow(a.z - b.z, 2);
	res = sqrt(res);
	return res;
}
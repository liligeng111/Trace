
#include <cmath>

#include "Torus.h"

double solveQuartic(double a, double b, double c, double d, double e)
{
	/*const double alpha = - 3 * b * b / (8 * a * a) + c / a;
	const double beta = b * b * b / (8 * a * a * a) - b * c / (2 * a * a) + d / a;
	const double gamma = -3 * b * b * b * b / (256 * a * a * a * a) + b * b * c / (16 * a * a * a) - b * d / (4 * a * a) + e / a;
	const double P = - alpha * alpha / 12 - gamma;
	const double Q = - alpha * alpha * alpha / 108 + alpha * gamma / 3 - beta * beta / 8;
	const double R = - Q / 2 + sqrt(Q * Q / 4 + P * P * P / 27);
	const double U = pow(R, 1.0 / 3.0);
	const double y = - 5 * alpha / 6 - U + ((abs(U) < RAY_EPSILON) ? 0 : P / (3 * U));
	double root1 = alpha + 2 * y;

	//no real root;
	if (root1 < 0) return 20000000;
	root1 = sqrt(root1);
	double t = 1000000;

	double root2 = -(3 * alpha + 2 * y + 2 * beta / root1);
	if (root2 > 0)
	{
		root2 = sqrt(root2);
		double result = - b / (4 * a) + (root1 - root2) / 2;
		if (result > RAY_EPSILON && result < t) t = result;
		result = - b / (4 * a) + (root1 + root2) / 2;
		if (result > RAY_EPSILON && result < t) t = result;
	}

	root1 = -(root1);
	root2 = -(3 * alpha + 2 * y + 2 * beta / root1);
	if (root2 > 0)
	{
		root2 = sqrt(root2);
		double result = - b / (4 * a) + (root1 - root2) / 2;
		if (result > RAY_EPSILON && result < t) t = result;
		result = - b / (4 * a) + (root1 + root2) / 2;
		if (result > RAY_EPSILON && result < t) t = result;
		}*/
	double a3 = b / a;
	double a2 = c / a;
	double a1 = d / a;
	double a0 = e / a;

	double T1 = -a3 / 4;
	double T2 = a2 * a2 - 3*a3*a1 + 12*a0;
	double T3 = (2*a2*a2*a2 - 9*a3*a2*a1 + 27*a1*a1 + 27*a3*a3*a0 - 72*a2*a0)/2;
	double T4 = (-a3*a3*a3 + 4*a3*a2 - 8*a1)/32;
	double T5 = (3*a3*a3 - 8*a2)/48;

	double R1 = sqrt(T3*T3 - T2*T2*T2);
	double R2 = pow(T3 + R1, 1.0/3.0);
	double R3 = (1.0/12.0)*(T2/R2 + R2);
	double R4 = sqrt(T5 + R3);
	double R5 = 2*T5 - R3;
	double R6 = T4/R4;

	double r1 = T1 - R4 - sqrt(R5 - R6);
	double r2 = T1 - R4 + sqrt(R5 - R6);
	double r3 = T1 + R4 - sqrt(R5 + R6);
	double r4 = T1 + R4 + sqrt(R5 + R6);

	printf("%f\n", r1);
	printf("%f\n", r2);
	printf("%f\n", r3);
	printf("%f\n", r4);

	return r1;
}

bool Torus::intersectLocal( const ray& r, isect& i ) const
{
	double test = solveQuartic(3, 6, -123, -126, 1080);
	//printf("%f\n", test);
	return false;

	i.obj = this;
	vec3f e = r.getPosition();
	vec4f d = r.getDirection();

	const double xd4 = d[0] * d[0] * d[0] * d[0];
	const double yd4 = d[1] * d[1] * d[1] * d[1];
	const double zd4 = d[2] * d[2] * d[2] * d[2];
	const double xd3 = d[0] * d[0] * d[0];
	const double yd3 = d[1] * d[1] * d[1];
	const double zd3 = d[2] * d[2] * d[2];
	const double xd2 = d[0] * d[0];
	const double yd2 = d[1] * d[1];
	const double zd2 = d[2] * d[2];
	
	const double xe4 = e[0] * e[0] * e[0] * e[0];
	const double ye4 = e[1] * e[1] * e[1] * e[1];
	const double ze4 = e[2] * e[2] * e[2] * e[2];
	const double xe3 = e[0] * e[0] * e[0];
	const double ye3 = e[1] * e[1] * e[1];
	const double ze3 = e[2] * e[2] * e[2];
	const double xe2 = e[0] * e[0];
	const double ye2 = e[1] * e[1];
	const double ze2 = e[2] * e[2];

	const double r2 = radius * radius;

	const double A  = xd4 + yd4 + zd4 + 2 * (xd2 * yd2 + xd2 * zd2 + yd2 * zd2);
	const double B  = 4 * (xd3 * e[0] + yd3 * e[1] + zd3 * e[2] + xd2 * e[1] * d[1] + xd2 * e[2] * d[2] + yd2 * e[0] * d[0] + yd2 * e[2] * d[2] + zd2 * e[0] * d[0] + zd2 * e[1] * d[1]);
	const double C  = 2 * (-xd2 - yd2 + zd2 - r2 * xd2 - r2 * yd2 - r2 * zd2) + 6 * xd2 * xe2 + 2 * xe2 * yd2 + 8 * e[0] * e[1] * d[0] * d[1] + 2 * xd2 * ye2 + 6 * yd2 * ye2 + 2 * xe2 * zd2 + 2 * ye2 * zd2 +
		8 * e[0] * e[2] * d[0] * d[2] + 8 * e[1] * e[2] * d[1] * d[2] + 2 * xd2 * ze2 + 2 * yd2 * ze2 + 6 * zd2 * ze2;
	const double D = 4 * (-e[0] * d[0] - e[1] * d[1] + e[2] * d[2] - r2 * e[0] * d[0] - r2 * e[1] * d[1] - r2 * e[2] * d[2] + d[0] * xe3 + xe2 * d[1] * e[1] + ye2 * d[0] * e[0] + ye3 * d[1] + xe2 * e[2] * d[2]
		+ ye2 * e[2] * d[2] + ze2 * e[0] * d[0] + ze2 * d[1] * e[1] + ze3 * d[2]);
	const double E = 1.0 - xe2 - ye2 + ze2 + r2 * r2 - 2 * r2 * (1 + xe2 + ye2 + ze2) + xe4 + ye4 + ze4 + 2 * (xe2 * ye2 + xe2 * ze2 + ye2 * ze2);

	double t = solveQuartic(A, B, C, D, E);

	if (t > 1000000) return false;
	i.obj = this;
	i.t = t;
	i.N = r.at(t).normalize();
	return true;
}


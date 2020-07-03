#pragma once

#include "..\Vector.h"


struct rkmethod_t
{
	const Matrix<double> A;
	const Vector<double> c;
	const Vector<double> b_main;
	const Vector<double> b_subs;

	rkmethod_t(const Matrix<double>& A_, const Vector<double>& c_, const Vector<double>& b_main_, const Vector<double>& b_subs_) :
	A(A_), c(c_), b_main(b_main_), b_subs(b_subs_)
	{ }

	static rkmethod_t Verner_5_6_8;
};


// Verner's RK5(6)8
rkmethod_t rkmethod_t::Verner_5_6_8(
	{{0,				0,				0,				0,				0,				0,		0,				0},
		{1.0 / 18,        0,				0,				0,				0,				0,		0,				0},
		{-1.0 / 12,       1.0 / 4,			0,				0,				0,				0,		0,				0},
		{-2.0 / 81,       4.0 / 27,			8.0 / 81,			0,				0,				0,		0,				0},
		{40.0 / 33,       -4.0 / 11,		-56.0 / 11,		54.0 / 11,		0,				0,		0,				0},
		{-369.0 / 73,     72.0 / 73,		5380.0 / 219,		-12285.0 / 584,	2695.0 / 1752,	0,		0,				0},
		{-8716.0 / 891,   656.0 / 297,		39520.0 / 891,	-416.0 / 11,		52.0 / 27,		0.0,	0,				0},
		{3015.0 / 256,    -9.0 / 4,		-4219.0 / 78,	5985.0 / 128,	-539.0 / 384,	0.0,	693.0 / 3328,	0}}, 
	{ 0.0, 1.0 / 18, 1.0 / 6, 2.0 / 9, 2.0 / 3, 1.0, 8.0 / 9, 1.0 }, // c
	{ 3.0 / 80, 0.0, 4.0 / 25, 243.0 / 1120, 77.0 / 160, 73.0 / 700, 0.0, 0.0 }, // b_main
	{ 57.0 / 640, 0.0, -16.0 / 65, 1377.0 / 2240, 121.0 / 320, 0.0, 891.0 / 8320, 2.0 / 35 } // b_subs
);

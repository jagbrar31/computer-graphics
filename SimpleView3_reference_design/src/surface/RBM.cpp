#include "RBM.hpp"

#define Pi 3.141592653589793238462643

RBM::RBM() {
	reset();
}

RBM::~RBM() {
}

void RBM::reset() {
	R = 360;
	S = 5;
	T = 25;
	row = R / 5 + 1;
	col = T + 1;
}

// generate the mesh point and normal of rotation surface

void RBM::RotateCurve() {
	Point pts[col];
	Vector tangents[col];
	// 1. call curve->computeBezPtTan function to fill up pts and tangents
	// 2. rotate curve to generate surface mesh point and normal data and store in Pts and Normal.

}


void RBM::setRotationCurve(Bezier *curve1)
{
	curve = curve1;
}

#include "Surface.hpp"

extern RenderMode renderMode;

Surface::Surface() {
	reset();
}

void Surface::reset() {
	row = 0;
	col = 0;
}

void Surface::drawSurface() {

	switch (renderMode) {
	case WIRE:
        // your code to mesh wire
		break;
	case CONSTANT:
	case FLAT:
	case SMOOTH:
	case PHONE:
	case TEXTURE:
		glPolygonMode( GL_FRONT, GL_FILL);
		glPolygonMode( GL_BACK, GL_LINE);
        // your code to draw filled mesh, using GL_QUAD_STRIP



		break;
	}
}

void Surface::draw() {
	glPushMatrix();
	ctmMultiply();
	glScalef(s * 0.01, s * 0.01, s * 0.01);		// to shrink the object to size of viewable
	drawSurface();
	glPopMatrix();
}


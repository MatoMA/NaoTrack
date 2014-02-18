/*
 * Point.hpp
 *
 *  Created on: Jan 22, 2014
 *      Author: sheng
 */

#ifndef POINT_HPP_
#define POINT_HPP_

// Let us use 2D points as prototypes.
// Our GNG-T will contain 2D points as prototypes.
class Point {
public:
	double x, y;
	int label;

	Point(void) :
			x(0), y(0),label(0) {
	}
	Point(double xx, double yy) :
			x(xx), y(yy), label(0) {
	}
	Point(const Point& p) :
			x(p.x), y(p.y),label(p.label) {
	}
	Point& operator=(const Point& p) {
		if (this != &p) {
			x = p.x;
			y = p.y;
		}
		return *this;
	}

	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y);
	}

	// We will obtain vector operation from the default class. So our
	// type must implement those methods.

	Point& operator=(double value) {
		x = value;
		y = value;
		return *this;
	}

	Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		return *this;
	}

	Point& operator/=(double scale) {
		x /= scale;
		y /= scale;
		return *this;
	}

	Point& operator*=(double scale) {
		x *= scale;
		y *= scale;
		return *this;
	}
};

#endif /* POINT_HPP_ */

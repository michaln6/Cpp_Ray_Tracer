#ifndef _PLANE_H
#define _PLANE_H
#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

//tutaj definiujemy obiekt poziomej p�aszczyzny oraz wszystkie metody z ni� zwi�zane

class Plane : public Object {
	//normal to wektor wsp�rz�dnych p�aszczyzny
	Vect normal;
	//distance to odleg�o�� od punktu pocz�tkowego {0, 0, 0}
	double distance;
	//color to kolor p�aszczyzny
	Color color;

public:
	Plane();

	Plane(Vect, double, Color);

	//metody funkcji
	Vect getPlaneNormal() { return normal; }
	double getPlaneDistance() { return distance; }
	virtual Color getColor() { return color; }

	virtual Vect getNormalAt(Vect point) {
		return normal;
	}
	//inicjalizacja metody kt�ra sprawdza czy promie� przeci�� p�aszczyzn�
	virtual double findIntersection(Ray ray) {
		Vect ray_direction = ray.getRayDirection();

		double a = ray_direction.dotProduct(normal);

		if (a == 0) {
			//promie� r�wnoleg�y do p�aszczyzny
			return -1;
		}
		else {
			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative())); 
			return -1 * b / a;
		}
	}

};

Plane::Plane() {
	normal = Vect(1, 0, 0);
	distance = 0;
	color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(Vect normalValue, double distanceValue, Color colorValue) {
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif
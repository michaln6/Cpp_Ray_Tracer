#ifndef _OBJECT_H
#define _OBJECT_H
#include "Ray.h"
#include "Vect.h"
#include "Color.h"

//klasa abstrakcyjna po kt�rej metody dziedzicz� klasy Sphere oraz Plane
class Object {
	public:
	Object();

	//metody funkcji
	virtual Color getColor() { return Color(0.0, 0.0, 0.0, 0); }
	//inicjacja metody znajdowania wektora normalnego w punkcie
	virtual Vect getNormalAt(Vect intersection_position) {
		return Vect(0, 0, 0);
	}
	//inicjacja metody znajdowania przeci�� promieni oraz obiekt�w
	virtual double findIntersection(Ray ray) {
		return 0;
	}
};

Object::Object() {

}

#endif
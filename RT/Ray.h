#ifndef _RAY_H
#define _RAY_H
#include "Vect.h"

//klasa zajmuj¹ca siê promieniami
class Ray {
	//pocz¹tek oraz kierunek promienia
	Vect origin, direction;

public:
	Ray();

	Ray(Vect, Vect);

	//metody funkcji
	Vect getRayOrigin() { return origin; }
	Vect getRayDirection() { return direction; }
};

Ray::Ray() {
	origin = Vect(0,0,0);
	direction = Vect(1, 0, 0);
}

Ray::Ray(Vect o, Vect d) {
	origin = o;
	direction = d;
}

#endif
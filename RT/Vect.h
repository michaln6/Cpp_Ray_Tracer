#ifndef _VECT_H
#define _VECT_H
#include "math.h"

//klasa tworz¹ca wektory, a tak¿e ró¿ne operacje wektorowe
class Vect {
	double x, y, z;

	public:
	Vect();

	Vect(double, double, double);

	//metody funkcji
	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }
	
	//d³ugoœæ wektora
	double magnitude() {
		return sqrt((x * x) + (y * y) + (z * z));
	}

	//normalizacja wektora
	Vect normalize() {
		double magnitude = sqrt((x * x) + (y * y) + (z * z));
		return Vect(x / magnitude, y / magnitude, z / magnitude);
	}
	
	//negacja wektora
	Vect negative() {
		return Vect(-x, -y, -z);
	}

	//produkt kropkowy wektora
	double dotProduct(Vect v) {
		return x * v.getVectX() + y * v.getVectY() + z * v.getVectZ();
	}

	//produkt krzy¿owy wektora
	Vect crossProduct(Vect v) {
		return Vect(y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());

	}

	//dodawanie wektorów
	Vect vectAdd(Vect v) {
		return Vect(x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
	}

	//mno¿enie wektora przez skalar
	Vect vectMult(double scalar){
		return Vect(x*scalar, y*scalar, z*scalar);
	}

};

Vect::Vect() {
	x = 0;
	y = 0;
	z = 0;
}

Vect::Vect(double i, double j, double k) {
	x = i;
	y = j;
	z = k;
}

#endif
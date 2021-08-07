#ifndef _LIGHT_H
#define _LIGHT_H
#include "Vect.h"
#include "Color.h"
#include "Source.h"

//klasa odpowiadaj¹ca za Ÿród³o œwiat³a
class Light : public Source {
	//pozycja Ÿród³a
	Vect position;
	//kolor œwiat³a
	Color color;

public:
	Light();

	Light(Vect, Color);

	//metody funkcji
	virtual Vect getLightPosition() { return position; }
	virtual Color getLightColor() { return color; }
};

Light::Light() {
	position = Vect(0, 0, 0);
	color = Color(1.0, 1.0, 1.0, 0);
}

Light::Light(Vect p, Color c) {
	position = p;
	color = c;
}

#endif

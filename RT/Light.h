#ifndef _LIGHT_H
#define _LIGHT_H
#include "Vect.h"
#include "Color.h"
#include "Source.h"

//klasa odpowiadaj�ca za �r�d�o �wiat�a
class Light : public Source {
	//pozycja �r�d�a
	Vect position;
	//kolor �wiat�a
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

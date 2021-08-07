#ifndef _SOURCE_H
#define _SOURCE_H
#include "Vect.h"
#include "Color.h"
//Klasa dziêki której mo¿emy tworzyæ Ÿród³a œwiat³a
class Source {
public:

	Source();

	virtual Vect getLightPosition() {
		return Vect(0, 0, 0);
	}
	virtual Color getLightColor() { 
		return Color(1, 1, 1, 0);
	}
};

Source::Source() {

}

#endif
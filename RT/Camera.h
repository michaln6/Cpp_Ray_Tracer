#ifndef _CAMERA_H
#define _CAMERA_H
#include "Vect.h"
//klasa tworz¹ca kamerê

class Camera {
	//campos- pocz¹tkowa pozycja kamery, camdir pozwala nam nakierunkowanie "spojrzenia" na dany punkt, camright oraz camdown odpowiadaj¹ za nasze nachylenie 
	Vect campos, camdir, camright, camdown;

public:
	Camera();

	Camera(Vect, Vect, Vect, Vect);

	//metody funkcji
	Vect getCameraPosition() { return campos; }
	Vect getCameraDirection() { return camdir; }
	Vect getCameraRight() { return camright; }
	Vect getCameraDown() { return camdown; }
};

Camera::Camera() {
	campos = Vect(0, 0, 0);
	camdir = Vect(0, 0, 1);
	camright = Vect(0, 0, 0);
	camdown = Vect(0, 0, 0);
}

Camera::Camera(Vect pos, Vect dir, Vect right, Vect down) {
	campos = pos;
	camdir = dir;
	camright = right;
	camdown = down;
}

#endif

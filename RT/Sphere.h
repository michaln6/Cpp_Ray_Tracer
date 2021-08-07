#ifndef _SPHERE_H
#define _SPHERE_H
#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

//klasa do stworzenia obiektu kuli
class Sphere : public Object {
	//center to wspó³rzêdne œrodka kuli
	Vect center;
	//radius to promieñ
	double radius;
	//color to kolor
	Color color;

public:
	Sphere();

	Sphere(Vect, double, Color);

	//metody funkcji
	Vect getSphereCenter() { return center; }
	double getSphereRadius() { return radius; }
	virtual Color getColor() { return color; }
	//normalizacja wektora, wskazuje zwrot wektora 
	virtual Vect getNormalAt(Vect point) {
		//normal zawsze wskazuje OD œrodka kuli
		Vect normal_Vect = point.vectAdd(center.negative()).normalize();
		return normal_Vect;
	}
	//funkcja szukaj¹ca przeciêæ promienia z kul¹
	virtual double findIntersection(Ray ray) {
		Vect ray_origin = ray.getRayOrigin();
		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		Vect ray_direction = ray.getRayDirection();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();

		Vect sphere_center = center;
		double sphere_center_x = sphere_center.getVectX();
		double sphere_center_y = sphere_center.getVectY();
		double sphere_center_z = sphere_center.getVectZ();
	
		double a = 1; //znormalizowane
		double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x)+ (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) + (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
		double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);

		double discriminant = b * b - 4 * c;

		if (discriminant > 0) {
			//promieñ przecina kulê

			double root_1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.0000001;

			if (root_1 > 0) {
				//pierwszy pierwiastek jest najmniejszym dodatnim pierwiastkiem
				return root_1;
			}
			else {
				//drugi pierwiastek jest najmniejszym dodatnim pierwiastkiem
				double root_2 = ((sqrt(discriminant) - b) / 2) - 0.0000001;
				return root_2;
			}
		}
		else {
			//promieñ min¹³ kulê
			return -1;
		}
	}
};

Sphere::Sphere() {
	center = Vect(0, 0, 0);
	radius = 1;
	color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(Vect centerValue, double radiusValue, Color colorValue) {
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
}

#endif
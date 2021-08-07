#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Source.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h" 

//PROGRAM RENDERUJE OŚWIETLONĄ KULĘ NA SZACHOWNICY, TWORZY OBRAZ W FORMACIE BMP W MIEJSCU KOMPILACJI
using namespace std;
struct RGBType {
	double r;
	double g;
	double b;
};

// Funkcja zapisująca plik .bmp 
void savebmp(const char* filename, int w, int h, int dpi, RGBType* data)
{
	FILE *f;
	int k = w * h;
	int s = 4 * k;
	int filesize = 54 + s;
	
	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi * m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);

	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(filename, "wb");
	
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	for (int i = 0; i < k; i++)
	{
		RGBType rgb = data[i];

		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;

		char color[3] = { (char)floor(blue),(char)floor(green),(char)floor(red) };

		fwrite(color, 1, 3, f);
	}

	fclose(f);
}

int winningObjectIndex(vector<double> object_intersections) {
	//zwraca index obiektu bliżej kamery
	int index_of_minimum_value;

	//pozbywa się niepotrzebnych kalkulacji
	if (object_intersections.size() == 0) {
		//nie ma przecięć
		return -1;
	}
	else if (object_intersections.size()== 1) {
		//jedno przecięcie
		if (object_intersections.at(0) > 0) {
			//jeżeli wartość tego przecięcia jest większa niż zero, to nasz indeks przecięcia
			return 0;
		}
		else {
			//jeżeli nie, brak przecięć w tym punkcie
			return -1;
		}
	}
	else {
		//przypadek gdy jest więcej niż jedno przecięcie
		//szukamy wartości maksymalnej
		double max = 0;
		for (int i = 0; i < object_intersections.size(); i++) {
			if (max < object_intersections.at(i)) {
				max = object_intersections.at(i);
			}
		}

		//zaczynając od wartości maksymalnej szukamy minimalnej pozytywnej wartości
		if (max > 0) {
			//we only want positive inters.
			for (int index = 0; index < object_intersections.size(); index++) {
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}

			return index_of_minimum_value;
		}
		else {
			//wszystkie przecięcia są negatywne
			return -1;
		}
	}
}
//podaje nam informacje o kolorze danego piksela 
Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight) {
	
	Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);
	
	//jeżeli wartość special obiektu wynosi 2, tworzymy na nim wzór szaachownicy
	if (winning_object_color.getColorSpecial() == 2) {

		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());
		
		if ((square % 2) == 0) {
			winning_object_color.setColorRed(0);
			winning_object_color.setColorGreen(0);
			winning_object_color.setColorBlue(0);
		}
		else {
			winning_object_color.setColorRed(1);
			winning_object_color.setColorRed(1);
			winning_object_color.setColorRed(1);
		}
	}

	Color final_color = winning_object_color.colorScalar(ambientlight);
	//obiekty, których wartość special jest pomiędzy 0 a 1 odbijają światło z natężeniem zależnym od wielkości tej zmiennej
	if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
		double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
		Vect scalar1 = winning_object_normal.vectMult(dot1);
		Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
		Vect scalar2 = add1.vectMult(2);
		Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
		Vect reflection_direction = add2.normalize();

		Ray reflection_ray(intersection_position, reflection_direction);

		//sprawdzamy z czym promień styka się najpierw
		vector<double> reflection_intersections;

		for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
			reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
		}
		int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

		if (index_of_winning_object_with_reflection != -1) {
			//promień odbicia minął wszystko inne
			if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
				//ustalamy pozycję oraz kierunek w punkcie przecięcia z promieniem odbicia
				//promień wpływa na kolor tylko wtedy jeżeli się od czegoś odbija

				Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vect reflection_intersection_ray_direction = reflection_direction;

				Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);

				final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));

			}
		}
	}
	//pętla sprawdzająca cieniowanie
	for (int light_index = 0; light_index < light_sources.size(); light_index++) {
		Vect light_direction = light_sources.at(light_index) -> getLightPosition().vectAdd(intersection_position.negative()).normalize();

		float cosine_angle = winning_object_normal.dotProduct(light_direction);

		if (cosine_angle > 0) {
			//sprawdzamy co powinno być zacieniowane
			bool shadowed = false;

			Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative());
			double distance_to_light_magnitude = distance_to_light.magnitude();
			
			Ray shadow_ray(intersection_position, light_sources.at(light_index) -> getLightPosition().vectAdd(intersection_position.negative()).normalize());
			
			vector<double> secondary_intersections;

			for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++)
			{
				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
			}
			//tutaj już samo cieniowanie po sprawdzeniu w którym miejscu cienie powinny wystąpić
			for (int c = 0; c < secondary_intersections.size(); c++) {
				if (secondary_intersections.at(c) > accuracy) {
					if (secondary_intersections.at(c) <= distance_to_light_magnitude)
					{
						shadowed = true;
					}
				}
				break;
			}
			if (shadowed == false) {
				//zmiana kolorów oraz połysku w miejscach, które nie są zacieniowane
				final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
					double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
					Vect scalar1 = winning_object_normal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
					Vect scalar2 = add1.vectMult(2);
					Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();

					double specular = reflection_direction.dotProduct(light_direction);
					if (specular > 0) {
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular * winning_object_color.getColorSpecial()));
					}
				}
			}

		}
	}
	//zwracamy finalny kolor piksela oraz go "zmiękczamy" aby nie powstały odcięcia kolorów przy oświetleniu
	return final_color.clip();
}


int main(int argc, char* argv[]) {
	cout << "Renderowanie (w zaleznosci od ustawien moze to zajac od kilku sekund do kilku minut)..." << endl;

	//te zmienne wykorzystujemy do obliczenia czasu renderowania
	clock_t t1, t2;
	t1 = clock();

	//ustawienia renderowanego obrazu
	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width * height;
	RGBType* pixels = new RGBType[n];

	//Dla szybkości renderowania polecam rozdzielczość 640x480p oraz aadepth na poziomie 1
	//Lepszy wygląd renderu zapewniają ustawienia 1920x1080p, aadepth = 4

	//ustawienia anti-alliasingu, aadepth odpowiada za ilość promieni na piksel (1:4), aathreshold to prawdopodobieństwo NIE wykonania anti-alliasingu na danym pikselu, accuracy poamaga nam w wielu obliczeniach
	int aadepth = 1;
	double aathreshold = 0.1;
	double aspectratio = (double)width / (double)height;
	double ambientlight = 0.2;
	double accuracy = 0.00001;

	//tworzymy początkowe wektory
	Vect O(0, 0, 0);
	Vect X(1, 0, 0);
	Vect Y(0, 1, 0);
	Vect Z(0, 0, 1);

	//pozycja kamery
	Vect campos(3, 1.5, -4);

	//ustawienie kamery na punkt (0,0,0)
	Vect look_at(0, 0, 0);
	Vect diff_btw(campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());

	Vect camdir = diff_btw.negative().normalize();
	Vect camright = Y.crossProduct(camdir).normalize();
	Vect camdown = camright.crossProduct(camdir);
	Camera scene_cam(campos, camdir, camright, camdown);

	Color white_light(1, 1, 1, 0);
	Color black(1, 1, 1, 2);
	Color orange(1, 0.67, 0, 0.3);

	Vect light_position(-7, 10, -10);
	Light scene_light(light_position, white_light);
	vector<Source*> light_sources;
	//dodajemy oświetlenie
	light_sources.push_back(dynamic_cast<Source*>(&scene_light));

	//dodajemy obiekty sceny
	Sphere scene_sphere(O, 1, orange);
	Plane scene_plane(Y, -1, black);
	vector<Object*> scene_objects;
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));

	int thisone, aa_index;
	double xamnt, yamnt;
	double tempRed, tempGreen, tempBlue;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			thisone = y * width + x;

			//zaczynamy z pustym pikselem
			double * tempRed = new double [aadepth*aadepth];
			double * tempGreen = new double [aadepth*aadepth];
			double * tempBlue = new double [aadepth*aadepth];


			for (int aax = 0; aax < aadepth; aax++) {
				for (int aay = 0; aay < aadepth; aay++) {

					aa_index = aay * aadepth + aax;

					srand(time(0));

					//tworzymy promień z kamery na piksel
					if (aadepth == 1) {
						//sytuacja bez anti-alliasingu
						if (width > height) {
							//obraz szerszy niż wyższy
							xamnt = ((x + 0.5) / width) * aspectratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + 0.5) / height;
						}

						else if (height > width) {
							//obraz wyższy niż szerszy
							xamnt = (x + 0.5) / width;
							yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else {
							//kwadrat
							xamnt = (x + 0.5) / width;
							yamnt = (((double)height - y) + 0.5) / height;
						}
					}
					else {
						// anti-alliasing
						if (width > height) {
							//obraz szerszy niż wyższy
							xamnt = ((x + (double)aax / ((double)aadepth - 1)) / width) * aspectratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + (double)aax / ((double)aadepth - 1)) / height;
						}

						else if (height > width) {
							//obraz wyższy niż szerszy
							xamnt = (x + (double)aax / ((double)aadepth - 1)) / width;
							yamnt = (((height - y) + (double)aax / ((double)aadepth - 1)) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else {
							//kwadrat
							xamnt = (x + (double)aax / ((double)aadepth - 1)) / width;
							yamnt = ((height - y) + (double)aax / ((double)aadepth - 1)) / height;
						}
					}

					Vect cam_ray_origin = scene_cam.getCameraPosition();
					Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();


					Ray cam_ray(cam_ray_origin, cam_ray_direction);

					vector<double> intersections;

					for (int index = 0; index < scene_objects.size(); index++) {
						intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
					}

					int index_of_winning_object = winningObjectIndex(intersections);

					if (index_of_winning_object == -1) {
						// tło czarne
						tempRed[aa_index] = 0;
						tempGreen[aa_index] = 0;
						tempBlue[aa_index] = 0;
					}
					else {
						// indeks odpowiada obiektowi na scenie
						if (intersections.at(index_of_winning_object) > accuracy) {
							//sprawdzamy pozycję i kierunek wektoru w punkcie przecięcia

							Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
							Vect intersecting_ray_direction = cam_ray_direction;

							Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight);

							tempRed[aa_index] = intersection_color.getColorRed();
							tempGreen[aa_index] = intersection_color.getColorGreen();
							tempBlue[aa_index] = intersection_color.getColorBlue();
						}
					}
				}
			}

			//uśredniamy kolor piksela
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aadepth * aadepth; iRed++) {
				totalRed = totalRed + tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth * aadepth; iGreen++) {
				totalGreen = totalGreen + tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth * aadepth; iBlue++) {
				totalBlue = totalBlue + tempBlue[iBlue];
			}
			double avgRed = totalRed / (aadepth * aadepth);
			double avgGreen = totalGreen / (aadepth * aadepth);
			double avgBlue = totalBlue / (aadepth * aadepth);

			pixels[thisone].r = avgRed;
			pixels[thisone].g = avgGreen;
			pixels[thisone].b = avgBlue;
		}
	}

	savebmp("Scena.bmp", width, height, dpi, pixels);

	delete pixels, tempRed, tempGreen, tempBlue;
	
	t2 = clock();
	float diff = ((float)t2 - (float)t1) / 1000;

	cout << "Renderowanie zakonczone, zajelo " << diff << " sekund." << endl;

	return 0;
}

#include <fstream>
#include <iostream>
#include "CImg.h"
#include <vector>
using namespace cimg_library;
using namespace std;
#include <math.h>       /* sin */
#include "Source.h"
#include <random>
#include <ctime>
#define PI 3.14159265
#include <string>




/*
 Classe plaque:

 Dans cette classe nous avons définie une plaque comme un rectangle.
		   Les grandeurs : epaisseur, longeur, distance sont générés de telle facons a avoir une valeur aléatoire entre deux extremes bien définis
*/
class Plaque{
	int epaisseur;
	int longeur;
	int distance;
public :
	Plaque::Plaque() {
		distance = 6;
		srand(time(0));
		int varE = rand() % 30;
		epaisseur = 10 + varE;

		srand(time(0));
		int varL = rand() % 50;
		longeur = 50 + varL;

	};
	Plaque::Plaque(int e, int l,int d) {
		epaisseur = e ; longeur = l; this->distance = d;
	};
	int Plaque::getE() { return this->epaisseur; };
	int Plaque::getL() { return this->longeur; };
	int Plaque::getD() { return this->distance; };
	void Plaque::setE(int e) {this->epaisseur=e; };
	void Plaque::setL(int l) { this->longeur = l; };
	void Plaque::setD(int d) { this->distance = d; };
};



/* 
Classe pour définir un Vaisseau
Définit comme un cercle de centre aléatoir et rayon aléatoire entre des extremes définis.

*/

class Vessel {
	int centreX; int centreY;
	double rayon;
public:
	Vessel::Vessel() {
		int rangerayon = 20;
		srand(time(0));
		 rayon = 50+rand() % rangerayon;
		int range = rayon/2;
		//Generat negative and positive Sign
		srand(time(0));
		int SignX = (rand() % 2) - 1; // number between -1 and 1
		if (SignX == 0) {
			SignX = 1;
		}
		srand(time(0));
		 centreX = 128 + SignX * rand() % range;

		 srand(time(0));
		 int SignY = (rand()*rand() % 2) - 1; // number between -1 and 1
		 if (SignY == 0) {
			 SignY = 1;
		 }
		
		//cout << "SignY : " << SignY << " SignX : " << SignX << endl;
		srand(time(0));
		 centreY = 128+ SignY * rand() % range;
		//centreX =128; centreY = 150; rayon = 50;
	};
	Vessel::Vessel(int x, int y, double r) {
		centreX = x; centreY = y; rayon = r;
	};
	int Vessel::getCenterX() { return this->centreX; };
	int Vessel::getCenterY() { return this->centreY; };
	double Vessel::getRayon() { return this->rayon; };
	void Vessel::setCenterX(int x) { this->centreX = x; };
	void Vessel::setCenterY(int y) { this->centreY = y; };
	void Vessel::setRayon(int d) { this->rayon = d; };
};

//Fonction qui return la distance entre le centre de la sonde et le centre de l'image :
double lenOfR(Point2D Icentre, Point2D Ppoint) {
	Icentre.setX(127); Icentre.setY(127);
	double distance = 0;
	distance = sqrt((Ppoint.getX() - Icentre.getX()) * (Ppoint.getX() - Icentre.getX()) + (Ppoint.getY() - Icentre.getY()) * (Ppoint.getY() - Icentre.getY()));
	return distance;

}


//Fonction qui return un vecteur contnenant les pixels (Point2D) existant sur la lumiere du vaisseau  :

/*
Cette fonction return un tableau contient des points.
Ces points vont etre utliser pour passer du cartésien vers le polaire
Algorithme de Bresenham entre le centre de la sonde et ces points.
*/
vector<Point2D> secondPoints(Vessel vessel) {
	vector<Point2D> BPoints;
	int x = vessel.getCenterX();
	int y = vessel.getCenterY();
	int r = vessel.getRayon();
	for (int i = 0; i < 255; i++) {
		for (int j = 0; j < 255; j++) {
			if (((i - x)*(i - x) + (j - y)*(j - y) <= r*r + 100) && ((i - x)*(i - x) + (j - y)*(j - y) >= r*r - 100)) {
				if (i >= vessel.getCenterX()) {
					if (j >= vessel.getCenterY()) { BPoints.insert(BPoints.begin(), Point2D(i, j)); }
					else { BPoints.insert(BPoints.end(), Point2D(i, j)); }
				}
				else {
					if (j >= vessel.getCenterY()) { BPoints.insert(BPoints.begin(), Point2D(i, j)); }
					else {
						BPoints.insert(BPoints.end(), Point2D(i, j));
					}
				}
			}
		}
	}
	return BPoints;
}





// Une fonction qui ajoute un bruit gamma sur notre image
void ImageWithGamaNoise(CImg<unsigned char> img, CImg<unsigned char> &Destimg) {
	std::default_random_engine generator;
	int moy = 0;
	cimg_for2XY(img, x, y) {
		int pixel_value = img(x, y)+1;
		double beta = 5, alpha = pixel_value / beta;
		std::gamma_distribution<double> distribution(alpha, beta);
		double number = distribution(generator);
		moy = moy + number;
		Destimg(x, y) = number;
	}
	moy = moy / (255 * 255);
}





//I - Plaque Fibrotique
//Fonction qui génere une image avec plaque fibrotique de dimensions aléatoires sur un vaisseau.
void Plaque_Fibrotique(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel) {
	std::default_random_engine generator;

	int epaisseur = plaque.getE();
	int d = plaque.getD();
	int l = plaque.getL();
	int range = (255 - l) + 1;
	srand(time(0));
	int xpoint = rand() % range;
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);
	//bindex repesente l'axe des x ou theta;
	for (int bindex = xpoint*BPoints.size() / 255; bindex < (xpoint + l)*BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		//r c'est l'axe de y dans mon image.
		double r = lenOfR(Icentre, pointB);
		for (int j = r + d; j < r + d + epaisseur; j++) {	
			double pixel_value = 200;
			double beta = 2, alpha = pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			double number = distribution(generator);
			//if (number > 255) number = 255;
			Destimg(bindex * 255 / BPoints.size(), j) = number;

		}
	}
}

//Fonction qui génere 3 images (image avec plaque fibrotique, le masque de cette plaque et la lumiere) de dimensions aléatoires sur un vaisseau.
void Plaque_Fibrotique(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel, CImg<unsigned char> &MaskImage, CImg<unsigned char>&LumImage) {
	std::default_random_engine generator;

	int epaisseur = plaque.getE();
	int d = plaque.getD();
	int l = plaque.getL();
	int range = (255 - l) + 1;
	srand(time(0));
	int xpoint = rand() % range;
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);

	for (int bindex = 0 * BPoints.size() / 255; bindex < 255 * BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		double r = lenOfR(Icentre, pointB);

		for (int j = r; j < 255; j++) {
			LumImage(bindex * 255 / BPoints.size(), j) = 255;
		}

	}
	//bindex repesente l'axe des x ou theta;
	for (int bindex = xpoint*BPoints.size() / 255; bindex < (xpoint + l)*BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		//r c'est l'axe de y dans mon image.
		double r = lenOfR(Icentre, pointB);
		for (int j = r + d; j < r + d + epaisseur; j++) {
			double pixel_value = 200;
			double beta = 2, alpha = pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			double number = distribution(generator);
			//if (number > 255) number = 255;
			Destimg(bindex * 255 / BPoints.size(), j) = number;
			MaskImage(bindex * 255 / BPoints.size(), j) = 255;
			LumImage(bindex * 255 / BPoints.size(), j) = 0;
		}
	}
}


//II - Plaque Calcifiée 

void Plaque_Calcifiee(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel) {
	std::default_random_engine generator;

	int epaisseur = plaque.getE();
	int d = plaque.getD();
	int l = plaque.getL();
	int range = (255 - l) + 1;
	srand(time(0));
	int xpoint = rand() % range;
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);
	//bindex repesente l'axe des x ou theta;


	for (int bindex = xpoint*BPoints.size() / 255; bindex < (xpoint + l)*BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		//r c'est l'axe de y dans mon image.
		double r = lenOfR(Icentre, pointB);
		//Destimg(bindex * 255 / BPoints.size(), r) = 255;
		for (int j = r + d; j < r + d + epaisseur; j++) {
			//II - Plaque Calcifiée  		
			double pixel_value = 80;
			double beta = 15, alpha = pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			//std::gamma_distribution<double> distribution(1, pixel_value);
			double number = distribution(generator);
			Destimg(bindex * 255 / BPoints.size(), j) = number;
		}
	}
}

//Fonction qui génere 3 images (image avec plaque Calcifiee, le masque de cette plaque et la lumiere) de dimensions aléatoires sur un vaisseau.
void Plaque_Calcifiee(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel, CImg<unsigned char> &MaskImage, CImg<unsigned char> &LumImage) {
	std::default_random_engine generator;

	int epaisseur = plaque.getE();
	int d = plaque.getD();
	int l = plaque.getL();
	int range = (255 - l) + 1;
	srand(time(0));
	int xpoint = rand() % range;
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);
	//bindex repesente l'axe des x ou theta;


	for (int bindex = 0 * BPoints.size() / 255; bindex < 255 * BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		double r = lenOfR(Icentre, pointB);

		for (int j = r; j < 255; j++) {
			LumImage(bindex * 255 / BPoints.size(), j) = 255;
		}

	}


	for (int bindex = xpoint*BPoints.size() / 255; bindex < (xpoint + l)*BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		//r c'est l'axe de y dans mon image.
		double r = lenOfR(Icentre, pointB);
		//Destimg(bindex * 255 / BPoints.size(), r) = 255;
		for (int j = r + d; j < r + d + epaisseur; j++) {
			//II - Plaque Calcifiée  		
			double pixel_value = 80;
			double beta = 15, alpha = pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			//std::gamma_distribution<double> distribution(1, pixel_value);
			double number = distribution(generator);
			Destimg(bindex * 255 / BPoints.size(), j) = number;
			MaskImage(bindex * 255 / BPoints.size(), j) = 255;
			LumImage(bindex * 255 / BPoints.size(), j) = 0;
		}
	}
}


void Plaque_Lipid(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel) {
	std::default_random_engine generator;

	int epaisseur = plaque.getE();
	int d = plaque.getD();
	int l = plaque.getL();
	int range = (255 - l) + 1;
	srand(time(0));
	int xpoint = rand() % range;
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);
	//bindex repesente l'axe des x ou theta;


	for (int bindex = xpoint*BPoints.size() / 255; bindex < (xpoint + l)*BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		double r = lenOfR(Icentre, pointB);
		for (int j = r + d; j < r + d + epaisseur; j++) {

			//III - Lipidique
			double pixel_value = 255 * exp(-5 * (j - 1 - r) / 34);

			double beta = 20, alpha = pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			double number = distribution(generator);
			Destimg(bindex * 255 / BPoints.size(), j) = number;
		}
		//This Part is only for Lipidique plaque !

		//To get the last value of the plaque
		double InitialValue = 255 * exp(-5 * (r + d + epaisseur - 1 - 1 - r) / 34);
		for (int j = int(r + d + epaisseur); j < 255; j++) {
			double final_pixel_value = InitialValue * exp(-1 * (j - 1 - r) / 100);
			double beta = 3, alpha = final_pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			double number = distribution(generator);
			Destimg(bindex * 255 / BPoints.size(), j) = number;
		}
	}

}
void Plaque_Lipid(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel, CImg<unsigned char> &MaskImage, CImg<unsigned char> &LumImage) {
	std::default_random_engine generator;

	int epaisseur = plaque.getE();
	int d = plaque.getD();
	int l = plaque.getL();
	int range = (255 - l) + 1;
	srand(time(0));
	int xpoint = rand() % range;
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);
	//bindex repesente l'axe des x ou theta;

	for (int bindex = 0 * BPoints.size() / 255; bindex < 255 * BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		double r = lenOfR(Icentre, pointB);

		for (int j = r; j < 255; j++) {
			LumImage(bindex * 255 / BPoints.size(), j) = 255;
		}

	}
	for (int bindex = xpoint*BPoints.size() / 255; bindex < (xpoint + l)*BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		double r = lenOfR(Icentre, pointB);
		for (int j = r + d; j < r + d + epaisseur; j++) {

			//III - Lipidique
			double pixel_value = 255 * exp(-5 * (j - 1 - r) / 34);

			double beta = 20, alpha = pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			double number = distribution(generator);
			Destimg(bindex * 255 / BPoints.size(), j) = number;
			MaskImage(bindex * 255 / BPoints.size(), j) = 255;
			LumImage(bindex * 255 / BPoints.size(), j) = 0;
		}
		//This Part is only for Lipidique plaque !

		//To get the last value of the plaque
		double InitialValue = 255 * exp(-5 * (r + d + epaisseur - 1 - 1 - r) / 34);
		for (int j = int(r + d + epaisseur); j < 255; j++) {
			double final_pixel_value = InitialValue * exp(-1 * (j - 1 - r) / 100);
			double beta = 3, alpha = final_pixel_value / beta;
			std::gamma_distribution<double> distribution(alpha, beta);
			double number = distribution(generator);
			Destimg(bindex * 255 / BPoints.size(), j) = number;
			
		}
	}

}

void Lisse_Lipid_Plaque(CImg<unsigned char> img,CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel) {
	std::default_random_engine generator;

	int epaisseur = plaque.getE();
	int d = plaque.getD();
	int l = plaque.getL();
	int range = (255 - l) + 1;
	srand(time(0));
	int xpoint = rand() % range;
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);
	
	//bindex repesente l'axe des x ou theta;
	for (int bindex =0*BPoints.size() / 255; bindex < 255*BPoints.size() / 255; bindex++) {
		Point2D pointB = BPoints[bindex];
		Point2D Icentre(127, 127);
		//r c'est l'axe de y dans mon image.
		double r = lenOfR(Icentre, pointB);	 
		for (int j = int(r + d+epaisseur/2); j < 255; j++) {
			double param, result;
			int Start = xpoint;
			int End = (Start + l - 1);
			int Max = img(Start - 10, j);
			int Min = img(Start , j);
			int inc = int(bindex * 255 / BPoints.size());
			if (inc<Start-1 & inc>(Start - 10)) {
				param = 1.1 - 0.08*(inc-Start+10);
				int value = img(inc, j);
				result =erf(param)*value ;
				Destimg(inc, j) = result;
			}	
			else if (inc<End+10 & inc>End) {
				param = 0.3 + 0.08*(inc - End+1);
				int value = img(inc, j);
				result = erf(param)*value;
				Destimg(inc, j) = result;
			}
		}		   
	}
}

void PlaqueLipidique(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel) {
	CImg<unsigned char> JustImgPlaque(256, 256);
	JustImgPlaque.fill(0);
	JustImgPlaque = Destimg;
	Plaque_Lipid(JustImgPlaque,  plaque,  vessel);
	Destimg = JustImgPlaque;
	//Plaque_Image(JustImgPlaque, plaque, vessel);
	//Plaque_Lipid(JustImgPlaque, plaque, vessel);
	Lisse_Lipid_Plaque(JustImgPlaque, Destimg, plaque, vessel);
}


void PlaqueLipidique(CImg<unsigned char> &Destimg, Plaque plaque, Vessel vessel, CImg<unsigned char> &MaskImage, CImg<unsigned char> &LumImage) {
	CImg<unsigned char> JustImgPlaque(256, 256);
	JustImgPlaque.fill(0);
	JustImgPlaque = Destimg;
	Plaque_Lipid(JustImgPlaque, plaque, vessel, MaskImage, LumImage);
	Destimg = JustImgPlaque;
	//Plaque_Image(JustImgPlaque, plaque, vessel);
	//Plaque_Lipid(JustImgPlaque, plaque, vessel);
	Lisse_Lipid_Plaque(JustImgPlaque, Destimg, plaque, vessel);
}


void   FlipImageYaxix(CImg<unsigned char> InputImage, CImg<unsigned char> &Destimg) {
	double r = 0; double theta = 0;
	cimg_for2XY(InputImage, i, j) {
		Destimg(i, 255 - j) = InputImage(i,j);
	}
}



// Fonction qui construit l'image de base dans le domaine polaire a partir d' un vaisseau
void DirectImage(Vessel vessel, CImg<unsigned char> &Destimg) {
	//get all points which are in the line (defined by theta)
	vector<Point2D> BPoints = secondPoints(vessel);
	//cout << " taille :" << BPoints.size() << endl;
	double rayon=0;
		//double theta = 0;
		for (int bindex = 0.0; bindex < BPoints.size(); bindex++) {
			Point2D pointB = BPoints[bindex];
			Point2D Icentre(127,127);
			//r c'est l'axe de y dans mon image.
			double r = lenOfR(Icentre, pointB);
			Destimg(bindex * 255/ BPoints.size(), r) = 180;
			for (int j = r+1; j < 255; j++) {
				//double pixel_value = 0;
				double pixel_value = 180 * exp(-1*(j-1-r)/34);
				//cout << " Values " << exp(-1 * (j - 1 - r) / 34) << endl;
				if (pixel_value >= 0) {
					Destimg(bindex * 255 / BPoints.size(), j) = pixel_value;
				}
				else {
					Destimg(bindex * 255 / BPoints.size(), j) = 0;
				}
			}
			 rayon = rayon + r;
		}
}



int main()
{
	
	CImg<unsigned char> imgLine(256, 256);
	imgLine.fill(0);
	CImg<unsigned char> imgResult(256, 256);
	imgResult.fill(255);
	CImg<unsigned char> finalImage(256, 256);
	finalImage.fill(255);
	//DrawLight(imgLine, imgLine,128);
	addGamaNoise(imgResult, imgResult);
	CImgList<unsigned char> List;
	CImg<unsigned char> directimage(256, 256);
	imgLine.fill(0);
	Vessel vessel;
	DirectImage(vessel, directimage);
	cimg_for2XY(directimage, i, j) {
		directimage(i,j)=255- directimage(i, j);
	}
	//CImgDisplay disp(directimage, "Image_direct"); while (!disp.is_closed) disp.wait();
	



	//Ici juste pour enregitrer les images dans le disque dur.


		/*
	string root = "C:/Users/youne/OneDrive/Documents/Visual Studio 2015/Projects/Simulation/Simulation/Images/SegNetData/Lumiere_without_plaque/100F/PlaqueFt";
	string ext = ".bmp";
	string rootMasque = "C:/Users/youne/OneDrive/Documents/Visual Studio 2015/Projects/Simulation/Simulation/Images/SegNetData/Lumiere_without_plaque/100F_mask/MaskFt";
	string rootLumiere = "C:/Users/youne/OneDrive/Documents/Visual Studio 2015/Projects/Simulation/Simulation/Images/SegNetData/Lumiere_without_plaque/LumiereL/LumiereLt";
	for (int i = 0; i < 100; i++) {
		CImg<unsigned char> MaskImage(256, 256);
		MaskImage.fill(0);			

		CImg<unsigned char> LumiereImage(256, 256);
		LumiereImage.fill(0);
		//Draw Vessel Image
		Vessel vessel;
		CImg<unsigned char> MEImage(256, 256);
		MEImage.fill(0);
		DirectImage(vessel, MEImage);
		CImg<unsigned char> ImageNonBruite(256, 256);
		ImageNonBruite = MEImage;
		ImageWithGamaNoise(ImageNonBruite, imgResult);
		//Add plaque aleatoire qui suit la lumiere.
		Plaque plaque;

		//Image of plaque :
		CImg<unsigned char> JustImgPlaque(256, 256);
		JustImgPlaque.fill(0);
		JustImgPlaque = imgResult;
		//Plaque_Image(JustImgPlaque, plaque, vessel);
		Plaque_Fibrotique(JustImgPlaque, plaque, vessel, MaskImage, LumiereImage);
		//List.assign(ImageNonBruite, imgResult, JustImgPlaque);
		JustImgPlaque.normalize(0, 255);
		MaskImage.normalize(0, 255);
		LumiereImage.normalize(0, 255);
		//Enregistrer l'image de l'OCT dans le dossier
		string chemin;
		chemin.append(root);
		chemin.append(to_string(i));
		chemin.append(ext);

		std::vector<char> writable(chemin.begin(), chemin.end());
		writable.push_back('\0');

		//Enregistrer l'image du masque dans le dossier
		string cheminMasque;
		cheminMasque.append(rootMasque);
		cheminMasque.append(to_string(i));
		cheminMasque.append(ext);

		std::vector<char> writableMasque(cheminMasque.begin(), cheminMasque.end());
		writableMasque.push_back('\0');
									 // don't forget to free the string after finished using it
		

		JustImgPlaque.save(&*writable.begin());
		MaskImage.save(&*writableMasque.begin());


	
		string cheminLumiere;
		cheminLumiere.append(rootLumiere);
		cheminLumiere.append(to_string(i));
		cheminLumiere.append(ext);

		std::vector<char> writableLumiere(cheminLumiere.begin(), cheminLumiere.end());
		writableLumiere.push_back('\0');
		// don't forget to free the string after finished using it
		//LumiereImage.save(&*writableLumiere.begin());
		
		//JustImgPlaque.save(root +string(i)+ext);
		//delete[] writable;
	 }

	  	*/
   
	CImg<unsigned char> MaskImage(256, 256);
	MaskImage.fill(0);
	//Draw Vessel Image
	//	Vessel vessel;
	CImg<unsigned char> MEImage(256, 256);
	MEImage.fill(0);
	DirectImage(vessel, MEImage);
	CImg<unsigned char> ImageNonBruite(256, 256);
	ImageNonBruite = MEImage;
	ImageWithGamaNoise(ImageNonBruite, imgResult);
	//Add plaque aleatoire qui suit la lumiere.
	Plaque plaque;

	//Image of plaque :
	CImg<unsigned char> JustImgPlaque(256, 256);
	JustImgPlaque.fill(0);
	JustImgPlaque = imgResult;
	//Plaque_Image(JustImgPlaque, plaque, vessel);
	//PlaqueLipidique(JustImgPlaque, plaque, vessel, MaskImage);
	List.assign(ImageNonBruite, imgResult, JustImgPlaque);
	
	CImgDisplay disp(List, "Image Without Noise || image avec bruit gamma || Image Plaque"); while (!disp.is_closed) disp.wait();
					 
	
	return 0;
}


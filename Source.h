#include <fstream>
#include <iostream>
#include "CImg.h"
#include <vector>
using namespace cimg_library;
using namespace std;

#include <iostream>

/*
Dear Teacher feel free to run the program but Please it is not working when y=a or x=a;
Thank you :D !

*/


//Creation of Point2D and Vec2D Classes in order to work easily

/*
Base class Point2D represents a point in two dimensional space
*/
class Point2D {

public:

	/*
	Static data members have to be initialized when declared.
	Static data memebers are usually declared const (constant),
	because the values should not be changed (Principle of Least Privilege)
	*/
	static const int ZERO = 0;

	/*
	Overloaded constructor with zero parameters
	*/
	Point2D() {
		//initialize the data members
		x = ZERO;
		y = ZERO;
		//cout<<*this<<endl;
	}

	/*
	Overloaded constructor with two parameters
	*/
	Point2D(int x2, int y2) {
		//initialize the data members
		x = x2;
		y = y2;
		//cout<<*this<<endl;
	}

	//set (mutator) functions
	void setX(int x2) {
		x = x2;
	}

	void setY(int y2) {
		y = y2;
	}

	//get (accessor) functions   
	int getX() {
		return x;

	}

	int getY() {
		return y;
	}

	/*
	Returns the distance from point (0,0).
	Can be the magnitude of a vector or length of a line.
	*/
	double distance() {
		return sqrt(x * x + y * y);
	}
	void display() {
		cout << "Point(" << this->x << "," << this->y << ")" << endl;
	}


	//data members represent two points in space
protected:
	int x;
	int y;
};


CImg<unsigned short> DressImage(CImg<unsigned short> img, vector<Point2D> Points, vector<int> Gris) {
	int k = 0;
	for (auto i = Points.begin(); i != Points.end(); ++i) {
		Point2D point = *i;
		for (int x = 0; x < img.dimx(); x++) {
			for (int y = 0; y < img.dimy(); y++)
			{
				if (x == point.getX() & y == point.getY()) {
					img(x, y) = Gris[k];
				}
			}
		}
		k++;
	}
	return	 img;

}
vector<int> getPixels_of_theta(CImg<unsigned char> img,Point2D A,Point2D B) {
	vector<int> NiveauDeGris;	  //Vector to store gris level of each pixel of the line
	vector<Point2D> ColoredPoint;
	if (B.getY() >= A.getY()) {
		int dx = B.getX() - A.getX();
		int dy = B.getY() - A.getY();
		if (dx >= dy) {
			//Looking in the right of our point A
			//cout << "here I-1 dx >= dy" << std::endl;
			//cout << "m <= 1" << std::endl;
			int dp = 2 * dy - dx;
			int deltaE = 2 * dy;
			int deltaNE = 2 * (dy - dx);
			int x = A.getX(); int y = A.getY();
			img(x, y) = 0;
			while (x < img.dimx()) {
				int nivgris = 0;
				if (dp <= 0) {
					//On choisit le point E 
					dp = dp + deltaE;  //Nouveau dp 
					x++;
					if (x >254) break;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					x++;
					y++;
					if (x >254 || y>254) break;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				NiveauDeGris.push_back(nivgris);
			}
			
		}
		else {
			int dp = 2 * dx - dy;
			int deltaE = 2 * dx;
			int deltaNE = 2 * (dx - dy);
			int x = A.getX(); int y = A.getY();
			img(x, y) = 0;
			//Looking in the right of our point A
			while (x < img.dimx()) {
				int nivgris = 0;
				if (dp <= 0) {
					//On choisit le point E 
					dp = dp + deltaE;  //Nouveau dp 
					y++;
					if (y > 254) break;
					//cout << "maybe here : " << x << " y :" << y << endl;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					x++;
					y++;
					if (x > 254 || y > 254) break;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				NiveauDeGris.push_back(nivgris);
			}	
		}

	}
	else {
		int dx = B.getX() - A.getX();
		int dy = A.getY() - B.getY();
		if (dx >= dy) {
			int dp = 2 * dy - dx;
			int deltaE = 2 * dy;
			int deltaNE = 2 * (dy - dx);
			int x = A.getX(); int y = A.getY();
			while (x > 0) {
				int nivgris = 0;
				if (dp < 0) {
					//On choisit le point E 
					dp = dp + deltaE;  //Nouveau dp 
					x--;
					if (x ==1) break;
					nivgris = img(x, y);

					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					x--;
					y++;
					if (x ==0 || y > 254) break;
					nivgris = img(x, y);
					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				NiveauDeGris.insert(NiveauDeGris.begin(), nivgris);
			}
		}
		else {
	
			int dp = 2 * dx - dy;
			int deltaE = 2 * dx;
			int deltaNE = 2 * (dx - dy);
			int x = A.getX(); int y = A.getY();
			img(x, y) = 0;

			while (x > 0) {
				int nivgris = 0;
				if (dp < 0) {
					//On choisit le point E 
					dp = dp + deltaE;  //Nouveau dp 
					y++;
					if (y > 254) break;
					nivgris = img(x, y);
					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					x--;
					y++;
					if (x ==0 || y > 254) break;
					nivgris = img(x, y);
					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				NiveauDeGris.insert(NiveauDeGris.begin(), nivgris);
			}

		}
	}

	return NiveauDeGris;
}
vector<Point2D> getPoints_of_theta(CImg<unsigned char> img, Point2D A, Point2D B) {
	vector<int> NiveauDeGris;	  //Vector to store gris level of each pixel of the line
	vector<Point2D> ColoredPoint;
	if (B.getY() >= A.getY()) {
		int dx = B.getX() - A.getX();
		int dy = B.getY() - A.getY();
		if (dx >= dy) {
			int dp = 2 * dy - dx;
			int deltaE = 2 * dy;
			int deltaNE = 2 * (dy - dx);
			int x = A.getX(); int y = A.getY();
			img(x, y) = 0;
			while (x < img.dimx()) {
				int nivgris = 0;
				if (dp <= 0) {
					//On choisit le point E 
					dp = dp + deltaE;  //Nouveau dp 
					x++;
					if (x >254) break;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					if (x >254 || y>254) break;
					x++;
					y++;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				NiveauDeGris.push_back(nivgris);
			}

		}
		else {
		
			int dp = 2 * dx - dy;
			int deltaE = 2 * dx;
			int deltaNE = 2 * (dx - dy);
			int x = A.getX(); int y = A.getY();
			img(x, y) = 0;
			//Looking in the right of our point A
			while (x < img.dimx()) {
				int nivgris = 0;
				if (dp <= 0) {
					//On choisit le point E 
					dp = dp + deltaE;  //Nouveau dp 
					if (x > 254 || y > 254) break;
					y++;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					if (x > 254 || y > 254) break;
					x++;
					y++;
					nivgris = img(x, y);
					ColoredPoint.push_back(Point2D(x, y));
				}
				NiveauDeGris.push_back(nivgris);
			}


		}


	}
	else {
		int dx = B.getX() - A.getX();
		int dy = A.getY() - B.getY();
		if (dx >= dy) {

			int dp = 2 * dy - dx;
			int deltaE = 2 * dy;
			int deltaNE = 2 * (dy - dx);
			int x = A.getX(); int y = A.getY();
			while (x > 0) {
				int nivgris = 0;
				if (dp < 0) {
					//On choisit le point E 
					if (x < 0) break;
					dp = dp + deltaE;  //Nouveau dp 
					x--;
					nivgris = img(x, y);

					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					if (x < 0|| y>254) break;
					x--;
					y++;
					nivgris = img(x, y);
					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				NiveauDeGris.insert(NiveauDeGris.begin(), nivgris);
			}
		}
		else {
			//Looking in the left of our point A
			int dp = 2 * dx - dy;
			int deltaE = 2 * dx;
			int deltaNE = 2 * (dx - dy);
			int x = A.getX(); int y = A.getY();
			img(x, y) = 0;

			while (x > 0) {
				int nivgris = 0;
				if (dp < 0) {
					//On choisit le point E 
					dp = dp + deltaE;  //Nouveau dp 
					y++;
					if ( y>254) break;
					nivgris = img(x, y);
					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				else {
					dp = dp + deltaNE;
					x--;
					y++;
					if (x < 0 || y>254) break;
					nivgris = img(x, y);
					ColoredPoint.insert(ColoredPoint.begin(), Point2D(x, y));
				}
				NiveauDeGris.insert(NiveauDeGris.begin(), nivgris);
			}

		}
	}
	return ColoredPoint;
}




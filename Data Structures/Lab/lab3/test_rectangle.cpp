#include "Rectangle.h"
#include "Point2D.h"
#include <iostream>

int main(){

	//4 points
	Point2D a = Point2D(  9.0,  3.5);
	Point2D b = Point2D(  4.0,  3.0);
	Point2D c = Point2D(  17.0,  9.0);
	Point2D d = Point2D(  8.0,  10.0);
	//2 rectangle boundary
	Point2D lowend1 = Point2D(  4.0,  3.0);
	Point2D highend1 = Point2D(  17.0,  9.0);
	Point2D lowend2 = Point2D(  9.0,  3.5);
	Point2D highend2 = Point2D(  17.0,  9.0);
	//2 rectangle
	Rectangle rect1 = Rectangle( lowend1,  highend1);
	Rectangle rect2 = Rectangle( lowend2,  highend2);

	//Test Rectangle class
	//test points_contains
	std::cout << "Rect1 contains 4.0,3.0 9.0,3.5 17.0,9.0: ";
	for (int i=0; i<rect1.points_contained().size(); i++){
		std::cout << rect1.points_contained()[i].x() << ',' << rect1.points_contained()[i].y() << ' ' << std::endl;
	}
	std::cout << "Rect2 contains 9.0,3.5 17.0,9.0: ";
	for (int i=0; i<rect2.points_contained().size(); i++){
	
		std::cout << rect2.points_contained()[i].x() << ',' << rect2.points_contained()[i].y() << ' ' << std::endl;
	}
	//test is_point_within
	std::cout << "(4,3) in rect1 should be true: " << rect1.is_point_within(b) << std::endl;
	std::cout << "(9,3.5) in rect1 should be true: " << rect1.is_point_within(a) << std::endl;
	std::cout << "(17,9) in rect1 should be true: " << rect1.is_point_within(c) << std::endl;
	std::cout << "(8,10) in rect1 should be false: " << rect1.is_point_within(d) << std::endl;
	std::cout << "(4,3) in rect2 should be false: " << rect2.is_point_within(b) << std::endl;
	std::cout << "(9,3.5) in rect2 should be true: " << rect2.is_point_within(a) << std::endl;
	std::cout << "(17,9) in rect2 should be true: " << rect2.is_point_within(c) << std::endl;
	std::cout << "(8,10) in rect2 should be false: " << rect2.is_point_within(d) << std::endl;
	//test add_point
	Point2D m = Point2D(  10.0,  10.0);
	Point2D n = Point2D(  10.0,  5.0);
	Point2D z = Point2D(  9.0,  3.5);
	std::cout << "Add (10,10) into rect1, should be false: " << rect1.add_point(m) << std::endl;
	std::cout << "Add (10,5) into rect1, should be true: " << rect1.add_point(n) << std::endl;
	std::cout << "Add (9.0,3.5) into rect1, should be false: " << rect1.add_point(z) << std::endl;
	std::cout << "Add (10,10) into rect2, should be false: " << rect1.add_point(m) << std::endl;
	std::cout << "Add (10,5) into rect2, should be true: " << rect1.add_point(n) << std::endl;
	std::cout << "Add (9.0,3.5) into rect2, should be false: " << rect1.add_point(z) << std::endl;

	return 0;
}
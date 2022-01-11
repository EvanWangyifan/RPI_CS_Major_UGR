#include <string>
#include <vector>
#include "utilities.h"

class Polygon
{
  public:
    Polygon(const std::string &name, const std::vector<Point> &verts)
    {
        if (verts.size() <= 2) 
            throw -1;
        _name = name;
        _verts = verts;
    }

    virtual ~Polygon() {}

    const std::string& getName() const { return _name; }

    virtual bool HasAllEqualSides() const
    {
        bool allEqual = true;
        for (int i = 2; i < _verts.size(); i++)
        {
            Point pt1 = _verts[i-2];
            Point pt2 = _verts[i-1];
            Point pt3 = _verts[i];
            double l1 = DistanceBetween(pt1, pt2);
            double l2 = DistanceBetween(pt2, pt3);
            if (!EqualSides(l1, l2))
            {
                allEqual = false; 
                break;
            }
        }
        return allEqual;
    }
    
    virtual bool HasAllEqualAngles() const
    {
      bool allEqual = true;
      std::vector<Point> tempVec =  _verts;
      tempVec.push_back(_verts[0]);
      tempVec.push_back(_verts[1]);
      tempVec.push_back(_verts[2]);
      for (int i = 3; i < tempVec.size(); i++)
      {
          Point pt1 = tempVec[i-3];
          Point pt2 = tempVec[i-2];
          Point pt3 = tempVec[i-1];
          Point pt4 = tempVec[i];
          double angle1  = Angle(pt1, pt2, pt3);
          double angle2 = Angle(pt2, pt3, pt4);
          if (!EqualAngles(angle1, angle2))
          {
              allEqual = false;
              return false;
          }
      }
      return allEqual;
    }

    virtual bool HasARightAngle() const
    {

      bool existRightAngle = false;
      std::vector<Point> tempVec =  _verts;
      tempVec.push_back(_verts[0]);
      tempVec.push_back(_verts[1]);
      for (int i = 2; i < tempVec.size(); i++)
      {
          Point pt1 = tempVec[i-2];
          Point pt2 = tempVec[i-1];
          Point pt3 = tempVec[i];
          double angle1  = Angle(pt1, pt2, pt3);
          if (RightAngle(angle1))
          {
              existRightAngle = true;
              break;
          }
      }
      return existRightAngle;

    }

  protected:
    std::string _name;
    std::vector<Point> _verts;
};

class Triangle : public Polygon
{
  public:
    Triangle(const std::string &name, const std::vector<Point> &verts) : Polygon(name, verts)
    {
        if (verts.size() != 3)
            throw -1;
    }
    
};

class IsoscelesTriangle : virtual public Triangle
{
  public:
    IsoscelesTriangle(const std::string &name, const std::vector<Point> &verts) : Triangle(name, verts)
    {
        Point pt1 = verts[0];
        Point pt2 = verts[1];
        Point pt3 = verts[2];
        double l1 = DistanceBetween(pt1, pt2);
        double l2 = DistanceBetween(pt2, pt3);
        double l3 = DistanceBetween(pt1, pt3);
        // If neither sides are equivalent, throw an exception.
        if (!(EqualSides(l1, l2) || EqualSides(l2, l3) || EqualSides(l3, l1)))
            throw -1;
    }


};

class RightTriangle : virtual public Triangle
{
  public:
    RightTriangle(const std::string &name, const std::vector<Point> &verts) : Triangle(name, verts)
    {
        Point pt1 = verts[0];
        Point pt2 = verts[1];
        Point pt3 = verts[2];
        double angle1 = Angle(pt1, pt2, pt3);
        double angle2 = Angle(pt2, pt3, pt1);
        double angle3 = Angle(pt3, pt1, pt2);
        // At least one angle should be a right angle.
        if (!RightAngle(angle1) && !RightAngle(angle2) && !RightAngle(angle3))
          throw -1;
    }

    virtual bool HasARightAngle() const {return true;}
};

class IsoscelesRightTriangle : public IsoscelesTriangle, public RightTriangle
{
  public:
    IsoscelesRightTriangle(const std::string &name, const std::vector<Point> &verts) : Triangle(name, verts),
                                                                                       IsoscelesTriangle(name, verts),
                                                                                       RightTriangle(name, verts)
    {}
    virtual bool HasARightAngle() const  {return true;}
    virtual bool HasAllEqualSides() const {return false;}
    virtual bool HasAllEqualAngles() const {return false;}
};

class EquilateralTriangle : public IsoscelesTriangle
{
  public:
    EquilateralTriangle(const std::string &name, const std::vector<Point> &verts) : Triangle(name, verts), 
                                                                                    IsoscelesTriangle(name, verts)
    {
        Point pt1 = verts[0];
        Point pt2 = verts[1];
        Point pt3 = verts[2];
        double l1 = DistanceBetween(pt1, pt2);
        double l2 = DistanceBetween(pt2, pt3);
        double l3 = DistanceBetween(pt1, pt3);
        // If any two sides are not equivalent, throw an exception.
        if (!EqualSides(l1, l2) || !EqualSides(l2, l3) || !EqualSides(l1, l3))
            throw -1;
    }

    virtual bool HasAllEqualSides() const {return true;}
    virtual bool HasAllEqualAngles() const {return true;}



};

class Quadrilateral : public Polygon
{
  public:
    Quadrilateral(const std::string &name, const std::vector<Point> &verts) : Polygon(name, verts)
    {
        if (verts.size() != 4)
            throw -1;
    }


};

class Rectangle : public Quadrilateral
{
  public:
    Rectangle(const std::string &name, const std::vector<Point> &verts) : Quadrilateral(name, verts)
    {
        Point pt1 = verts[0];
        Point pt2 = verts[1];
        Point pt3 = verts[2];
        Point pt4 = verts[3];
        double angle1 = Angle(pt1, pt2, pt3);
        double angle2 = Angle(pt2, pt3, pt4);
        double angle3 = Angle(pt3, pt4, pt1);
        double angle4 = Angle(pt4, pt1, pt2);
        // If any angle is not a right angle, throw an exception
        if (!RightAngle(angle1) || !RightAngle(angle2) || !RightAngle(angle3) || !RightAngle(angle4))
            throw -1;
    }

    virtual bool HasARightAngle() const  {return true;}
    virtual bool HasAllEqualAngles() const {return true;}

    
};

class Square : public Rectangle
{
  public:
    Square(const std::string &name, const std::vector<Point> &verts) : Rectangle(name, verts)
    {
        Point pt1 = verts[0];
        Point pt2 = verts[1];
        Point pt3 = verts[2];
        Point pt4 = verts[3];
        int l1 = DistanceBetween(pt1, pt2);
        int l2 = DistanceBetween(pt2, pt3);
        int l3 = DistanceBetween(pt3, pt4);
        int l4 = DistanceBetween(pt4, pt1);
        // If any two sides are not equivalent, thrwo an exception
        if (!(EqualSides(l1, l2) || EqualSides(l2, l3) || EqualSides(l3, l4) || EqualSides(l4, l1)))
            throw -1;
    }

    virtual bool HasARightAngle() const  {return true;}
    virtual bool HasAllEqualSides() const {return true;}
    virtual bool HasAllEqualAngles() const {return true;}

};
/*
  Author: Daniel Pinto
*/
/*
  Collaborated with Brereton Conway, he taught me about structs and then we did the code together
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<cmath>
using namespace std;

const long double pi = 3.14159265359;

typedef struct point_t {
  double x;
  double y;
  double z;
} point_t;
typedef struct  triangle_t {
  point_t one;
  point_t two;
  point_t three;
} triangle_t;


point_t getNorm(triangle_t t, double size){
  point_t n, U, V;
  U.x = t.two.x - t.one.x;
  U.y = t.two.y - t.one.y;
  U.z = t.two.z - t.one.z;

  V.x = t.three.x - t.two.x;
  V.y = t.three.y - t.two.y;
  V.z = t.three.z - t.two.z;

  n.x  = ((U.y * V.z) - (U.z * V.y))/(size*size);
  n.y  = ((U.z * V.x) - (U.x * V.z))/(size*size);
  n.z  = ((U.x * V.y) - (U.y * V.x))/(size*size);
  return n;
}

class Shape {
private:
  double x;
  double y;
  double z;

public:
  Shape(double x, double y, double z) : x(x), y(y), z(z){}
  virtual void write(string f) const = 0;
  //virtual void getNorm(triangle_t t) const = 0;

  triangle_t makeTri(point_t a, point_t b, point_t c){
    triangle_t triangle;
    triangle.one = a;
    triangle.two = b;
    triangle.three = c;
    return triangle;
  }

};

int count = 0;

void movePoint(point_t& b, double x, double y, double r, double theta, double& thetaCount){

  b.x = x + r*(cos(thetaCount));
  b.y = y + r*(sin(thetaCount));
  thetaCount += theta;
}

class Cylinder : public Shape{
private:
  double r, h;
  int facets;
  vector<triangle_t> triangle;
public:
  Cylinder(double x, double y, double z, double r, double h, int facets) : Shape(x, y, z), r(r), h(h)  {
    double theta = (double(2*pi))/(double(facets));
    double thetaCount = (double(2*pi))/(double(facets));
    point_t origin1 = {x,y,0};
    point_t origin2 = {x,y,h};
    point_t start = {x+r,0,0};
    point_t start2 = {x+r,0,h};
    point_t temp;
    point_t temp2;
    temp.x = start.x;
    temp.y = start.y;
    temp.z = start.z;
    temp2.x = start2.x;
    temp2.y = start2.y;
    temp2.z = start2.z;


    for(int i = 0; i < facets; i++){
      movePoint(temp, x, y, r, theta, thetaCount);
      triangle.push_back(makeTri(origin1,temp,start));
      triangle.push_back(makeTri(start,start2,temp));

      start.x = temp.x;
      start.y = temp.y;
      temp2.x = temp.x;
      temp2.y = temp.y;

      triangle.push_back(makeTri(origin2,temp2,start2));
      triangle.push_back(makeTri(start2,temp,temp2));
      start2.x = temp2.x;
      start2.y = temp2.y;
    }
  }
  void write(string myfile) const {
    count++;
    ofstream f(myfile, ios::app);
    f << "solid cylinder" << count << endl << "\t";
    for(int i = 0; i<triangle.size(); i++){
      f << "\t facet normal " << getNorm(triangle[i], pow(r,2.0)).x << " " << getNorm(triangle[i],pow(r,2.0)).y << " " << getNorm(triangle[i],pow(r,2.0)).z << endl << "\t" << "outer loop" << endl;
      f << "\t \t outer loop \n";
      f << "\t \t \t vertex " << triangle[i].one.x << " " <<triangle[i].one.y << " " << triangle[i].one.z << "\n";
      f << "\t \t \t vertex " << triangle[i].two.x << " " <<triangle[i].two.y << " " << triangle[i].two.z << "\n";
      f << "\t \t \t vertex " << triangle[i].three.x << " " <<triangle[i].three.y << " " << triangle[i].three.z << "\n";
      f <<  "\t \t end loop" << "\n";
      f << "\t end facet" << '\n';
      }
    f << "endsolid cylinder" << count <<'\n';

  }
};


class Cube :public Shape {
private:
  double size;
  point_t normal[12];
  vector<triangle_t> triangle;
public:
  Cube(double x, double y, double z, double size) : Shape(x,y,z), size(size){
    point_t c0 = {x, y, z};
    point_t c1 = {x + size, y, z};
    point_t c2 = {x, y + size, z};
    point_t c3 = {x, y, z + size};
    point_t c4 = {x + size, y + size, z};
    point_t c5 = {x + size,y , z + size};
    point_t c6 = {x, y + size, z + size};
    point_t c7 = {x + size, y + size, z + size};

    triangle.push_back(makeTri(c6,c5,c7));
    triangle.push_back(makeTri(c5,c6,c3));
    triangle.push_back(makeTri(c0,c4,c1));
    triangle.push_back(makeTri(c4,c0,c2));
    triangle.push_back(makeTri(c0,c5,c3));
    triangle.push_back(makeTri(c5,c0,c1));
    triangle.push_back(makeTri(c5,c4,c7));
    triangle.push_back(makeTri(c4,c5,c1));
    triangle.push_back(makeTri(c4,c6,c7));
    triangle.push_back(makeTri(c6,c4,c2));
    triangle.push_back(makeTri(c0,c6,c2));
    triangle.push_back(makeTri(c6,c0,c3));
  }

  triangle_t makeTri(point_t a, point_t b, point_t c){
    triangle_t triangle;
    triangle.one = a;
    triangle.two = b;
    triangle.three = c;
    return triangle;
  }



  void write(string f) const{
    count++;
    ofstream myfile(f, ios::app);
    myfile <<"solid Cube " << count <<"\n";
    for(int i = 0; i < 12; i++){
      myfile << "\t facet normal "<<(getNorm(triangle[i], this->size)).x<<" " <<(getNorm(triangle[i], this->size)).y<< " "<< (getNorm(triangle[i], this->size)).z <<"\n";
      myfile << "\t \t outer loop \n";
      myfile << "\t \t \t vertex " << triangle[i].one.x << " " <<triangle[i].one.y << " " << triangle[i].one.z << "\n";
      myfile << "\t \t \t vertex " << triangle[i].two.x << " " <<triangle[i].two.y << " " << triangle[i].two.z << "\n";
      myfile << "\t \t \t vertex " << triangle[i].three.x << " " <<triangle[i].three.y << " " << triangle[i].three.z << "\n";
      myfile <<  "\t \t end loop" << "\n";
      myfile << "\t end facet" << '\n';
    }
    myfile << "endsolid Cube " << count<< '\n';
  }

};


class CAD{
private:
  vector<Shape*> shapes;
public:
  CAD(){}
  ~CAD(){
    for(int i = 0; i < shapes.size(); i++){
      delete shapes[i];
    }
  }
  void add( Shape* shape) {
    shapes.push_back(shape);
  }

  void write(string f){
    for (int i = 0; i < shapes.size(); i++)
      shapes[i]->write(f);
  }
  // void add(Cube c){
  //   shapes.push_back(c);
  // }
  // void add(Cylinder c){
  //   shapes.push_back(c);
  // }
  // //void write(string s);
};

int main(){
  ofstream myfile;
  myfile.open("test.stl");
  CAD c;
  c.add(new Cube (0,0,0,5));
  c.add(new Cylinder(100, 0, 0,3,10,10));
  c.write("test.stl");

}

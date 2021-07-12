// Cristofear Santillan

#include <iostream>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <cmath>

using namespace std;

class Point{
  public:
    long double x;
    long double y;

    Point(long double px, long double py){
      x = px;
      y = py;
    }

    Point(){
      x = 0.0;
      y = 0.0;
    }
};

int readData(vector<Point*> &mPoints){

  ifstream fin("input.txt");

  int n;
  fin >> n;

  double x1,y1,x2,y2;

  while(fin >> x1 >> y1 >> x2 >> y2){
    mPoints.push_back(new Point(x1,y1));
    mPoints.push_back(new Point(x2,y2));
  }

  fin.close();

  return n;

}

string rules( char c ){

  if ( c == 'A' ){

    return "B-A-B";

  } else if ( c == 'B' ){

    return "A+B+A";

  }

  return string(1,c);

}

string evalString( string s ){

  string newS = "";

  for( int i = 0; i < s.length(); i++ ){

    newS += rules(s[i]);

  }

  return newS;

}

string createSierpinski( int n, string a ){

  string s = a;

  for( int i = 0; i < n; i++ ){

    s = evalString(s);

  }

  return s;

}

double direction( Point* Pi, Point* Pj, Point* Pk ){

  return ((Pj->x - Pi->x)*(Pk->y - Pi->y)) - ((Pk->x - Pi->x)*(Pj->y - Pi->y));

}

bool segment( Point* Pi, Point* Pj, Point* Pk ){

  if ( ( min(Pi->x,Pj->x) <= Pk->x && max(Pi->x,Pj->x) >= Pk->x ) && ( min(Pi->y,Pj->y) <= Pk->y && max(Pi->y,Pj->y) >= Pk->y ) ){

    return true;

  }

  return false;

}

bool intersect( Point* p1, Point* p2, Point* p3, Point* p4 ){

  double d1 = direction(p3,p4,p1);
  double d2 = direction(p3,p4,p2);
  double d3 = direction(p1,p2,p3);
  double d4 = direction(p1,p2,p4);

  if ( ((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)) ){

    return true;

  } else if ( d1 == 0 && segment(p3,p4,p1) ){

    return true;

  } else if ( d2 == 0 && segment(p3,p4,p2) ){

    return true;

  } else if ( d3 == 0 && segment(p1,p2,p3) ){

    return true;

  } else if ( d4 == 0 && segment(p1,p2,p4) ){

    return true;

  }

  return false;

}

vector<Point*> conversion( string s ){

  if(s.length()%3 == 0){
    s = " " + s;
  }

  vector<Point*> sp;
  sp.push_back(new Point(0.0,0.0));

  int degree = 0;
  long double d = sqrt(3)/2;
  char c = 0;
  Point prev;

  for(int i = 0; i < s.length(); i++){

    c = s[i];

    if( c == '+' ){
      degree += 60;
      continue;
    } else if ( c == '-' ){
      degree -= 60;
      continue;
    } else if (c == ' '){
      continue;
    }

    prev = *sp[sp.size()-1];

    if(degree == 0 || degree == 360 || degree == -360){

      prev.x += 1.0;

      if( degree == 360 || degree == -360 ){
        degree = 0;
      }

    } else if (degree == 60 || degree == -300){

      prev.x += 0.5;
      prev.y += d;

    } else if (degree == 120 || degree == -240){

      prev.x -= 0.5;
      prev.y += d;

    } else if (degree == 180 || degree == -180){

      prev.x -= 1;

    } else if (degree == 240 || degree == -120){

      prev.x -= 0.5;
      prev.y -= d;

    } else if (degree == 300 || degree == -60){

      prev.x += 0.5;
      prev.y -= d;

    }

    sp.push_back(new Point(prev.x,prev.y));

  }

  return sp;

}

void outputData( vector<Point*> mPoints, vector<Point*> sierpinskiPoints ){

  vector<int> results;

  bool intersects = false;

  for(int i = 0; i < mPoints.size(); i += 2){

    Point *p1 = mPoints.at(i);
    Point *p2 = mPoints.at(i+1);

    for(int j = 0; j < sierpinskiPoints.size()-1; j++){

      Point *p3 = sierpinskiPoints.at(j);
      Point *p4 = sierpinskiPoints.at(j+1);

      if(intersect(p1,p2,p3,p4)){

        intersects = true;
        break;

      }

    }

    if(intersects){

      results.push_back(1);
      intersects = false;

    }else{

      results.push_back(0);

    }

  }

  ofstream fout("output.txt");

  for(auto& k : results){fout << k << endl;}

  fout.close();

}

int main() {

  cout.precision(17);

  vector<Point*> mPoints;

  int n = readData(mPoints);

  string sierpinskiString = "";

  if( n % 2 == 0 ){

    sierpinskiString = createSierpinski( n, "A" );

  } else {

    sierpinskiString = createSierpinski( n, "+A" );

  }

  vector<Point*> sierpinskiPoints = conversion(sierpinskiString);

  outputData( mPoints, sierpinskiPoints );

}

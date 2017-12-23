#include "std_lib_facilities_3.h"
#include "Simple_window.h"
#include "Graph.h"

struct Regular_polygon : Polygon {

public:
    Regular_polygon(int x, int y, int n, int r);

};

Regular_polygon::Regular_polygon(int x, int y, int n, int r) {
    double pi = 3.14159;
    for (double i=0; i<n; ++i) {
        double xx = x + (r * cos(2*pi * i/n));
        double yy = y + (r * sin(2*pi * i/n));
        Polygon::add(Point(xx,yy));
    }
}

int main() {
    try {
        cout << "Drawing polygons...\n";
        
        Simple_window win1(Point(100,200),600,600,"Concentric Polygons");
        
        //method using a loop; caused incomprehensible errors
        /*vector<Regular_polygon> polys;
        for (int i=3; i<10; ++i) {
            Regular_polygon poly(150, 150, i, 50+2*i);
            polys.push_back(poly);
            win1.attach(polys[polys.size()-1]);
        }*/
        
        //method written out
        Regular_polygon tri(150, 150, 3, 45);
        win1.attach(tri);
        Regular_polygon squar(150, 150, 4, 60);
        win1.attach(squar);
        Regular_polygon pent(150, 150, 5, 70);
        win1.attach(pent);
        Regular_polygon hex(150, 150, 6, 80);
        win1.attach(hex);
        Regular_polygon hept(150, 150, 7, 90);
        win1.attach(hept);

        cout << "Polygons drawn successfully.\n";
        win1.wait_for_button();
    } catch(exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        cerr << "Some exception\n";
        return 2;
    }
    return 0;
}
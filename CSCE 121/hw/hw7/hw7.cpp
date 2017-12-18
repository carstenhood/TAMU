
#include <iostream>
#include <sstream>
#include "Graph.h"
#include "GUI.h"
#include "Window.h"
#include "std_lib_facilities_3.h"

using namespace Graph_lib;
using namespace std;

struct Triangle : public Polygon {
    Triangle(Point p) {
        double height = 30*1.732;
        add(Point(p.x, p.y-height/2));
        add(Point(p.x+30, p.y+height/2));
        add(Point(p.x-30, p.y+height/2));
    }
};

struct Hexagon : public Polygon {
    Hexagon(Point p) {
        double pi = 3.14159;
        for (double i=0; i<6; ++i) {
            double xx = p.x + (30 * cos(2*pi * i/6));
            double yy = p.y + (30 * sin(2*pi * i/6));
            add(Point(xx,yy));
        }
    }
};

struct Shapes_window : Graph_lib::Window {
    Shapes_window(Point xy, int w, int h, const string& title );
private:
    Vector_ref<Shape> shapes;
    string which_shape;
    
    // Widgets:
    Button add_button;
    Button quit_button;
    In_box next_x;
    In_box next_y;
    Out_box xy_out;
    Menu shapes_menu;
    Button menu_button;

    void add_circle(Point p);
    void add_square(Point p);
    void add_triangle(Point p);
    void add_hexagon(Point p);
    
    void hide_menu() { shapes_menu.hide(); menu_button.show(); } 

    // actions invoked by callbacks:
    void circle_pressed() { which_shape = "circle";  hide_menu(); }
    void square_pressed() { which_shape = "square";  hide_menu(); }
    void triangle_pressed() { which_shape = "triangle";  hide_menu(); }
    void hexagon_pressed() { which_shape = "hexagon";  hide_menu(); }
    void menu_pressed() { menu_button.hide();  shapes_menu.show(); }
    void next(); 
    void quit(); 

    // callback functions:
    static void cb_circle(Address, Address);
    static void cb_square(Address, Address);
    static void cb_triangle(Address, Address);
    static void cb_hexagon(Address, Address);
    static void cb_menu(Address, Address);
    static void cb_next(Address, Address);
    static void cb_quit(Address, Address);
};

//------------------------------------------------------------------------------

Shapes_window::Shapes_window(Point xy, int w, int h, const string& title)
    :Window(xy,w,h,title),
    shapes_menu(Point(x_max()-70,30),70,20,Menu::vertical,"Shapes"),
    menu_button(Point(x_max()-80,30), 80, 20, "Shapes", cb_menu),
    add_button(Point(x_max()-150,0), 70, 20, "Add", cb_next),
    quit_button(Point(x_max()-70,0), 70, 20, "Quit", cb_quit),
    next_x(Point(x_max()-310,0), 50, 20, "x:"),
    next_y(Point(x_max()-210,0), 50, 20, "y:"),
    xy_out(Point(100,0), 100, 20, "current (x,y):")
{
    attach(add_button);
    attach(quit_button);
    attach(next_x);
    attach(next_y);
    attach(xy_out);
    xy_out.put("no point");
    shapes_menu.attach(new Button(Point(0,0),0,0,"circle",cb_circle));
    shapes_menu.attach(new Button(Point(0,0),0,0,"square",cb_square));
    shapes_menu.attach(new Button(Point(0,0),0,0,"triangle",cb_triangle));
    shapes_menu.attach(new Button(Point(0,0),0,0,"hexagon",cb_hexagon));
    attach(shapes_menu);
    shapes_menu.hide();
    attach(menu_button);
    
    which_shape = "circle"; //circle is default
}

//------------------------------------------------------------------------------

void Shapes_window::cb_quit(Address, Address pw) {
    reference_to<Shapes_window>(pw).quit();
}

void Shapes_window::quit() {
    hide();
}

//------------------------------------------------------------------------------

void Shapes_window::cb_next(Address, Address pw) {
    reference_to<Shapes_window>(pw).next();
} 

void Shapes_window::next() {
    int x = next_x.get_int();
    int y = next_y.get_int();

    if (which_shape == "circle") {
        add_circle(Point(x,y));
    } else if (which_shape == "square") {
        add_square(Point(x,y));
    } else if (which_shape == "triangle") {
        add_triangle(Point(x,y));
    } else if (which_shape == "hexagon") {
        add_hexagon(Point(x,y));
    }

    stringstream ss;
    ss << '(' << x << ',' << y << ')';
    xy_out.put(ss.str());

    redraw();
}

void Shapes_window::add_circle(Point p) {
    shapes.push_back(new Circle(p, 30));
    attach(shapes[shapes.size()-1]);
}

void Shapes_window::add_square(Point p) {
    shapes.push_back(new Rectangle(Point(p.x-25, p.y-25), 50, 50));
    attach(shapes[shapes.size()-1]);
}

void Shapes_window::add_triangle(Point p) {
    shapes.push_back(new Triangle(p));
    attach(shapes[shapes.size()-1]);
}

void Shapes_window::add_hexagon(Point p) {
    shapes.push_back(new Hexagon(p));
    attach(shapes[shapes.size()-1]);
}

//------------------------------------------------------------------------------

void Shapes_window::cb_circle(Address, Address pw) {
    reference_to<Shapes_window>(pw).circle_pressed();
}

void Shapes_window::cb_square(Address, Address pw) {
    reference_to<Shapes_window>(pw).square_pressed();
} 

void Shapes_window::cb_triangle(Address, Address pw) {
    reference_to<Shapes_window>(pw).triangle_pressed();
}

void Shapes_window::cb_hexagon(Address, Address pw) {
    reference_to<Shapes_window>(pw).hexagon_pressed();
}

void Shapes_window::cb_menu(Address, Address pw) {
    reference_to<Shapes_window>(pw).menu_pressed();
} 

//------------------------------------------------------------------------------

int main()
try {
    Shapes_window win(Point(100,100),600,400,"Shapes");
    return gui_main();
}
catch(exception& e) {
    cerr << "exception: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Some exception\n";
    return 2;
}

//------------------------------------------------------------------------------

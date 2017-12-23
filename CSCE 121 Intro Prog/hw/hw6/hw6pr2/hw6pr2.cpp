#include "std_lib_facilities_3.h"
#include "Simple_window.h"
#include "Graph.h"

class Bad_argument { };
class Bad_input { };

struct Binary_tree : Shape {
private:
    vector<Point> nodes;
    int levels;
    
    void add(Point p) {
        Shape::add(p);
    }
    
public:
    Binary_tree(int levs) {
        levels = levs;
        
        if (levels==0) return;
        
        //use a default size for binary tree
        const double border = 50;
        const double screen_width = 600;
        const double screen_height = 600;
        const double height = screen_width-border*2;
        const double width = screen_height-border*2;
        
        double xshift = width/2;
        double yshift = height/levels;
        double x = border + width/2;
        double y = border;
        
        for (int r=0; r<levels; ++r) { //adds node points to vector
            int columns = pow(2,r);
            xshift = width/(columns+1);
            x = border+xshift;
            for (int c=0; c<columns; ++c) {
                add(Point(x,y));
                x+=xshift;
            }
            y+=yshift;
        }
    }
    
    void draw_lines() const {
        if (levels==0) { return; }
        
        const int rad = 5; //node radius
        for (int i=0; i<Shape::number_of_points(); ++i) //draw nodes
            fl_arc(Shape::point(i).x-rad,Shape::point(i).y-rad,rad*2,rad*2,0,360);
        
        for (int r=0; r<levels-1; ++r) { //draw lines
            int rstart = pow(2,r)-1;
            int rend = pow(2,r+1)-1;
            int next = rend;
            for (int c = rstart; c < rend; ++c) {
                for (int i=0; i<2; ++i) {
                    fl_line(Shape::point(c).x, Shape::point(c).y, Shape::point(next).x, Shape::point(next).y);
                    ++next;
                }
            }
        }
    }
};

int get_levels() {
    int limit = 20;
    while (true) {
        try {
            cout << "Enter number of levels for your binary tree: ";
            string input;
            getline(cin, input);
            
            int temp_arg=0;
            stringstream arg_stream(input);
            arg_stream >> temp_arg;
            
            if (!arg_stream)
                throw Bad_input();
            else if (temp_arg < 0 || temp_arg>limit)
                throw Bad_argument();
            
            return temp_arg;
        } catch(Bad_input) {
            cout << "Invalid input. Try again.\n";
        } catch(Bad_argument) {
            cout << "Number of levels must be at least 0 and no more than " << limit << ".\n";
        }
    }
}

int main() {
    try {
        int levels = get_levels();
        cout << "\nDrawing binary tree...\n";
        Binary_tree bt(levels);
        
        Simple_window win1(Point(100,200),600,600,"Binary Tree");
        win1.attach(bt);
        cout << "\nBinary tree drawn successfully.\n";
        win1.wait_for_button();
    } catch(exception& e) {
        cerr << "Exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        cerr << "Some exception\n";
        return 1;
    }
    return 0;
}
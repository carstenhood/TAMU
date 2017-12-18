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

vector<int> get_args() {
    vector<string> vars;
    vars.push_back("center x-coordinate");
    vars.push_back("center y-coordinate");
    vars.push_back("number of sides");
    vars.push_back("distance from center to corner");
    cout << "Enter arguments to define your polygon.\n";
    cout << "Units are pixels.\n";
    
    vector<int> args;
    for (int i=0; i<vars.size(); ++i) {
        while (true) {
            try {
                cout << "Enter the " << vars[i] << ": ";
                string input;
                getline(cin, input);
                
                int temp_arg=0;
                stringstream arg_stream(input);
                arg_stream >> temp_arg;
                
                if (!arg_stream)
                    throw Bad_input();
                else if ((i==2) && temp_arg<3)
                    throw Bad_argument();
                else if (temp_arg <= 0)
                    throw Negative_input();
                
                args.push_back(temp_arg);
                break;
            } catch(Bad_input) {
                cout << "Invalid input. Try again.\n";
            } catch(Negative_input) {
                cout << "Input values must be positive.\n";
            } catch(Bad_argument) {
                cout << "Number of sides must be greater than 2.\n";
            }
        }
    }
    return args;
}

int main() {
    try {
        vector<int> args = get_args();
        cout << "\nDrawing polygon...\n";
        Regular_polygon poly(args[0], args[1], args[2], args[3]);
        
        Simple_window win1(Point(100,200),600,600,"Regular Polygon");
        win1.attach(poly);
        cout << "\nPolygon drawn successfully.\n";
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
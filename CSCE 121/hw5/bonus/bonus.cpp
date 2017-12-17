#include "std_lib_facilities_3.h"
#include "Simple_window.h"
#include "Graph.h"

class Bad_argument { };
class Bad_points { };
class Bad_input { };

vector<Point> get_points(double a, double b, double m, double n, double N) {
    int num = N;
    vector<Point> points;
    double radius = a;
    double sign = -1;
    double direction = 1;
    double interval = radius*4/num;
    double x = 0;
    cout << "\nDrawing lines between points:";
    for (int i=0; i<num; i++) {
        sign*=-1;
        double term = 1 - pow(abs(x/a),m);
        double y = sign*pow(term,1/n)*b;
        cout << "\n(" << x << ", " << y << ")";
        points.push_back(Point(x+a, y+b));
        x+=direction*interval;
        if (x>radius) {
            x = radius-(x-radius);
            direction=-1;
        } else if (x<-radius) {
            x = -radius+(-x-radius);
            direction=1;
        }
    }
    cout << "\nDrawing shape...\n";
    return points;
}

vector<double> get_args() {
    vector<char> vars;
    vars.push_back('a');
    vars.push_back('b');
    vars.push_back('m');
    vars.push_back('n');
    vars.push_back('N');
    cout << "Please enter at least 2 for 'N' (number of points).\n";
    cout << "Units for 'a' and 'b' are pixels.\n";
    vector<double> args;
    for (int i=0; i<vars.size(); ++i) {
        while (true) {
            try {
                cout << "Enter " << vars[i] << ": ";
                string input;
                getline(cin, input);
                if (input == "") throw Bad_input();
                
                double temp_arg=0;
                stringstream arg_stream(input);
                arg_stream >> temp_arg;
                if (!arg_stream) throw Bad_input();
                if (temp_arg<=0) {
                    throw Bad_argument();
                } else if ((i==4) && temp_arg<2) {
                    throw Bad_points();
                }
                args.push_back(temp_arg);
                break;
            } catch(Bad_input) {
                cout << "Invalid input. Try again.\n";
            } catch(Bad_argument) {
                cout << "Please enter values greater than 0.\n";
            } catch(Bad_points) {
                cout << "Please enter at least 2 for 'N' (number of points).\n";
            }
        }
    }
    return args;
}

int main() {
    cout << "Enter arguments to define your superellipse.\n";
    try {
        vector<double> args = get_args();
        vector<Point> points = get_points(args[0],args[1],args[2],args[3],args[4]);

        Simple_window win1(Point(100,200),600,600,"Superellipse");
        
        Lines le;
        int loops=0;
        for (int i=0; i<points.size(); ++i) {
            for (int ii=i+1; ii<points.size(); ++ii) {
                le.add(points[i],points[ii]);
                ++loops;
            }
        }
        win1.attach(le);
        cout << "\nNumber of lines: " << loops;
        cout << "\nDone.";
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

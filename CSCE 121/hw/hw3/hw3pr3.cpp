#include "std_lib_facilities_3.h"

void organize(vector<double> series, double &smallest, double &largest, double &mean, double &median) {
    for (int k=1; k<series.size(); ++k) {
        double prev = series[k-1];
        double curr = series[k];
        if (prev>curr) {
            series[k]=prev;
            series[k-1]=curr;
            k=0;
        }
    }
    smallest = series[0];
    largest = series[series.size()-1];
   
    double total_sum = 0; //sum values to determine mean
    for (int i=0; i<series.size(); ++i) {
        total_sum+=series[i];
    }
    mean = total_sum/series.size();
   
    int middle = series.size()/2;
    median = series[middle];
    if (series.size()%2==0) { //if 2 values in the middle, average them
        int middle2 = series.size()/2-1;
        median = (median + series[middle2])/2;
    }
}

int main() {
    vector<double> series;
    cout << "Enter a series of numbers. End with a non-number.\n";
    double input;
    while (true) {
        cin >> input;
        if (!cin) {
            break;
        }
        series.push_back(input);
    }
    if (series.size() > 0) {
        double smallest=0;
        double largest=0;
        double mean=0;
        double median=0;
        organize(series, smallest, largest, mean, median);
        cout << "Smallest: " << smallest << " Largest: " << largest << " Mean: " << mean << " Median: " << median;
    }
    return 0;
}



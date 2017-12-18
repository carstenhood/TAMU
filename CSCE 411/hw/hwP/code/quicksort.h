// Carsten Hood
// UIN: 922009787
// CSCE 411-200H Fall 2016
// HW P due 2016/12/05
// quicksort.h

// custom quicksort implementation

// significant references:
// http://thecodersportal.com/mst-krushkal/;

#ifndef quicksort_h
#define quicksort_h

using namespace std;

// quicksort is used at the beginning of kruskal's algorithm
void quick_sort(vector<edge> &edges, int left, int right) {
    int i = left;
    int j = right;
    int pivot = edges[(left + right)/2].weight; // pivot at middle
    
    // swap out-of-place elements to correct sides of pivots
    while (i <= j) {
        while (edges[i].weight < pivot) i++;
        while (edges[j].weight > pivot) j--;
        if (i <= j) {
            // swap elements
            edge temp = edges[i];
            edges[i] = edges[j];
            edges[j] = temp;
            i++;
            j--;
        }
    }

    // recursion
    if (i < right) quick_sort(edges, i, right);
    if (j > left) quick_sort(edges, left, j);
}

#endif
/* quicksort_h */

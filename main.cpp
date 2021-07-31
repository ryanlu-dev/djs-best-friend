// Hello, I'm the driver.
#include "back.cpp"
#include "front.cpp"
#include "time.h"
#include <iostream>
using namespace std;

/*
    Default data structure:
        Hash table with keys of Spotify IDs and values of Track objects.
    Alternative data structure:

*/

int main() { // Move data parsing to back-end?
    clock_t t1, t2;
    t1 = clock();
    SongHash sHash;
    t2 = clock();
    float elapsed = ((float)t2 - (float)t1)/CLOCKS_PER_SEC;
    cout << "Phew, that took " << elapsed << " seconds." << endl;
    return 0;
}
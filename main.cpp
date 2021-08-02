// Hello, I'm the driver.
#include "back.cpp"
#include "front.cpp"
#include "time.h"
#include <iostream>
#define TESTING_ID "1UGD3lW3tDmgZfAVDh6w7r"
#define TESTING_TEMPO 0.15
#define TESTING_KEY 0.1
#define TESTING_DANCEABILITY 0.2
#define TESTING_ENERGY 0.05
using namespace std;

/*
    Default data structure:
        Hash table with keys of Spotify IDs and values of Track objects.
    Alternative data structure:

*/

int main() {
    clock_t t1, t2, t3, t4;
    t1 = clock();
    SongHash sHash;
    t2 = clock();
    float parsed = ((float)t2 - (float)t1)/CLOCKS_PER_SEC;
    cout << "Phew, that took " << parsed << " seconds to parse the data and add it into the hash table." << endl;
    unsigned int DIND = sHash.GetHash(TESTING_ID);
    Track* devil = sHash.GetTrack(DIND, TESTING_ID);
    t3 = clock();
    float found = ((float)t3 - (float)t2)/CLOCKS_PER_SEC;
    cout << "Then, it took " << found << " seconds to find Devil in a New Dress." << endl;
    vector<Track*> similarTrax = sHash.GetSimilarTracks(devil, TESTING_TEMPO, TESTING_KEY, TESTING_DANCEABILITY, TESTING_ENERGY);
    for(auto it : similarTrax) {
        cout << "Similar track found: " << it->name << " by ";
        for(auto aIt : it->artists) {
            cout << aIt << " ";
        }
        cout << endl;
    }
    t4 = clock();
    float similar = ((float)t4 - (float)t3)/CLOCKS_PER_SEC;
    cout << "Wow, it took " << similar << " seconds to find all these similar songs." << endl;
    return 0;
}
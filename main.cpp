// Hello, I'm the driver.
#include "back.cpp"
#include "front.cpp"
#include "time.h"
#define TESTING_ID "3cfOd4CMv2snFaKAnMdnvK"
#define TESTING_TEMPO 0.05
#define TESTING_KEY 0.0
#define TESTING_DANCEABILITY 0.1
#define TESTING_ENERGY 0.05

/*
    Default data structure:
        Hash table with keys of Spotify IDs and values of Track objects.
    Alternative data structure:
        Binary Search Tree which compares Spotify IDs and values of Track objects.
*/

int main() {
    cout << "Hash table implementation:" << endl << endl;
    clock_t t1, t2, t3, t4;
    t1 = clock();
    SongHash sHash;
    t2 = clock();
    float parsed = ((float)t2 - (float)t1)/CLOCKS_PER_SEC;
    cout << "Phew, that took " << parsed << " seconds to parse the data and add it into the hash table." << endl;
    unsigned int DIND = sHash.GetHash(TESTING_ID);
    Track* devil = sHash.GetTrack(DIND, TESTING_ID);
    t3 = clock();
    double found = ((double)t3 - (double)t2) * 1000000 / CLOCKS_PER_SEC;
    cout << "Then, it took " << found << " microseconds to find " << devil->name << endl;
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

    cout << "\nBinary Search Tree implementation: " << endl << endl;
    clock_t t5, t6, t7, t8;
    t5 = clock();
    SongTree sTree;
    t6 = clock();
    float tParsed = ((float)t6 - (float)t5)/CLOCKS_PER_SEC;
    cout << "Phew, that took " << tParsed << " seconds to parse the data and add it into the tree." << endl;
    Track* tDevil = sTree.Search(sTree.GetRoot(), TESTING_ID)->GetTrack();
    t7 = clock();
    double tFound = ((double)t7 - (double)t6) * 1000000 / CLOCKS_PER_SEC;
    cout << "Then, it took " << tFound << " microseconds to find " << tDevil->name << endl;
    vector<Track*> similarTractors = sTree.GetSimilarTracks(tDevil, TESTING_TEMPO, TESTING_KEY, TESTING_DANCEABILITY, TESTING_ENERGY);
    for(auto it : similarTractors) {
        cout << "Similar track found: " << it->name << " by ";
        for(auto aIt : it->artists) {
            cout << aIt << " ";
        }
        cout << endl;
    }
    t8 = clock();
    float tSimilar = ((float)t8 - (float)t7)/CLOCKS_PER_SEC;
    cout << "Wow, it took " << similar << " seconds to find all these similar songs." << endl;

    return 0;
}
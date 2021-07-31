#pragma once
using namespace std;
#include <vector>
#include <string>
struct Track {
    Track(string _id, string _name, vector<string> _artists, unsigned int _danceability,
          unsigned int _energy, unsigned char _tempo, unsigned char _key, bool _mode,
          unsigned char _timeSig) :
            id(_id),
            name(_name),
            artists(_artists),
            danceability(_danceability),
            energy(_energy),
            tempo(_tempo),
            key(_key),
            mode(_mode),
            timeSig(_timeSig)
            {} // Member initializer for constructors with all criteria.
    string id, name;
    vector<string> artists;
    unsigned int danceability, energy;
    unsigned char tempo, key, timeSig;
    bool mode;
};

class SongHash { // Implement a hash table.
  public:
    void AddTrack(Track t);
    SongHash();
  private:

};

class SongTree { // Implement a tree.
  public:
    void AddTrack(Track t);
    SongTree();
  private:
    
};
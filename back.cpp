// Howdy, I'm the back-end code.
#pragma once
using namespace std;
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
class Track {
  public:
    Track(); // Probably will go unused.
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
    // Simple getters. Will likely not need to re-set any values.
    string GetID() { return id; }
    string GetName() { return name; }
    vector<string> GetArtists() { return artists; }
    unsigned int GetDanceability() { return danceability; }
    unsigned int GetEnergy() { return energy; }
    unsigned char GetTempo() { return tempo; }
    unsigned char GetKey() { return key; }
    bool GetMode() { return mode; }
    unsigned char GetTimeSig() { return timeSig; }

  private:
    string id, name;
    vector<string> artists;
    unsigned int danceability, energy;
    unsigned char tempo, key, timeSig;
    bool mode;
};

class SongUM {
  public:
    void AddTrack(Track t);
  private:
    unordered_map<string, Track> internal; // Maps ID strings to Tracks.
};

class SongTree {
  public:
    void AddTrack(Track t);
  private:
    map<string, Track> internal; // Maps ID strings to Tracks.
};
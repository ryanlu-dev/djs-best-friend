#pragma once
using namespace std;
#include <queue>
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
    SongHash();
    unsigned int GetHash(string s);
    Track* GetTrack(unsigned int hID, string ID);
    vector<Track*> GetSimilarTracks(Track* t, float tempoPct, float keyPct, float danceabilityPct, float energyPct);
  private:
    vector<Track*>* internal;
};

class TreeNode {
  string id;
  Track* tr;
 public:
  TreeNode();
  TreeNode(string n, Track* tr);
  TreeNode* left;
  TreeNode* right;
  int GetHeight();
  int GetBal();
  void SetNode(TreeNode* passed);
  string GetID();
  Track* GetTrack();
  void SetNameID(string n, unsigned int id);
  TreeNode* LeftRotate();
  TreeNode* RightRotate();
};

class SongTree { // Implement a tree.
  public:
    SongTree();
    TreeNode* GetRoot();
    int GetNodes();
    void Insert(string id, Track* tr);
    TreeNode* Search(TreeNode* rt, string id);
    TreeNode* nInsert(TreeNode* rt, string id, Track* tr);
    vector<Track*> GetSimilarTracks(Track* t, float tempoPct, float keyPct, float danceabilityPct, float energyPct);
    void TraverseInOrder(TreeNode* rt, vector<Track*>& similars, Track* tr, unsigned char a, 
                         unsigned char b, unsigned char aa, unsigned char bb, unsigned int aaa, 
                         unsigned int bbb, unsigned int aaaa, unsigned int bbbb, vector<short> c);
  private:
    TreeNode* root;
};
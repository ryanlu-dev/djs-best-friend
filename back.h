#pragma once
#include <string>
#include <vector>
struct Track {
    Track(std::string _id, std::string _name, std::vector<std::string> _artists, unsigned int _danceability,
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
    std::string id, name;
    std::vector<std::string> artists;
    unsigned int danceability, energy;
    unsigned char tempo, key, timeSig;
    bool mode;
};

class SongHash { // Implement a hash table.
  public:
    SongHash();
    unsigned int GetHash(std::string s);
    Track* GetTrack(unsigned int hID, std::string ID);
    std::vector<Track*> GetSimilarTracks(Track* t, float tempoPct, float keyPct, float danceabilityPct, float energyPct);
  private:
    std::vector<Track*>* internal;
};

class TreeNode {
  std::string id;
  Track* tr;
 public:
  TreeNode();
  TreeNode(std::string n, Track* tr);
  TreeNode* left;
  TreeNode* right;
  int GetHeight();
  int GetBal();
  void SetNode(TreeNode* passed);
  std::string GetID();
  Track* GetTrack();
  void SetNameID(std::string n, unsigned int id);
  TreeNode* LeftRotate();
  TreeNode* RightRotate();
};

class SongTree { // Implement a tree.
  public:
    SongTree();
    TreeNode* GetRoot();
    int GetNodes();
    void Insert(std::string id, Track* tr);
    TreeNode* Search(TreeNode* rt, std::string id);
    TreeNode* nInsert(TreeNode* rt, std::string id, Track* tr);
    std::vector<Track*> GetSimilarTracks(Track* t, float tempoPct, float keyPct, float danceabilityPct, float energyPct);
    void TraverseInOrder(TreeNode* rt, std::vector<Track*>& similars, Track* tr, unsigned char a, 
                         unsigned char b, unsigned char aa, unsigned char bb, unsigned int aaa, 
                         unsigned int bbb, unsigned int aaaa, unsigned int bbbb, std::vector<short> c);
  private:
    TreeNode* root;
};
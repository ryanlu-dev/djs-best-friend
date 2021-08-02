// Howdy, I'm the back-end code.
#include "back.h"
#include "MurmurHash3.cpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#define BUCKET_AMT 70000

 // Constructing the hashtable parses relevant values from the data set and
 // places them into the table as Track objects.
SongHash::SongHash() {
    internal = new vector<Track*>[BUCKET_AMT];
    /*
        ==Parsing the data==
        ids and names are stored as strings.
        artists will be stored as a vector of strings.
        danceability and energy will be stored as unsigned ints.
        tempo, keys, and time signatures will be stored as unsigned chars.
        modes will be stored as bools.

        NOTES:
        danceability and energy will be multiplied by 10,000 to avoid 
        having to use a float as a key for either map due to floating point
        precision issues.

        tempo will be rounded to the nearest whole number.

        two keys will be similar if they are within 2 in either direction
        (e.g., 4 is similar to 5, 8 is similar to 6, 0 is similar to 11, and
        11 is similar to 1 as it wraps around)

        tracks must be in the same time signature and mode to be similar.
    */
    ifstream input;
    input.open("assets/US.csv");
    if(!input.is_open()) {
        cerr << "Error: File not found or unable to be opened." << endl;
        input.close();
    }
    int row = 2;
    string trash;
    getline(input, trash);
    
    while(!input.eof()) {
        // Reading in the ID, only alphanumeric. Pretty simple.
        string id;
        getline(input, id, ',');
        //cout << "The ID at " << row << " is " << id << endl;

        /*  Reading in the name of the track, may contain commas or quotes.
            If it begins with a quote, then there must be a quote inside the
            string or a comma or both. */
        string name, toAdd;
        bool special = false;
        bool doubleQuotes = false;
        if(input.get() != '\"') {
            special = false;
            input.unget();
        }
        else {
            special = true;
        }
        if(!special)
            getline(input, name, ',');
        else {
            // Fill into end of column
            do {
                getline(input, toAdd, '\"');
                name.append(toAdd);
                name += '\"';
            } while (input.get() == '\"');
            // Remove double-quotes
            do {
                if(name.find("\"\"") != string::npos) {
                    doubleQuotes = true;
                    name.replace(name.find("\"\""), 2, "\"");
                }
                else {
                    doubleQuotes = false;
                }
            } while (doubleQuotes);
            // Remove ending quote
            name = name.substr(0, name.length() - 1);
        }
        //cout << "The name at " << row << " is " << name << endl;
        
        // Skipping three columns to get to artists.
        for(int i = 0; i < 3; i++)
            getline(input, trash, ',');
        
        /*  Reading in the artists.
            Single artists with no apostrophes - ['NAME'],
            Multiple artists with no apostrophes - "['NAME1', 'NAME2', 'NAME3']",
            Single artists with apostrophes - "[""NA'ME""]",
            Multiple artists, first with apostrophes - "[""NA'ME1"", 'NAME2']",
            Multiple artists, with multiple apostrophes:
                "[""NA'ME1"", 'NAME2', ""NA'ME3"", ""NA'ME4"", 'NAME5']",
            Multiple artists, with the second having quotes:
                "['NAME1', 'NAME2 ""Namelicious"" NAME3']", */

        vector<string> artists;
        string artist;
        getline(input, artist, ',');
        if(artist.at(0) == '[') { // Single artist, no apostrophe
            artist = artist.substr(2, (artist.length() - 4));
            artists.push_back(artist);
            special = false;
        }
        else // Must be multiple artists or a single artist with apostrophe
            special = true;
        if(special) {
            // Fill into end of column
            do {
                input.unget();
                getline(input, toAdd, ',');
                artist.append(toAdd);
                artist += ',';
            } while (input.get() == ' ');
            // Remove double-quotes
            do {
                if(artist.find("\"\"") != string::npos) {
                    doubleQuotes = true;
                    artist.replace(artist.find("\"\""), 2, "\"");
                }
                else {
                    doubleQuotes = false;
                }
            } while (doubleQuotes);
            // Remove starting and ending quotes and brackets
            artist = artist.substr(2, artist.length() - 4);
            // Parsing name that either starts with a quote or apostrophe.
            string thisArtist;
            while(special) {
                thisArtist = artist.substr(1);
                int charPos;
                if(artist.at(0) == '\"')
                    charPos = thisArtist.find('\"');
                else if(artist.at(0) == '\'' && artist.find('\\') == string::npos)
                    charPos = thisArtist.find('\'');
                else if(artist.at(0) == '\'' && artist.find('\\') != string::npos)
                    charPos = thisArtist.rfind('\'');
                thisArtist = thisArtist.substr(0, charPos);
                artists.push_back(thisArtist);
                if(thisArtist != artist.substr(1, artist.length() - 3))
                    artist = artist.substr(charPos + 4);
                else
                    special = false;
            }
        }
        
        /* cout << "The artist(s) associated with " << row << " is ";
        int i = 1;
        for(auto it : artists) {
            if(i != 1)
                cout << ", ";
            cout << it;
            if(i == artists.size())
                cout << endl;
            i++;
        } */

        // Skipping the artist_id and release_date columns
        input.unget();
        if(input.get() == '\"') {
            getline(input, trash, '\"');
            input.get();
        }
        else {
            getline(input, trash, ',');
        }
        //cout << "Skipped artist_id: " << trash << endl;
        getline(input, trash, ',');
        //cout << "Skipped release_date: " << trash << endl;

        // Reading in the danceability
        string danceabilityS;
        getline(input, danceabilityS, ',');
        float danceabilityF = stof(danceabilityS);
        unsigned int danceability = (10000 * danceabilityF);
        //cout << "The danceability of " << row << " is " << danceability << endl;
        
        // Reading in the energy
        string energyS;
        getline(input, energyS, ',');
        float energyF = stof(energyS);
        unsigned int energy = (10000 * energyF);
        //cout << "The energy of " << row << " is " << energy << endl;

        // Reading in the key
        string keyS;
        getline(input, keyS, ',');
        unsigned char key = stoi(keyS);
        //cout << "The key of " << row << " is " << (int)key << endl;

        // Skipping loudness
        getline(input, trash, ',');

        // Reading in the mode
        string modeS;
        getline(input, modeS, ',');
        bool mode = stoi(modeS);
        //cout << "The mode of " << row << " is ";
        //(mode) ? cout << "major" << endl : cout << "minor" << endl;

        // Skipping 5 columns
        for(int i = 0; i < 5; i++)
            getline(input, trash, ',');

        // Reading tempo
        string tempoS;
        getline(input, tempoS, ',');
        unsigned char tempo = stoi(tempoS);
        //cout << "The tempo of " << row << " is " << (int)tempo << endl;

        // Reading time signature
        string timeSigS;
        getline(input, timeSigS, '\n');
        unsigned char timeSig = stoi(timeSigS);
        //cout << "The time signature of " << row << " is " << (int)timeSig << endl;

        // Turning this track into an object.
        Track* thisTrack = new Track(id, name, artists, danceability, energy, tempo, key, mode, timeSig);
        unsigned int index = GetHash(id);
        internal[index].push_back(thisTrack);

        if(row == 93136) // How many rows to go through
            break;
        row++;
    }
    input.close();
}

unsigned int SongHash::GetHash(string s) {
    // Hashing this ID into an unsigned 32-bit integer (uint32_t)
    const char* cID = s.c_str();
    uint32_t hashedID[1];
    MurmurHash3_x86_32(cID, 22, 1, hashedID);
    return hashedID[0] % BUCKET_AMT;
}

Track* SongHash::GetTrack(unsigned int hID, string ID) {
    for(auto it : internal[hID]) {
        if(it->id == ID)
            return it;
    }
}

vector<Track*> SongHash::GetSimilarTracks(Track* t, float tempoPct, float keyPct, float danceabilityPct, float energyPct) {
    vector<Track*> similarTracks;
    unsigned char tempoUpper, tempoLower, keyUpper, keyLower, keyToChange;
    unsigned int danceabilityUpper, danceabilityLower, energyUpper, energyLower;
    unsigned char acceptableKeys[5];

    tempoUpper = (1 + tempoPct) * t->tempo;
    tempoLower = (1 - tempoPct) * t->tempo;

    if(keyPct > 0.1333) {
        keyToChange = 2;
    }
    else if(keyPct <= 0.1333 && keyPct > 0.06) {
        keyToChange = 1;
    }
    else {
        keyToChange = 0;
    }
    keyUpper = t->key + keyToChange;
    keyLower = t->key - keyToChange;
    if(keyUpper > 11) {
        keyUpper -= 12;
    }
    if(keyLower < 0) {
        keyLower += 12;
    }
    acceptableKeys[0] = t->key;
    acceptableKeys[1] = keyUpper;
    acceptableKeys[2] = keyLower;

    if(t->key == 11 && keyToChange == 2) {
        acceptableKeys[3] = 0;
        acceptableKeys[4] = 10;
    }
    else if(t->key == 10 && keyToChange == 2) {
        acceptableKeys[3] = 11;
        acceptableKeys[4] = 9;
    }
    else if(t->key == 0 && keyToChange == 2) {
        acceptableKeys[3] = 1;
        acceptableKeys[4] = 11;
    }
    else if(t->key == 1 && keyToChange == 2) {
        acceptableKeys[3] = 2;
        acceptableKeys[4] = 0;
    }
    
    danceabilityUpper = (1 + danceabilityPct) * t->danceability;
    danceabilityLower = (1 - danceabilityPct) * t->danceability;
    if(danceabilityUpper > 10000)
        danceabilityUpper = 10000;

    energyUpper = (1 + energyPct) * t->energy;
    energyLower = (1 - energyPct) * t->energy;
    if(energyUpper > 10000)
        energyUpper = 10000;

    for(int i = 0; i < BUCKET_AMT; i++) { // Iterating through every bucket
        for(auto x : internal[i]) { // Iterating through every track in bucket
            if(t->mode == x->mode && t->timeSig == x->timeSig) { // Matching time signature and mode
                for(auto k : acceptableKeys) { 
                    if(x->key == k) { // Key is within acceptable range
                        if(x->tempo > tempoLower && x->tempo < tempoUpper) { // Tempo is within acceptable range
                            if(x->danceability > danceabilityLower && x->danceability < danceabilityUpper) { // Danceability is within acceptable range
                                if(x->energy > energyLower && x->energy < energyUpper) { // Energy is within acceptable range
                                    bool duplicate = false;
                                    for(auto d : similarTracks) {
                                        if(d->name == x->name) {
                                            duplicate = true;
                                            break;
                                        }
                                    }
                                    if(!duplicate && t->name != x->name)
                                        similarTracks.push_back(x);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return similarTracks;
}

SongTree::SongTree() {
    root = nullptr;
    /*
        ==Parsing the data==
        ids and names are stored as strings.
        artists will be stored as a vector of strings.
        danceability and energy will be stored as unsigned ints.
        tempo, keys, and time signatures will be stored as unsigned chars.
        modes will be stored as bools.

        NOTES:
        danceability and energy will be multiplied by 10,000 to avoid 
        having to use a float as a key for either map due to floating point
        precision issues.

        tempo will be rounded to the nearest whole number.

        two keys will be similar if they are within 2 in either direction
        (e.g., 4 is similar to 5, 8 is similar to 6, 0 is similar to 11, and
        11 is similar to 1 as it wraps around)

        tracks must be in the same time signature and mode to be similar.
    */
    ifstream input;
    input.open("assets/US.csv");
    if(!input.is_open()) {
        cerr << "Error: File not found or unable to be opened." << endl;
        input.close();
    }
    int row = 2;
    string trash;
    getline(input, trash);
    
    while(!input.eof()) {
        // Reading in the ID, only alphanumeric. Pretty simple.
        string id;
        getline(input, id, ',');
        //cout << "The ID at " << row << " is " << id << endl;

        /*  Reading in the name of the track, may contain commas or quotes.
            If it begins with a quote, then there must be a quote inside the
            string or a comma or both.
        */
        string name, toAdd;
        bool special = false;
        bool doubleQuotes = false;
        if(input.get() != '\"') {
            special = false;
            input.unget();
        }
        else {
            special = true;
        }
        if(!special)
            getline(input, name, ',');
        else {
            // Fill into end of column
            do {
                getline(input, toAdd, '\"');
                name.append(toAdd);
                name += '\"';
            } while (input.get() == '\"');
            // Remove double-quotes
            do {
                if(name.find("\"\"") != string::npos) {
                    doubleQuotes = true;
                    name.replace(name.find("\"\""), 2, "\"");
                }
                else {
                    doubleQuotes = false;
                }
            } while (doubleQuotes);
            // Remove ending quote
            name = name.substr(0, name.length() - 1);
        }
        //cout << "The name at " << row << " is " << name << endl;
        
        // Skipping three columns to get to artists.
        for(int i = 0; i < 3; i++)
            getline(input, trash, ',');
        
        /*  Reading in the artists.
            Single artists with no apostrophes - ['NAME'],
            Multiple artists with no apostrophes - "['NAME1', 'NAME2', 'NAME3']",
            Single artists with apostrophes - "[""NA'ME""]",
            Multiple artists, first with apostrophes - "[""NA'ME1"", 'NAME2']",
            Multiple artists, with multiple apostrophes:
                "[""NA'ME1"", 'NAME2', ""NA'ME3"", ""NA'ME4"", 'NAME5']",
            Multiple artists, with the second having quotes:
                "['NAME1', 'NAME2 ""Namelicious"" NAME3']", */
        vector<string> artists;
        string artist;
        getline(input, artist, ',');
        if(artist.at(0) == '[') { // Single artist, no apostrophe
            artist = artist.substr(2, (artist.length() - 4));
            artists.push_back(artist);
            special = false;
        }
        else // Must be multiple artists or a single artist with apostrophe
            special = true;
        if(special) {
            // Fill into end of column
            do {
                input.unget();
                getline(input, toAdd, ',');
                artist.append(toAdd);
                artist += ',';
            } while (input.get() == ' ');
            // Remove double-quotes
            do {
                if(artist.find("\"\"") != string::npos) {
                    doubleQuotes = true;
                    artist.replace(artist.find("\"\""), 2, "\"");
                }
                else {
                    doubleQuotes = false;
                }
            } while (doubleQuotes);
            // Remove starting and ending quotes and brackets
            artist = artist.substr(2, artist.length() - 4);
            // Parsing name that either starts with a quote or apostrophe.
            string thisArtist;
            while(special) {
                thisArtist = artist.substr(1);
                int charPos;
                if(artist.at(0) == '\"')
                    charPos = thisArtist.find('\"');
                else if(artist.at(0) == '\'' && artist.find('\\') == string::npos)
                    charPos = thisArtist.find('\'');
                else if(artist.at(0) == '\'' && artist.find('\\') != string::npos)
                    charPos = thisArtist.rfind('\'');
                thisArtist = thisArtist.substr(0, charPos);
                artists.push_back(thisArtist);
                if(thisArtist != artist.substr(1, artist.length() - 3))
                    artist = artist.substr(charPos + 4);
                else
                    special = false;
            }
        }
        
        /* cout << "The artist(s) associated with " << row << " is ";
        int i = 1;
        for(auto it : artists) {
            if(i != 1)
                cout << ", ";
            cout << it;
            if(i == artists.size())
                cout << endl;
            i++;
        } */

        // Skipping the artist_id and release_date columns
        input.unget();
        if(input.get() == '\"') {
            getline(input, trash, '\"');
            input.get();
        }
        else {
            getline(input, trash, ',');
        }
        //cout << "Skipped artist_id: " << trash << endl;
        getline(input, trash, ',');
        //cout << "Skipped release_date: " << trash << endl;

        // Reading in the danceability
        string danceabilityS;
        getline(input, danceabilityS, ',');
        float danceabilityF = stof(danceabilityS);
        unsigned int danceability = (10000 * danceabilityF);
        //cout << "The danceability of " << row << " is " << danceability << endl;
        
        // Reading in the energy
        string energyS;
        getline(input, energyS, ',');
        float energyF = stof(energyS);
        unsigned int energy = (10000 * energyF);
        //cout << "The energy of " << row << " is " << energy << endl;

        // Reading in the key
        string keyS;
        getline(input, keyS, ',');
        unsigned char key = stoi(keyS);
        //cout << "The key of " << row << " is " << (int)key << endl;

        // Skipping loudness
        getline(input, trash, ',');

        // Reading in the mode
        string modeS;
        getline(input, modeS, ',');
        bool mode = stoi(modeS);
        //cout << "The mode of " << row << " is ";
        //(mode) ? cout << "major" << endl : cout << "minor" << endl;

        // Skipping 5 columns
        for(int i = 0; i < 5; i++)
            getline(input, trash, ',');

        // Reading tempo
        string tempoS;
        getline(input, tempoS, ',');
        unsigned char tempo = stoi(tempoS);
        //cout << "The tempo of " << row << " is " << (int)tempo << endl;

        // Reading time signature
        string timeSigS;
        getline(input, timeSigS, '\n');
        unsigned char timeSig = stoi(timeSigS);
        //cout << "The time signature of " << row << " is " << (int)timeSig << endl;

        // Turning this track into an object.
        Track* thisTrack = new Track(id, name, artists, danceability, energy, tempo, key, mode, timeSig);
        Insert(id, thisTrack);
        if(row == 93136) // How many rows to go through
            break;
        row++;
    }
    input.close();
}

TreeNode* SongTree::GetRoot() { return root; }

void SongTree::Insert(string id, Track* tr) {
    root = nInsert(root, id, tr);
}

TreeNode* SongTree::nInsert(TreeNode* rt, string id, Track* tr) {
    // If rt is null, make a new TreeNode and return it
    if(!rt) {
        return new TreeNode(id, tr);
    }

    // If the passed ID is less than or greater than the current node's ID, delve
    // in that direction in the tree.
    if(id < rt->GetID()) {
        rt->left = nInsert(rt->left, id, tr);
    }
    else {
        rt->right = nInsert(rt->right, id, tr);
    }
    return rt;
}

TreeNode* SongTree::Search(TreeNode* rt, string id) {
    if(!rt) {
        //cout << "I give up." << endl;
        return nullptr;
    }
    if(rt->GetID() == id) {
        return rt;
    }
    else if(rt->GetID() < id) {
        return Search(rt->right, id);
    }
    else if(rt->GetID() > id) {
        return Search(rt->left, id);
    }
}

vector<Track*> SongTree::GetSimilarTracks(Track* t, float tempoPct, float keyPct, float danceabilityPct, float energyPct) {
    vector<Track*> similarTracks;
    unsigned char tempoUpper, tempoLower, keyUpper, keyLower, keyToChange;
    unsigned int danceabilityUpper, danceabilityLower, energyUpper, energyLower;
    vector<short> acceptableKeys;

    tempoUpper = (1 + tempoPct) * t->tempo;
    tempoLower = (1 - tempoPct) * t->tempo;

    if(keyPct > 0.1333) {
        keyToChange = 2;
    }
    else if(keyPct <= 0.1333 && keyPct > 0.06) {
        keyToChange = 1;
    }
    else {
        keyToChange = 0;
    }
    keyUpper = t->key + keyToChange;
    keyLower = t->key - keyToChange;
    if(keyUpper > 11) {
        keyUpper -= 12;
    }
    if(keyLower < 0) {
        keyLower += 12;
    }
    acceptableKeys.push_back(t->key);
    acceptableKeys.push_back(keyUpper);
    acceptableKeys.push_back(keyLower);

    if(t->key == 11 && keyToChange == 2) {
        acceptableKeys.push_back(0);
        acceptableKeys.push_back(10);
    }
    else if(t->key == 10 && keyToChange == 2) {
        acceptableKeys.push_back(11);
        acceptableKeys.push_back(9);
    }
    else if(t->key == 0 && keyToChange == 2) {
        acceptableKeys.push_back(1);
        acceptableKeys.push_back(11);
    }
    else if(t->key == 1 && keyToChange == 2) {
        acceptableKeys.push_back(2);
        acceptableKeys.push_back(0);
    }
    
    danceabilityUpper = (1 + danceabilityPct) * t->danceability;
    danceabilityLower = (1 - danceabilityPct) * t->danceability;
    if(danceabilityUpper > 10000)
        danceabilityUpper = 10000;

    energyUpper = (1 + energyPct) * t->energy;
    energyLower = (1 - energyPct) * t->energy;
    if(energyUpper > 10000)
        energyUpper = 10000;

    TreeNode* currNode = root;
    TraverseInOrder(currNode, similarTracks, t, tempoUpper, tempoLower, keyUpper, keyLower, danceabilityUpper, danceabilityLower, energyUpper, energyLower, acceptableKeys);

    return similarTracks;
}

void SongTree::TraverseInOrder(TreeNode* rt, vector<Track*>& similars, Track* tr, unsigned char a, 
                         unsigned char b, unsigned char aa, unsigned char bb, unsigned int aaa, 
                         unsigned int bbb, unsigned int aaaa, unsigned int bbbb, vector<short> c) {
    if(!rt)
        return;
    else {
        TraverseInOrder(rt->left, similars, tr, a, b, aa, bb, aaa, bbb, aaaa, bbbb, c);
        if(tr->mode == rt->GetTrack()->mode && tr->timeSig == rt->GetTrack()->timeSig) { // This is going to get really ugly.
            for(auto k : c) {
                if(rt->GetTrack()->key == k) {
                    if(rt->GetTrack()->tempo > b && rt->GetTrack()->tempo < a) {
                        if(rt->GetTrack()->danceability > bbb && rt->GetTrack()->danceability < aaa) {
                            if(rt->GetTrack()->energy > bbbb && rt->GetTrack()->energy < aaaa) {
                                bool duplicate = false;
                                for(auto d : similars) {
                                    if(d->name == rt->GetTrack()->name) {
                                        duplicate = true;
                                        break;
                                    }
                                }
                                if(!duplicate && tr->name != rt->GetTrack()->name) {
                                    similars.push_back(rt->GetTrack());
                                }
                            }
                        }
                    }
                }
            }
        }
        TraverseInOrder(rt->right, similars, tr, a, b, aa, bb, aaa, bbb, aaaa, bbbb, c);
    }
}

TreeNode::TreeNode(string _id, Track* _tr) 
    : id(_id),
      tr(_tr),
      left(nullptr),
      right(nullptr) {}

string TreeNode::GetID() { return id; }

Track* TreeNode::GetTrack() { return tr; }

TreeNode* TreeNode::LeftRotate() {
    TreeNode* newParent = right;
    TreeNode* grandchild = right->left;
    right = grandchild;
    newParent->left = this;
    return newParent;
}

TreeNode* TreeNode::RightRotate() {
    TreeNode* newParent = left;
    TreeNode* grandchild = left->right;
    left = grandchild;
    newParent->right = this;
    return newParent;
}
// Hello, I'm the driver.
#include "back.cpp"
#include "front.cpp"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

/*
    Default data structure:
        Unordered map with keys of Spotify IDs and values of Track objects,
        implemented as a hash table by STL.
    Alternative data structure:
        Map with keys of Spotify IDs and values of Track objects, implemented
        as a Red-Black Tree by STL.
*/

int main() {
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
        return 1;
    }
    int x = 2;
    string trash;
    getline(input, trash);
    while(!input.eof()) {
        // Reading in the ID, only alphanumeric. Pretty simple.
        string id;
        getline(input, id, ',');

        // Reading in the name of the track, may contain commas or quotes.
        // TODO: Refactor?
        bool special = false;
        bool wentThruSpecial = false;
        bool doubleQuotes = false;
        string name;
        string toAdd;
            getline(input, name, ',');
            if(name.at(0) == '\"')
                special = true;
        if(special)
            name = name.substr(1);
        while(special) {
            do {
                if(name.find("\"\"") != string::npos) {
                    doubleQuotes = true;
                    name.replace(name.find("\"\""), 2, "\"");
                }
                else {
                    int quoteCount = 0;
                    for(int i = 0; i < name.length() - 1; i++) {
                        if(name.at(i) == '\"')
                            quoteCount++;
                    }
                    if(quoteCount % 2 == 1)
                        name += '\"';
                    doubleQuotes = false;
                }
            } while(doubleQuotes);
            if(input.get() != ' ') {
                wentThruSpecial = true;
                break;
            }
            input.unget();
            getline(input, toAdd, '\"');
            name.append(toAdd);
            name += '\"';
            if(input.get() == ',') {
                if(input.get() == ' ') {
                    name += ", ";
                    getline(input, toAdd, ',');
                }
                else
                    special = false;
            }
            wentThruSpecial = true;
        }
        if(wentThruSpecial)
            name = name.substr(0, name.length() - 1);
        wentThruSpecial = false;
        
        // Skipping three columns
        for(int i = 0; i < 3; i++)
            getline(input, trash, ',');
        
        // Reading in the artists.
        // Single artists with no apostrophes - ['NAME'],
        // Multiple artists with no apostrophes - "['NAME1', 'NAME2', 'NAME3']",
        // Single artists with apostrophes - "[""NA'ME""]",
        // Multiple artists, first with apostrophes - "[""NA'ME1"", 'NAME2']",
        // Multiple artists, with multiple apostrophes:
        //  "[""NA'ME1"", 'NAME2', ""NA'ME3"", ""NA'ME4"", 'NAME5']",
        // Multiple artists, with the second having quotes:
        //  "['NAME1', 'NAME2 ""Namelicious"" NAME3']",
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
                else if(artist.at(0) == '\'')
                    charPos = thisArtist.find('\'');
                thisArtist = thisArtist.substr(0, charPos);
                artists.push_back(thisArtist);
                if(thisArtist != artist.substr(1, artist.length() - 3))
                    artist = artist.substr(charPos + 4);
                else
                    special = false;
            }
        }
        /* TESTING
        cout << "The ID at " << x << " is " << id << endl;
        cout << "The name at " << x << " is " << name << endl;
        cout << "The artist(s) associated with " << x << " is ";
        int i = 1;
        for(auto it : artists) {
            if(i != 1)
                cout << ", ";
            cout << it;
            if(i == artists.size())
                cout << endl;
            i++;
        }

        if(x == 100) // How many rows to go through
            break;
        x++;
        getline(input, id);
        */
    }
    return 0;
}
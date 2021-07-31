// Howdy, I'm the back-end code.
#include "back.h"
#include <fstream>
#include <iostream>
#include <iomanip>
 // Constructing the hashtable parses relevant values from the data set and
 // places them into the table as Track objects.
SongHash::SongHash() {
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
                //cout << "Working name: " << name << endl;
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
                "['NAME1', 'NAME2 ""Namelicious"" NAME3']",
        */

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

        if(row == 93136) // How many rows to go through
            break;
        row++;
    }
    input.close();
}
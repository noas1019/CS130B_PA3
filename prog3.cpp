#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <vector>

using namespace std;

//InputParser function by borrowed from 
class InputParser {
public:
    InputParser(int& argc, char** argv) {
        for (int i = 1; i < argc; ++i)
            this->tokens.push_back(string(argv[i]));
    }
    /// @author iain
    const string& getCmdOption(const string& option) const {
        vector<string>::const_iterator itr;
        itr = find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const string empty_string("");
        return empty_string;
    }
    /// @author iain
    bool cmdOptionExists(const string& option) const {
        return find(this->tokens.begin(), this->tokens.end(), option)
            != this->tokens.end();
    }
private:
    vector <string> tokens;
};

double getScore(char a, char b, double m, double c) {
    if (a == b) {
        return m;
    }
    else {
        return c;
    }
}

int main(int argc, char** argv) {
    double m=2, c=-0.5, d=-1;
    InputParser input(argc, argv);
    if (input.cmdOptionExists("-m")) {
        m = atof(input.getCmdOption("-m").c_str());
    }
    if (input.cmdOptionExists("-c")) {
        c = atof(input.getCmdOption("-c").c_str());
    }
    if (input.cmdOptionExists("-d")) {
        d = atof(input.getCmdOption("-d").c_str());
    }
    string filename1 = input.getCmdOption("-1");
    string filename2 = input.getCmdOption("-2");
    size_t found1 = filename1.find(".txt");
    size_t found2 = filename2.find(".txt");
    if (found1 == string::npos) {
        filename1 = filename1 + ".txt";
    }
    if (found2 == string::npos) {
        filename2 = filename2 + ".txt";
    }

    string DNA1;
    string DNA2;
    fstream myfile;
    myfile.open(filename1);
    if (myfile.is_open()) {
        getline(myfile, DNA1);
        myfile.close();
    }
    myfile.open(filename2);
    if (myfile.is_open()) {
        getline(myfile, DNA2);
        myfile.close();
    }

    int xLength = DNA1.length() + 1;
    int yLength = DNA2.length() + 1;
    double ScoreMatrix[yLength][xLength];
    for (int i = 0; i < xLength; i++) {
        ScoreMatrix[0][i] = d * i;
    }
    for (int j = 0; j < yLength; j++) {
        ScoreMatrix[j][0] = d * j; 
    }
    double match, skip1, skip2, change;
    for (int j = 1; j < yLength; j++) {
        for (int i = 1; i < xLength; i++) {
            match = ScoreMatrix[j - 1][i - 1] + getScore(DNA1[i - 1], DNA2[j - 1], m, c);
            skip1 = ScoreMatrix[j - 1][i] + d;
            skip2 = ScoreMatrix[j][i - 1] + d;
            if (j == yLength-1 && i == xLength-1) {
                cout << "match: " << match << endl;
                cout << "skip1: " << skip1 << endl;
                cout << "skip2: " << skip2 << endl;
            }
            ScoreMatrix[j][i] = (double)max({ match, skip1, skip2});
        }
    }

    int sizeDNA1 = DNA1.length();
    int sizeDNA2 = DNA2.length();
    cout << "DNA1 length: " << sizeDNA1 << endl;
    cout << "DNA2 length: " << sizeDNA2 << endl;
    string align_DNA1 = "";
    string align_DNA2 = "";
    while (sizeDNA1 > 0 || sizeDNA2 > 0) {
        if ((sizeDNA1 > 0 && sizeDNA2 > 0) && DNA1[sizeDNA1-1] == DNA2[sizeDNA2-1]) {
            align_DNA1 = DNA1[sizeDNA1 - 1] + align_DNA1;
            align_DNA2 = DNA2[sizeDNA2 - 1] + align_DNA2;
            sizeDNA1 = sizeDNA1 - 1;
            sizeDNA2 = sizeDNA2 - 1;
        }
        else if ((sizeDNA1 > 0 && sizeDNA2 > 0) && (ScoreMatrix[sizeDNA2][sizeDNA1] == ScoreMatrix[sizeDNA2 - 1][sizeDNA1 - 1] + c)) {
            align_DNA1 = DNA1[sizeDNA1 - 1] + align_DNA1;
            align_DNA2 = 'R' + align_DNA2;
            sizeDNA1 = sizeDNA1 - 1;
            sizeDNA2 = sizeDNA2 - 1;
        }
        else if (sizeDNA1 > 0 && ScoreMatrix[sizeDNA2][sizeDNA1] == ScoreMatrix[sizeDNA2][sizeDNA1-1] + d) {
            align_DNA1 = DNA1[sizeDNA1 - 1] + align_DNA1;
            align_DNA2 = "_" + align_DNA2;
            sizeDNA1 = sizeDNA1 - 1;
        }
        else if (sizeDNA2 > 0 && ScoreMatrix[sizeDNA2][sizeDNA1] == ScoreMatrix[sizeDNA2-1][sizeDNA1] + d) {
            align_DNA1 = "_" + align_DNA1;
            align_DNA2 = DNA2[sizeDNA2 - 1] + align_DNA2;
            sizeDNA2 = sizeDNA2 - 1;
        }
    }

    double similarityScore = 0;
    for (int i = 0; i < align_DNA1.length(); i++) {
        if (align_DNA2[i] == 'R') {
            similarityScore = c + similarityScore;
        }
        else if (align_DNA1[i] == align_DNA2[i]) {
            similarityScore = m + similarityScore;
        }
        else {
            similarityScore = d + similarityScore;
        }
    }

    cout << "Match measure: " << m << endl;
    cout << "Change measure: " << c << endl;
    cout << "Skip measure: " << d << endl;
    cout << "Filename1: " << filename1 << endl;
    cout << "Filename2: " << filename2 << endl;
    cout << "DNA1: " << DNA1 << endl;
    cout << "DNA2: " << DNA2 << endl;
    for (int j = 0; j < yLength; j++) {
        for (int i = 0; i < xLength; i++) {
            cout << ScoreMatrix[j][i] << " ";
        }
        cout << endl;
    }

    cout << similarityScore << endl;
    cout << align_DNA1 << endl;
    cout << align_DNA2 << endl;

    return 0;
}
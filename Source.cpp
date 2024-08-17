#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include<string>


using namespace std;

//we know that if the plaintext contains J, then it is replaced by I.
const string PlayFaiCipALPHABET = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // so here we write all the alphabet without J 
// Function to create  PlayFair key matrix
// we know in PlayFair Cipher the matrix 5*5

vector<vector<char>> KeyMATRIXcreate() {
    string key;
    vector<vector<char>> MATRIX(5, vector<char>(5));
    string shuffle = PlayFaiCipALPHABET;

    // Remove duplicate characters 
    sort(shuffle.begin(), shuffle.end());
    shuffle.erase(unique(shuffle.begin(), shuffle.end()), shuffle.end());

    while (shuffle.size() < 25) {
        shuffle += 'B';
    }

    key = shuffle.substr(0, 25); 

    int k = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            MATRIX[i][j] = key[k++];
        }
    }

    return MATRIX;
}

string DesignPlainttxt(string text) {

    text.erase(remove_if(text.begin(), text.end(), [](char c) { return !isalpha(c); }), text.end());
    transform(text.begin(), text.end(), text.begin(), ::toupper);

    // here we replace  J with I
    replace(text.begin(), text.end(), 'J', 'I');

    string formattedText;
    for (size_t i = 0; i < text.size(); ++i) {
        formattedText += text[i];
        if (i < text.size() - 1 && text[i] == text[i + 1]) {
            formattedText += 'X';
        }
    }
    if (formattedText.size() % 2 != 0) {
        formattedText += 'X';
    }

    return formattedText;
}

// find  the position of characters 
void Pos(vector<vector<char>>& M, char ch, int& r, int& c) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (M[i][j] == ch) {
                r = i;
                c = j;
                return;
            }
        }
    }
}

//  encrypt a pair of characters
string encryptPair(vector<vector<char>>& M, char a, char b) {
    int ROW1, COL, ROW2, COL2;
    Pos(M, a, ROW1, COL);
    Pos(M, b, ROW2, COL2);

    if (ROW1 == ROW2) {
        return string(1, M[ROW1][(COL + 1) % 5]) + M[ROW2][(COL2 + 1) % 5];
    }
    else if (COL == COL2) {
        return string(1, M[(ROW1 + 1) % 5][COL]) + M[(ROW2 + 1) % 5][COL2];
    }
    else {
        return string(1, M[ROW1][COL2]) + M[ROW2][COL];
    }
}

string encrypt(vector<vector<char>>& M, string PLAINText) {
    PLAINText = DesignPlainttxt(PLAINText);
    string CIPHTXT;

    for (size_t i = 0; i < PLAINText.size(); i += 2) {
        CIPHTXT += encryptPair(M, PLAINText[i], PLAINText[i + 1]);
    }

    return CIPHTXT;
}

//  decryption pair 
string DecryptionPairs(vector<vector<char>>& M, char a, char b) {
    int r, c, ROW2, COL2;
    Pos(M, a, r, c);
    Pos(M, b, ROW2, COL2);

    if (r == ROW2) {
        return string(1, M[r][(c + 4) % 5]) + M[ROW2][(COL2 + 4) % 5];
    }
    else if (c == COL2) {
        return string(1, M[(r + 4) % 5][c]) + M[(ROW2 + 4) % 5][COL2];
    }
    else {
        return string(1, M[r][COL2]) + M[ROW2][c];
    }
}

//  decryption  ciphertext using PlayFair Cipher
string decrypt(vector<vector<char>>& M, string CIPHR) {
    string plaintext;

    for (int i = 0; i < CIPHR.size(); i += 2) {
        plaintext += DecryptionPairs(M, CIPHR[i], CIPHR[i + 1]);
    }

    return plaintext;
}

vector<double> computFreq(string T) {
    vector<double> freqVector(26, 0.0); 

    int totalChars = 0;
    for (char c : T) {
        if (isalpha(c)) {
            freqVector[toupper(c) - 'A']++; 
            totalChars++;
        }
    }

    for (double& freq : freqVector) {
        freq = (freq / totalChars) * 100;
    }

    return freqVector;
}


void printFrequencies(const  vector<double>& freq) {
    cout << "Character Frequency :\n";
    char ltr = 'A';
    for (double freq : freq) {
        cout << ltr << "  : " << freq << endl;
        ltr++;
    }
}
int main() {
    vector<vector<char>> KEY = KeyMATRIXcreate();
    ifstream in("plaintexts.txt");
    if (!in) {
        cerr << "oops !cant opening this fil !\n";
        exit(1);
    }

    vector<string> plaintexts;
    string line;
    while (getline(in, line)) {
        plaintexts.push_back(line);
    }
    in.close();

    ofstream out("ciphertexts.txt");
    if (!out) {
        cerr << "oops !cant opening this fil!\n";
        exit(1);
    }

    vector<string> ciphertexts;
    for (const string& plaintext : plaintexts) {
        string ciphertext = encrypt(KEY, plaintext);
        ciphertexts.push_back(ciphertext);
        cout << "Plaintext: " << plaintext << endl;
        cout << "Ciphertext: " << ciphertext << endl;
        out << ciphertext << endl;
    }
    out.close();

    ifstream encryptedFile("ciphertexts.txt");
    if (!encryptedFile) {
        cerr << "Error in  opening cipher text file!"<<endl;
        exit(1);
    }

    vector<string> decryptedTexts;
    while (getline(encryptedFile, line)) {
        decryptedTexts.push_back(decrypt(KEY, line));
    }
    encryptedFile.close();

    for (const string& decryptedText : decryptedTexts) {
        cout << "Decrypted Text: " << decryptedText << "\n";
    }

    for (const string& ciphertext : ciphertexts) {
        vector<double> freqVect = computFreq(ciphertext);
        cout << endl;
        cout << "Ciphertext: " << ciphertext << endl;
        printFrequencies(freqVect);
    }

    return 0;
}



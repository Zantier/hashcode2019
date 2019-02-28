#include <bits/stdc++.h>
#include <dirent.h>

#define ii pair<int,int>
#define vi vector<int>
#define vii vector<pair<int,int>>

#define g0 get<0>
#define g1 get<1>
#define g2 get<2>
#define g3 get<3>
#define g4 get<4>

#define uset unordered_set
#define umap unordered_map
#define mp make_pair
#define mt make_tuple
#define pub push_back
#define puf push_front
#define pob pop_back
#define pof pop_front
#define all(v) v.begin(),v.end()
#define rall(v) v.rbegin(),v.rend()
#define fr(i,n) for (int i = 0; i < n; i++)

using namespace std;

int bestScore = INT_MIN;

int main(int argc, char* argv[]) {
	// Get input filename from argv
	if (argc < 2) {
		cerr << "need input filename (without .in)" << endl;
		return 1;
	}
	string problemName = argv[1];
	string inFilename = problemName + ".in";
	cerr << "input file: " << inFilename << endl;

	// Read input file
	ifstream inFile;
	inFile.open(inFilename);
	if (!inFile) {
		cerr << "Unable to open " << inFilename << endl;
		return 1;
	}
	int a,b;
	inFile>>a>>b;

	inFile.close();

	// Get best score so far
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (".")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			string filename = ent->d_name;
			string problemDash = problemName + "-";
			if (filename.compare(0, problemDash.length(), problemDash) == 0) {
				string scoreStr = filename.substr(problemDash.length(), filename.length() - problemDash.length() - 4);
				int score = stoi(scoreStr);
				if (score > bestScore) {
					bestScore = score;
				}
			}
		}
		closedir(dir);
	} else {
		cerr << "Could not open directory" << endl;
		return 1;
	}

	cerr << "Best score before starting: " << bestScore << endl;
}

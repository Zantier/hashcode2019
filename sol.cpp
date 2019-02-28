#include <bits/stdc++.h>
#include <dirent.h>
#include <omp.h>

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

struct bound {
    string name;
    float start;
    float end;
    int steps;
};

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

	

	// Bounds
	vector<bound> bs;
	bs.push_back({"t1", 0, 5, 5});
	bs.push_back({"t2", 1, 100, 5});
	// bs.push_back({"t3", 3.2, 34, 8});

	// Grid size
	for (int x = 0; x < 5; x++) {

		auto start = std::chrono::steady_clock::now();

		// Print bounds
		cerr << "bounds:" << endl;
		for (auto b : bs) {
			cerr << b.name << " start: " << b.start << " end: " << b.end << " steps: " << b.steps << endl;
		}

		// Make vectors
		vector<vector<float>> config(1);
		for (auto b : bs) {


			vector<vector<float>> config_tmp;
			for (int i = 0; i < b.steps; i++) {
				float v = b.start + i*(b.end - b.start)/(b.steps-1);

				// Add this val to every element in config already.
				for (int i = 0; i < config.size(); i++) {
					vector<float> c = config[i];
					c.push_back(v);
					config_tmp.push_back(c);
				}

			}
			config = config_tmp;
		}


		// test
		// for (auto v : config) {
		//     for (int i = 0; i < v.size(); i++) {
		//         cerr << bs[i].name << ": " << v[i] << ", ";
		//     }

		//     cerr << endl;
		// }

		// Lock
		omp_lock_t writelock;
		omp_init_lock(&writelock);

		vector<float> best_params;

#pragma omp parallel for
		for (int i = 0; i < config.size(); i++) {
			auto begin = std::chrono::steady_clock::now();
			vector<float> v = config[i];









			// TODO: Solve problem
			float score = 10000 - (v[0] - 3.5)*(v[0] - 3.5) - (v[1] - 60)*(v[1] - 60);







			// output...
			omp_set_lock(&writelock);

			cerr << "score: " << score << "       ";

			for (int i = 0; i < v.size(); i++) {
				cerr << bs[i].name << ": " << v[i] << ", ";
			}

			if (score > bestScore) {
				// TODO: save file
				best_params = v;
				bestScore = score;
			}

			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			float row   = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
			cerr << " took: " << row;

			cerr << endl;
			omp_unset_lock(&writelock);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		float total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
		cerr << "Completed grid!! Time: " << total << " score: " << bestScore << endl;
		cerr << "Best params: ";
		for (float v : best_params)
			cerr << " " << v;
		cerr << endl << endl;

		// TODO: Check this is right lul
		for (int i = 0; i < bs.size(); i++) {
			auto p = best_params[i];
			float inc = (bs[i].end - bs[i].start)/(bs[i].steps-1);

			bs[i].start = p-inc;
			bs[i].end = p+inc;
		}

	}
}

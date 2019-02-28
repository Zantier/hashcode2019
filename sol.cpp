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

umap<string,int> tagMap;


struct photo {
	int id;
	set<int> tags;
};
struct slide {
	set<int> tags;
	vector<int> ids;
};

vector<photo> hs;
vector<photo> vs;
vector<slide> slides;

vector<vector<int>> tagToSlides;

int total_best_score = INT_MIN;

int calcScore(int id1, int id2) {
	auto& tags1 = slides[id1].tags;
	auto& tags2 = slides[id2].tags;
	auto it1 = tags1.begin();
	auto it2 = tags2.begin();

	// diagram order
	vector<int> scores = {0,0,0};

	while (it1 != tags1.end() || it2 != tags2.end()) {
		if (it1 == tags1.end()) {
			scores[2]++;
			it2++;
			continue;
		}
		if (it2 == tags2.end()) {
			scores[0]++;
			it1++;
			continue;
		}

		if (*it1 < *it2) {
			scores[0]++;
			it1++;
			continue;
		}
		if (*it1 > *it2) {
			scores[2]++;
			it2++;
			continue;
		}

		// equal
		scores[1]++;
		it1++;
		it2++;
	}

	int score = *min_element(all(scores));
	return score;
}

int overlap(set<int>& tags1, set<int>& tags2) {
	auto it1 = tags1.begin();
	auto it2 = tags2.begin();

	// diagram order
	vector<int> scores = {0,0,0};

	while (it1 != tags1.end() || it2 != tags2.end()) {
		if (it1 == tags1.end()) {
			scores[2]++;
			it2++;
			continue;
		}
		if (it2 == tags2.end()) {
			scores[0]++;
			it1++;
			continue;
		}

		if (*it1 < *it2) {
			scores[0]++;
			it1++;
			continue;
		}
		if (*it1 > *it2) {
			scores[2]++;
			it2++;
			continue;
		}

		// equal
		scores[1]++;
		it1++;
		it2++;
	}

	return scores[1];
}

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

	int N;
	inFile>>N;
	vector<photo> photos(N);
	int tagIndex = 0;
	for (int i = 0; i < N; i++) {
		photo ph;
		ph.id=i;

		string V;
		inFile>>V;
		bool vertical = V[0] == 'V';
		int tagN;
		inFile>>tagN;
		for (int j = 0; j < tagN; j++) {
			string tag;
			inFile>>tag;

			// not in map
			if (tagMap.find(tag) == tagMap.end()) {
				tagMap[tag] = tagIndex;
				tagIndex++;
			}

			ph.tags.insert(tagMap[tag]);
		}

		if (vertical) {
			vs.pub(ph);
		} else {
			hs.pub(ph);
		}
	}

	// Try to pair verticals with least overlap of tags
	uset<int> visitedVerticals;

	for (int i = 0; i < vs.size(); i++) {
		int min_score = INT_MAX;
		int best_index = -1;
		if (visitedVerticals.find(i) != visitedVerticals.end()) {
			continue;
		}

		for (int j = i+1; j < min(i+100, (int)vs.size()); j++) {
			if (visitedVerticals.find(j) != visitedVerticals.end()) {
				continue;
			}

			int score = overlap(vs[i].tags,vs[j].tags);
			if (score < min_score) {
				min_score = score;
				best_index = j;
			}
		}

		if (best_index != -1) {
			slide sl;
			sl.ids = vector<int>{vs[i].id, vs[best_index].id};
			visitedVerticals.insert(i);
			visitedVerticals.insert(best_index);
			sl.tags = vs[i].tags;
			for (auto& t : vs[best_index].tags) {
				sl.tags.insert(t);
			}
			slides.pub(sl);
		}
	}

	cerr << "Tags: " << tagIndex << endl;
	tagToSlides.resize(tagIndex);

	for (int i = 0; i < hs.size(); i++) {
		slide sl;
		sl.ids = vector<int>{hs[i].id};
		sl.tags = hs[i].tags;
		slides.pub(sl);
	}
	//for (int i = 0; i < vs.size()/2; i++) {
	//	slide sl;
	//	sl.ids = vector<int>{vs[2*i].id, vs[2*i+1].id};
	//	sl.tags = vs[2*i+1].tags;
	//	for (auto& t : vs[2*i].tags) {
	//		sl.tags.insert(t);
	//	}
	//	slides.pub(sl);
	//}

	for (int i = 0; i < slides.size(); i++) {
		slide& sl = slides[i];
		for (auto& t : sl.tags) {
			tagToSlides[t].pub(i);
		}
	}

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
				if (score > total_best_score) {
					total_best_score = score;
				}
			}
		}
		closedir(dir);
	} else {
		cerr << "Could not open directory" << endl;
		return 1;
	}

	cerr << "Best score before starting: " << total_best_score << endl;

	

	// Bounds
	vector<bound> bs;
	//bs.push_back({"t1", 0, 5, 5});
	//bs.push_back({"t2", 1, 100, 5});
	// bs.push_back({"t3", 3.2, 34, 8});

	// Grid iterations
	for (int x = 0; x < 1; x++) {

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

		int best_score = INT_MIN;
		vector<float> best_params;

#pragma omp parallel for
		for (int i = 0; i < config.size(); i++) {
			auto begin = std::chrono::steady_clock::now();
			vector<float> v = config[i];









			// TODO: Solve problem
			//float score = 10000 - (v[0] - 3.5)*(v[0] - 3.5) - (v[1] - 60)*(v[1] - 60);

//struct photo {
//	bool vertical;
//	set<int> tags;
//};
			int score = 0;

			// visited slide IDs
			uset<int> visited;
			// slide IDs
			vector<int> result;

			result.pub(0);
			int last_id = 0;
			visited.insert(0);
			while (true) {
				slide& sl = slides[last_id];
				int next_id = -1;
				int loop_score = 0;

				uset<int> candidate_slides;
				int candidate_count = 100;
				for (auto tag : sl.tags) {
					for (int i : tagToSlides[tag]) {
						// if not visited
						if (visited.find(i) == visited.end()) {
							candidate_slides.insert(i);
							candidate_count--;
						}

						if (candidate_count == 0) {
							break;
						}
					}
					if (candidate_count == 0) {
						break;
					}
				}

				for (auto slide_id : candidate_slides) {
					int temp_score = calcScore(slide_id, last_id);
					if (temp_score > loop_score) {
						loop_score = temp_score;
						next_id = slide_id;
					}
				}

				if (next_id == -1) {
					for (int i = 0; i < slides.size(); i++) {
						if (visited.find(i) == visited.end()) {
							next_id = i;
							break;
						}
					}
				}

				if (next_id == -1) {
					break;
				}

				score += loop_score;
				visited.insert(next_id);
				result.pub(next_id);

				last_id = next_id;
				if (result.size() % 1000 == 0) {
					cerr << "Result size " << result.size() << endl;
				}
			}




			// output...
			omp_set_lock(&writelock);

			cerr << "score: " << score << "       ";

			for (int i = 0; i < v.size(); i++) {
				cerr << bs[i].name << ": " << v[i] << ", ";
			}

			if (score > best_score) {
				best_params = v;
				best_score = score;

				//if (best_score > total_best_score) {
					total_best_score = best_score;

					ofstream outFile;
					string outFilename = problemName + "-" + to_string(best_score) + ".out";
					outFile.open(outFilename);
					outFile << result.size() << endl;
					for (auto& s : result) {
						string delim = "";
						for (auto& id : slides[s].ids) {
							outFile << delim << id;
							delim = " ";
						}
						outFile << endl;
					}
					outFile.close();
				//}
			}

			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			float row   = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
			cerr << " took: " << row;

			cerr << endl;
			omp_unset_lock(&writelock);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		float total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
		cerr << "Completed grid!! Time: " << total << " score: " << best_score << endl;
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

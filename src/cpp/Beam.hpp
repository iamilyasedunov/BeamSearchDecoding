#include <iostream>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iterator>
#include <functional>
#include <algorithm>
#include "MatrixCSV.hpp"

using std::map;
using std::string;
using std::set;
using std::vector;
using std::pair;

struct BeamEntry {
    float prTotal = 0;    // blank and non-blank
    float prNonBlank = 0; // non-blank
    float prBlank = 0;    // blank
    float prText = 1;     // LM score
    set<int> labeling;
};

class BeamState {
public:
    map <set<int>, BeamEntry> entries ;
    vector <set<int>> getBestLabelings(int beamWidth = 10);
    void addBeam(set<int> labeling);
    BeamState& operator=(const BeamState& right) {
    if (this == &right) {
            return *this;
        }
        entries = right.entries;
        return *this;
    }
};
// void BeamState::addBeam(set<int> labeling);
// vector <set<int>> BeamState::getBestLabelings(int beamWidth);
void displayIntSet(set<int> s);
void displayBeamEntry(struct BeamEntry be);

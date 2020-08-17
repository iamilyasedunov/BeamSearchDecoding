#include "Beam.hpp"

void BeamState::addBeam(set<int> labeling) {
    if (this->entries.find(labeling) != this->entries.end()) {
        BeamEntry be;
        this->entries[labeling] = be;
    }
}
vector <set<int>> BeamState::getBestLabelings(int beamWidth) {
    typedef std::pair<set<int>, BeamEntry> KeyValueType;
    vector <KeyValueType> entries(this->entries.begin(), this->entries.end());
    std::sort
    (
        entries.begin(),
        entries.end(),
        [](const KeyValueType& a, const KeyValueType& b) {return a.second.prTotal*a.second.prText > b.second.prTotal*b.second.prText;} 
    );

    vector <set<int>> res;
    res.reserve(beamWidth);
    for (size_t i = 0; i < entries.size() && (int)i < beamWidth; ++i) {
        res.push_back(entries[i].second.labeling);
    }
    return res;
}

void displayIntSet(set<int> s) {
    std::cout<<"{";
    for (auto it=s.begin(); it != s.end(); ++it) {
        std::cout<<' '<< *it;
    }
    std::cout<<"}";
}

void displayBeamEntry(struct BeamEntry be) {
    std::cout<<"    prTotal: "<<be.prTotal<<"\n";
    std::cout<<"    prNonBlank: "<<be.prNonBlank<<"\n";
    std::cout<<"    prBlank: "<<be.prBlank<<"\n";
    std::cout<<"    prText: "<<be.prText<<"\n";
    std::cout<<"    labeling: ";
    displayIntSet(be.labeling);
    std::cout<<"\n\n";
}
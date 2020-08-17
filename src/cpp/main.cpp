#ifdef __cplusplus
    #include <iostream>
    #include <string>
    #include <map>
    #include <set>
    #include <fstream>
    #include <iterator>
    #include <functional>
    #include <algorithm>
    #include "MatrixCSV.hpp"
    #include "Beam.hpp"
    #include "cxxopts.hpp"
#endif

#ifdef _WIN32
    #ifdef BUILD_CBMP
        #define EXPORT_SYMBOL __declspec(dllexport)
    #else
        #define EXPORT_SYMBOL __declspec(dllimport)
    #endif
#else
    #define EXPORT_SYMBOL
#endif

// Example program

using std::map;
using std::string;
using std::set;
using std::vector;
using std::pair;

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s) {
    os << "{";
    typename std::vector<T>::const_iterator it = s.begin();
    for( ; it != s.end(); ++it)
        os << *it;

    os << "}";
    return os;
}

std::string getAlphabet(const std::string& filename) {
    std::ifstream f(filename);
    std::string line;
    std::getline(f, line);
    return line;
}

void BeamSearch(const MatrixCSV& mat, const std::string& alphabet, int beamWidth = 10) {
    size_t maxT = mat.rows();
    size_t maxC = mat.cols();
    int blankIdx = alphabet.length();

    // initialise beam state
    BeamEntry be;
    BeamState last;
    set<int> labeling;
    last.entries[labeling] = be;
    last.entries[labeling].prBlank = 1;
    last.entries[labeling].prTotal = 1;

    // go over all time-steps
    for (size_t t = 0; t < maxT; t++) {
        BeamState curr;
        // get beam-labelings of best beams
        vector <set<int>> bestLabelings = last.getBestLabelings(beamWidth);
        // go over best beams
        for (set<int> labeling : bestLabelings) {
            // probability of paths ending with a non-blank
            float prNonBlank = 0.0;
            // in case of non-empty beam
            if (!labeling.empty()) {
                prNonBlank = last.entries[labeling].prNonBlank * mat.getAt(t, *labeling.rbegin());
            }
            // probability of paths ending with a blank
            float prBlank = last.entries[labeling].prTotal * mat.getAt(t, blankIdx);
            // add beam at current time-step if needed
            curr.addBeam(labeling);
            // fill in data
            curr.entries[labeling].labeling = labeling;
            curr.entries[labeling].prNonBlank += prNonBlank;
            curr.entries[labeling].prBlank += prBlank;
            curr.entries[labeling].prTotal += prBlank + prNonBlank;
            // beam-labeling not changed, therefore also LM score unchanged from
            curr.entries[labeling].prText = last.entries[labeling].prText;

            // extend current beam-labeling
            for (int c = 0; c < (int)maxC - 1; c++) {

                // add new char to current beam-labeling
                labeling.insert(c);
                set<int> newLabeling = labeling;

                // if new labeling contains duplicate char at the end, only consider paths ending with a blank
                if (!labeling.empty() && (*labeling.rbegin() == c)) {
                    prNonBlank = mat.getAt(t, c) * last.entries[labeling].prBlank;
                } else {
                    prNonBlank = mat.getAt(t, c) * last.entries[labeling].prTotal;
                }
                // add beam at current time-step if needed
                curr.addBeam(newLabeling);

                // fill in data
                curr.entries[newLabeling].labeling = newLabeling;
                curr.entries[newLabeling].prNonBlank += prNonBlank;
                curr.entries[newLabeling].prTotal += prNonBlank;
            }
        }
        // set new beam state

        last = std::move(curr);
    }
    vector <set<int>> bestLabeling = last.getBestLabelings(1);

    for(set<int> const &mySet : bestLabeling){
        displayIntSet(mySet);
        for(const int i : mySet){
            std::cout<<alphabet[i]<<" ";
        }
    }
}

int main(int argc, const char* argv[]) {
    // parse
    cxxopts::Options options("test", "A brief description");
    options.add_options()
        ("a,alphabet_path", "Path to alphabet.txt", cxxopts::value<std::string>())
        ("p,probs_path", "Path to probs.csv", cxxopts::value<std::string>())
    ;
    auto result = options.parse(argc, argv);
    
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    // define filenames
    std::string probsFileName; // = "../../data/test1/probs.csv";
    std::string alphabetFileName; // = "../../data/test1/alphabet.txt";
    if (result.count("alphabet_path")) {
        alphabetFileName = result["alphabet_path"].as<std::string>();
    } else {
        std::cout<<"\nPlease use -a option\n";
        std::cout<< options.help() << std::endl;
    }
    if (result.count("probs_path")) {
        probsFileName = result["probs_path"].as<std::string>();
    } else {
        std::cout<<"\nPlease use -p option\n";
        std::cout<< options.help() << std::endl;
    }
    // load Data
    MatrixCSV mat(probsFileName);
    std::string alphabet = getAlphabet(alphabetFileName);

    BeamSearch(mat, alphabet);
    
    return 0;
}

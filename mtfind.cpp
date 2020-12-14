#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using str = std::string;

struct Match {
    int line;
    int pos;
    str substr;
};
typedef std::vector<Match> Matches;

void find_matches(const str& line, int line_num, const str& pattern, Matches& matches) {
    int max_index = line.size() - pattern.size();
    for (int i = 0; i <= max_index; ++i) {
        int pos = 0;
        for (; pos < pattern.size(); ++pos) {
            auto c = pattern[pos];
            if (c != '?' && c != line[i + pos]) {
                break;
            }
        }
        if (pos == pattern.size()) {
            matches.push_back({ line_num, i + 1, line.substr(i, pos) });
            i += pos - 1; // matches do not overlap
        }
    }
}

int main(int argc, const char* argv[])
{
    if (argc < 3) {
        std::cerr << "Specify input file and search string" << std::endl;
        return 1;
    }

    str pattern = argv[2];
    if (pattern.size() < 1) {
        std::cerr << "Specify non-empty search string (may include '?' chars)" << std::endl;
        return 2;
    }

    std::vector<Match> matches;

    std::ifstream fs;
    fs.open(argv[1]);
    if (fs.fail()) {
        std::cerr << "Can't open " << argv[1] << " for reading" << std::endl;
        return 3;
    }

    str line;
    for (int num = 1; std::getline(fs, line); ++num) {
        find_matches(line, num, pattern, matches);
    }
 
    std::cout << matches.size() << std::endl;

    for_each(matches.begin(), matches.end(), [](const Match& m) { 
        std::cout << m.line << " " << m.pos << " " << m.substr << std::endl; 
    });
}

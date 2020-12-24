#pragma once
#include <vector>
#include <string>

using str = std::string;

// Single match
struct Match {
    int line_num;
    int pos;
    str substr;
};
typedef std::vector<Match> Matches;

// Input item
struct Input {
    str pattern;
    str line;
    int line_num;

    void Find(Matches& matches);
};

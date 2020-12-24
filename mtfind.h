#pragma once
#include <vector>
#include <string>
#include <mutex>

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
    str line;
    int line_num;

    // find matches for one line
    bool Find(const str& pattern, Matches& matches);
};

// Search results
class Results: Matches {
public:
    void Append(Matches&); // append is synchronized
    void Sort();
    void Print() const;
private:
    std::mutex m;
};

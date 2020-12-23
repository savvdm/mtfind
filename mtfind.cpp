#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>

#include "channel.h"

using str = std::string;

struct Match {
    int line_num;
    int pos;
    str substr;
};
typedef std::vector<Match> Matches;

struct Input {
    str pattern;
    str line;
    int line_num;

    void Find(Matches& matches);
};

void Input::Find(Matches& matches) {
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

    std::ifstream fs;
    fs.open(argv[1]);
    if (fs.fail()) {
        std::cerr << "Can't open " << argv[1] << " for reading" << std::endl;
        return 3;
    }

    Channel<Input> in;
    Matches out;
    std::mutex m;

    std::vector<std::thread> workers;
    auto count = std::thread::hardware_concurrency();
    for (auto i = 0; i < count; ++i) {
        workers.emplace(workers.end(), std::thread([&in, &out, &m] {
            Input input;
            while (in.Read(input)) {
                //std::cerr << "Read line: " << input.line << std::endl;
                Matches matches;
                input.Find(matches);
                std::lock_guard<std::mutex> lk(m);
                out.insert(out.end(), matches.begin(), matches.end());
            }
        }));
    }

    str line;
    for (int num = 1; std::getline(fs, line); ++num) {
        in.Write({pattern, line, num});
        //std::cerr << "Wrote line: " << line << std::endl;
    }
    in.Close();
 
    std::for_each(workers.begin(), workers.end(), [](std::thread& w){ w.join(); });

    std::stable_sort(out.begin(), out.end(),
        [](const Match& a, const Match& b){
            return a.line_num < b.line_num;
        });

    std::cout << out.size() << std::endl;

    for_each(out.begin(), out.end(), [](const Match& m) {
        std::cout << m.line_num << " " << m.pos << " " << m.substr << std::endl;
    });
}

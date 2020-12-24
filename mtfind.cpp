#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>

#include "mtfind.h"
#include "channel.h"

// Search input line for matches
void Input::Find(const str& pattern, Matches& matches) {
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

void Output::Append(Matches& matches) {
    std::lock_guard<std::mutex> lk(m);
    insert(end(), matches.begin(), matches.end());
}

void Output::Sort() {
    std::stable_sort(begin(), end(),
        [](const Match& a, const Match& b){ return a.line_num < b.line_num; }
    );
}

void Output::Print() const {
    std::cout << size() << std::endl;
    for_each(begin(), end(), [](const Match& m) {
        std::cout << m.line_num << " " << m.pos << " " << m.substr << std::endl;
    });
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
    Output out;

    // processing input lines
    std::vector<std::thread> workers;
    auto count = std::thread::hardware_concurrency();
    for (auto i = 0; i < count; ++i) {
        workers.emplace(workers.end(), std::thread([&in, &out, &pattern] {
            Input input;
            Matches matches;
            while (in.Read(input)) {
                //std::cerr << "Read line: " << input.line << std::endl;
                matches.clear();
                input.Find(pattern, matches);
                out.Append(matches);
            }
        }));
    }

    // reading input
    str line;
    for (int num = 1; std::getline(fs, line); ++num) {
        in.Write({line, num});
        //std::cerr << "Wrote line: " << line << std::endl;
    }

    // close input channel
    in.Close();
 
    // wait for workers to finish
    std::for_each(workers.begin(), workers.end(), [](std::thread& w){ w.join(); });

    // sort matches by line number
    out.Sort();

    // print results
    out.Print();
}

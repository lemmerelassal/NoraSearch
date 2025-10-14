#include <iostream>
#include <string>
#include <vector>

// -------------------------------------------------------
// solidSearch: C++ translation of the Go function
// -------------------------------------------------------
std::vector<std::vector<int>> solidSearch(const std::string& str, const std::string& substr, int minmatch) {
    // Preallocate index tables for ASCII characters (512 to be safe)
    std::vector<std::vector<int>> indexSub(512);
    std::vector<std::vector<int>> indexStr(512);

    // Fill indexSub: positions of each character in substr
    for (int idx = 0; idx < (int)substr.size(); ++idx) {
        unsigned char c = substr[idx];
        indexSub[c].push_back(idx);
    }

    // Fill indexStr: positions of each character in str
    for (int idx = 0; idx < (int)str.size(); ++idx) {
        unsigned char c = str[idx];
        indexStr[c].push_back(idx);
    }

    // Count matching offsets
    std::vector<int> distancesPositive(512, 0);

    for (int ch = 0; ch < 512; ++ch) {
        const auto& subPositions = indexSub[ch];
        const auto& strPositions = indexStr[ch];

        if (!subPositions.empty()) {
            for (int w : subPositions) {
                for (int x : strPositions) {
                    int dist = x - w;
                    if (dist >= 0 && dist < (int)distancesPositive.size()) {
                        distancesPositive[dist]++;
                    }
                }
            }
        }
    }

    // Collect results: [offset, count]
    std::vector<std::vector<int>> res;
    for (int offset = 0; offset < (int)distancesPositive.size(); ++offset) {
        int count = distancesPositive[offset];
        if (count > minmatch) {
            res.push_back({offset, count});
        }
    }

    return res;
}

// -------------------------------------------------------
// Main program for testing
// -------------------------------------------------------
int main() {
    std::string str = "abracadabra";
    std::string substr = "abra";
    int minmatch = 1;

    auto result = solidSearch(str, substr, minmatch);

    if (result.empty()) {
        std::cout << "No matches found.\n";
    } else {
        std::cout << "Matches found:\n";
        for (const auto& pair : result) {
            std::cout << "  Offset: " << pair[0] << ", Count: " << pair[1] << "\n";
        }
    }

    return 0;
}

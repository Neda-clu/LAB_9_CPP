#include <iostream>
#include <vector>
#include "group_stats.hpp"

int main() {
    int arr[] = {10, 11, 12, 13, 14, 15, 16, 17};
    std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));


    auto stats = groupStats(vec, [](int x) { return x % 2; }, [](int x) { return x; });

    for (const auto& g : stats) {
        std::cout << "Key: " << g.key
                  << ", Size: " << g.gsize
                  << ", Min: " << g.minEl
                  << ", Max: " << g.maxEl
                  << ", Sum: " << g.sum
                  << ", Avg: " << g.avg
                  << std::endl;
    }
}

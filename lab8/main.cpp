#include <iostream>
#include <vector>
#include <algorithm>

struct TSegment {
    long long L, R;
    int index;
};

std::vector<TSegment> min_segments_to_cover(std::vector<TSegment>& segments, int M) {
    std::sort(segments.begin(), segments.end(), [](const TSegment& a, const TSegment& b) {
        return a.L < b.L || (a.L == b.L && a.R > b.R);
    });

    std::vector<TSegment> selected_segments;
    int right = 0;
    int i = 0;
    int n = segments.size();

    while (right < M) {
        int farthest = right;
        int best_index = -1;

        while (i < n && segments[i].L <= right) {
            if (segments[i].R > farthest) {
                farthest = segments[i].R;
                best_index = i;
            }
            ++i;
        }

        if (best_index == -1) return {};

        selected_segments.push_back(segments[best_index]);
        right = farthest;
    }
    
    std::sort(selected_segments.begin(), selected_segments.end(), [](const TSegment& a, const TSegment& b) {
        return a.index < b.index;
    });

    return selected_segments;
}

int main() {
    int N, M;
    std::cin >> N;

    std::vector<TSegment> segments;
    for (int i = 0; i < N; ++i) {
        long long L, R;
        std::cin >> L >> R;
        segments.push_back({L, R, i + 1}); 
    }
    std::cin >> M;

    auto result = min_segments_to_cover(segments, M);

    if (result.empty()) {
        std::cout << "0\n";
    } else {
        std::cout << result.size() << "\n";
        for (const auto& seg : result) {
            std::cout << seg.L << " " << seg.R << "\n";
        }
    }

    return 0;
}

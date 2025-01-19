#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const long long INF = numeric_limits<long long>::max();

struct Edge {
    int u, v;
    long long weight;
};

int main() {
    int n, m, start, finish;
    cin >> n >> m >> start >> finish;
    
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;
    }

    vector<long long> distance(n + 1, INF);
    distance[start] = 0;

    // Bellman-Ford Algorithm
    for (int i = 1; i <= n - 1; i++) {
        bool updated = false;
        for (const Edge& edge : edges) {
            if (distance[edge.u] != INF && distance[edge.u] + edge.weight < distance[edge.v]) {
                distance[edge.v] = distance[edge.u] + edge.weight;
                updated = true;
            }
        }
        if (!updated) break; // Early stopping if no updates
    }

    // Check for negative-weight cycles
    for (const Edge& edge : edges) {
        if (distance[edge.u] != INF && distance[edge.u] + edge.weight < distance[edge.v]) {
            cout << "No solution" << endl;
            return 0;
        }
    }

    // Output the result
    if (distance[finish] == INF) {
        cout << "No solution" << endl;
    } else {
        cout << distance[finish] << endl;
    }

    return 0;
}
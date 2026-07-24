#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>

using namespace std;

struct Edge {
    int toNode;
    int travelTimeMs;
};

class RoutingEngine {
private:
    unordered_map<int, vector<Edge>> graph;

public:
    void addRoadSegment(int u, int v, int travelTime) {
        graph[u].push_back({ v, travelTime });
    }

    // Dijkstra's algorithm for lowest latency route
    int findShortestPath(int startNode, int targetNode) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        unordered_map<int, int> distances;

        for (const auto& pair : graph) {
            distances[pair.first] = INT_MAX;
        }

        distances[startNode] = 0;
        pq.push({ 0, startNode });

        while (!pq.empty()) {
            int currentDist = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (u == targetNode) return currentDist;

            if (currentDist > distances[u]) continue;

            for (const auto& edge : graph[u]) {
                if (distances[u] + edge.travelTimeMs < distances[edge.toNode]) {
                    distances[edge.toNode] = distances[u] + edge.travelTimeMs;
                    pq.push({ distances[edge.toNode], edge.toNode });
                }
            }
        }
        return -1; // Path not found
    }
};

int main() {
    RoutingEngine engine;

    engine.addRoadSegment(1, 2, 120);
    engine.addRoadSegment(2, 3, 85);
    engine.addRoadSegment(1, 3, 250);
    engine.addRoadSegment(3, 4, 45);

    int start = 1, destination = 4;
    int shortestTime = engine.findShortestPath(start, destination);

    cout << "Calculated Optimal Path Duration from Node " << start
        << " to Node " << destination << ": " << shortestTime << " ms" << endl;

    return 0;
}
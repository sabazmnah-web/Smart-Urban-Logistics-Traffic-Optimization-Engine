#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <optional>

struct Coordinate {
    double x;
    double y;
};

struct Edge {
    int toNode;
    double travelTimeMs;
};

struct RouteResult {
    double totalDurationMs;
    std::vector<int> path;
    bool success;
};

class RoutingEngine {
private:
    std::unordered_map<int, Coordinate> nodeCoordinates;
    std::unordered_map<int, std::vector<Edge>> graph;

    double calculateHeuristic(int nodeA, int nodeB) {
        if (nodeCoordinates.find(nodeA) == nodeCoordinates.end() || 
            nodeCoordinates.find(nodeB) == nodeCoordinates.end()) {
            return 0.0;
        }
        const auto& c1 = nodeCoordinates[nodeA];
        const auto& c2 = nodeCoordinates[nodeB];
        return std::hypot(c1.x - c2.x, c1.y - c2.y) * 1.0; 
    }

public:
    void registerNode(int nodeId, double x, double y) {
        nodeCoordinates[nodeId] = {x, y};
    }

    void addRoadSegment(int u, int v, double travelTime) {
        graph[u].push_back({v, travelTime});
    }

    RouteResult findOptimalPath(int startNode, int targetNode) {
        std::priority_queue<std::pair<double, int>, 
                            std::vector<std::pair<double, int>>, 
                            std::greater<std::pair<double, int>>> pq;

        std::unordered_map<int, double> gScores;
        std::unordered_map<int, int> previousNodes;

        for (const auto& pair : graph) {
            gScores[pair.first] = __DBL_MAX__;
        }
        gScores[startNode] = 0.0;

        double initialF = calculateHeuristic(startNode, targetNode);
        pq.push({initialF, startNode});

        while (!pq.empty()) {
            auto [currentF, u] = pq.top();
            pq.pop();

            if (u == targetNode) break;

            for (const auto& edge : graph[u]) {
                double tentativeG = gScores[u] + edge.travelTimeMs;
                if (tentativeG < gScores[edge.toNode]) {
                    gScores[edge.toNode] = tentativeG;
                    previousNodes[edge.toNode] = u;
                    double fScore = tentativeG + calculateHeuristic(edge.toNode, targetNode);
                    pq.push({fScore, edge.toNode});
                }
            }
        }

        if (gScores.find(targetNode) == gScores.end() || gScores[targetNode] == __DBL_MAX__) {
            return {0.0, {}, false};
        }

        std::vector<int> path;
        int curr = targetNode;
        while (curr != startNode) {
            path.push_back(curr);
            curr = previousNodes[curr];
        }
        path.push_back(startNode);
        std::reverse(path.begin(), path.end());

        return {gScores[targetNode], path, true};
    }
};

int main() {
    RoutingEngine engine;

    engine.registerNode(1, 0.0, 0.0);
    engine.registerNode(2, 5.0, 2.0);
    engine.registerNode(3, 3.0, 8.0);
    engine.registerNode(4, 10.0, 10.0);

    engine.addRoadSegment(1, 2, 120.0);
    engine.addRoadSegment(2, 3, 85.0);
    engine.addRoadSegment(1, 3, 250.0);
    engine.addRoadSegment(3, 4, 45.0);
    engine.addRoadSegment(2, 4, 200.0);

    int start = 1, destination = 4;
    RouteResult result = engine.findOptimalPath(start, destination);

    std::cout << "--- C++ A* Spatial Routing Engine ---" << std::endl;
    if (result.success) {
        std::cout << "Optimal Path Cost: " << result.totalDurationMs << " ms" << std::endl;
        std::cout << "Traversal Sequence: ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            std::cout << result.path[i] << (i + 1 < result.path.size() ? " -> " : "");
        }
        std::cout << std::endl;
    } else {
        std::cout << "Route resolution failed." << std::endl;
    }

    return 0;
}

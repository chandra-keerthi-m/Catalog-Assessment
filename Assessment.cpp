#include <json/json.h>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream file("input.json");
    Json::Value root;
    file >> root;

    // Read n and k
    int n = root["keys"]["n"].asInt();
    int k = root["keys"]["k"].asInt();

    std::cout << "n: " << n << ", k: " << k << std::endl;
    return 0;
}
long long decodeBase(const std::string& value, int base) {
    long long result = 0;
    for (char c : value) {
        result = result * base + (isdigit(c) ? c - '0' : c - 'A' + 10);
    }
    return result;
}
#include <vector>
#include <utility>  // For std::pair

double lagrangeInterpolation(const std::vector<std::pair<int, long long>>& points) {
    double secret = 0.0;
    int k = points.size();
    
    for (int i = 0; i < k; ++i) {
        double term = points[i].second;  // y_i
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= double(0 - points[j].first) / double(points[i].first - points[j].first);
            }
        }
        secret += term;
    }
    return secret;
}
bool checkPoint(const std::vector<std::pair<int, long long>>& points, int x, long long y) {
    double result = 0.0;
    int k = points.size();
    
    for (int i = 0; i < k; ++i) {
        double term = points[i].second;  // y_i
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= double(x - points[j].first) / double(points[i].first - points[j].first);
            }
        }
        result += term;
    }
    return std::abs(result - y) < 1e-9;  // Allowing for floating-point precision error
}
int main() {
    // Read the JSON input file
    std::ifstream file("input.json");
    Json::Value root;
    file >> root;

    // Extract n and k
    int n = root["keys"]["n"].asInt();
    int k = root["keys"]["k"].asInt();

    // Store the points
    std::vector<std::pair<int, long long>> points;
    for (const auto& key : root.getMemberNames()) {
        if (key == "keys") continue;  // Skip the "keys" part of the JSON
        
        int x = std::stoi(key);  // x is the key of the object
        std::string base_str = root[key]["base"].asString();
        std::string value_str = root[key]["value"].asString();

        int base = std::stoi(base_str);  // Decode the base
        long long y = decodeBase(value_str, base);  // Decode the y value

        points.push_back({x, y});
    }

    // Select the first k points for interpolation
    std::vector<std::pair<int, long long>> selected_points(points.begin(), points.begin() + k);

    // Calculate the constant term using Lagrange interpolation
    double secret = lagrangeInterpolation(selected_points);
    std::cout << "Secret (constant term): " << secret << std::endl;

    // Check for wrong points
    std::vector<int> wrong_points;
    for (int i = k; i < n; ++i) {
        if (!checkPoint(selected_points, points[i].first, points[i].second)) {
            wrong_points.push_back(points[i].first);
        }
    }

    // Output the wrong points
    if (!wrong_points.empty()) {
        std::cout << "Wrong points: ";
        for (int wp : wrong_points) {
            std::cout << wp << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No wrong points found." << std::endl;
    }

    return 0;
}
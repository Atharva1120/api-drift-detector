#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

// Function to read lines from a file into a vector of strings
std::vector<std::string> loadRoutes(const std::string& filename) {
    std::vector<std::string> routes;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return routes;
    }
    
    while (std::getline(file, line)) {
        // Ignore empty lines
        if (!line.empty()) {
            routes.push_back(line);
        }
    }
    file.close();
    return routes;
}

int main() {
    std::cout << "--- Running API Contract Drift Detector ---" << std::endl;

    // Load backend and frontend route lists
    std::vector<std::string> backendRoutes = loadRoutes("backend_routes.txt");
    std::vector<std::string> frontendCalls = loadRoutes("frontend_calls.txt");

    if (backendRoutes.empty() || frontendCalls.empty()) {
        std::cerr << "Initialization failed. Check your text files." << std::endl;
        return 1;
    }

    int driftCount = 0;

    // Check every frontend call against the backend routes
    for (const auto& call : frontendCalls) {
        auto it = std::find(backendRoutes.begin(), backendRoutes.end(), call);
        if (it == backendRoutes.end()) {
            std::cout << "[DRIFT DETECTED] Frontend is calling '" << call 
                      << "', but this route DOES NOT exist on the backend!" << std::endl;
            driftCount++;
        } else {
            std::cout << "[OK] Route matches: " << call << std::endl;
        }
    }

    std::cout << "------------------------------------------" << std::endl;
    if (driftCount > 0) {
        std::cout << "Result: Found " << driftCount << " contract drift error(s)!" << std::endl;
        return 1; // Exit with error code (useful for CI/CD pipelines later)
    } else {
        std::cout << "Result: All API contracts match perfectly!" << std::endl;
        return 0;
    }
}
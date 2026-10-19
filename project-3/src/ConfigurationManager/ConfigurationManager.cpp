#include "ConfigurationManager.h"
#include <fstream>
#include <iostream>

ConfigurationManager& ConfigurationManager::getInstance() {
    static ConfigurationManager instance;
    return instance;
}

ConfigurationManager::ConfigurationManager() {}

bool ConfigurationManager::loadConfiguration(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ifstream file(filename);
    std::cerr << filename;
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file: " << filename << std::endl;
        return false;
    }
    try {
        file >> config;
    } catch (json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool ConfigurationManager::saveConfiguration(const std::string& filename) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open configuration file for saving: " << filename << std::endl;
        return false;
    }
    try {
        file << config.dump(4);
    } catch (json::exception& e) {
        std::cerr << "Failed to write configuration: " << e.what() << std::endl;
        return false;
    }
    return true;
}


bool ConfigurationManager::hasKey(const std::string& key) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return config.contains(key);
}

bool ConfigurationManager::validateConfiguration() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!config["pareto_shape"].is_number()) {
        std::cerr << "'pareto_shape' must be a number." << std::endl;
        return false;
    }
    return true;
}




#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <./../json.hpp>
#include <string>
#include <mutex>
#include <iostream>

using json = nlohmann::json;

class ConfigurationManager {
public:
    static ConfigurationManager& getInstance();

    bool loadConfiguration(const std::string& filename);
    bool saveConfiguration(const std::string& filename) const;


    template<typename T>
    T get(const std::string& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!config.contains(key)) {
            throw std::runtime_error("Key '" + key + "' not found in configuration.");
        }
        return config.at(key).get<T>();
    }


    template<typename T>
    T get(const std::string& key, const T& defaultValue) {
        std::lock_guard<std::mutex> lock(m_mutex);
        try {
            if (config.at(key).is_number()) {
                return config.at(key).get<T>();
            } else {
                std::cerr << "Expected a number but found a different type for key: " << key << std::endl;
                return defaultValue;
            }
        } catch (const json::exception& e) {
            std::cerr << "Error retrieving configuration for key " << key << ": " << e.what() << std::endl;
            return defaultValue;
        }
    }


    bool hasKey(const std::string& key) const;
    bool validateConfiguration() const;


private:
    ConfigurationManager();
    json config;
    mutable std::mutex m_mutex;
};


#endif // CONFIGURATIONMANAGER_H

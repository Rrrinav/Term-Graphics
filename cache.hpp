// WE WILL IMPLEMENT CACHE SYSTEM HERE TO CACHE MEMORY TO DECREASE PERFORMANCE OVERHEAD
#include <string>
#include <unordered_map>
#include <vector>
#define L_GEBRA_IMPLEMENTATION
#include "l_gebra.hpp"
#include "shapes.hpp"

class Cache
{
    std::unordered_map<std::string, std::vector<utl::Vec<int, 2>>> _cache_buffer;

public:
    Cache() : _cache_buffer(std::unordered_map<std::string, std::vector<utl::Vec<int, 2>>>()) {}
    Cache(const std::unordered_map<std::string, std::vector<utl::Vec<int, 2>>> &cache_buffer) : _cache_buffer(cache_buffer) {}
    Cache(const Cache &other) : _cache_buffer(other._cache_buffer) {}

    void cache_data(const std::string &key, const std::vector<utl::Vec<int, 2>> &value) { _cache_buffer[key] = value; }

    void cache_point(const std::string &key, const utl::Vec<int, 2> &value)
    {
        // If key doesn't exist, create a new key
        if (_cache_buffer.find(key) == _cache_buffer.end())
        {
            _cache_buffer[key] = {value};
        }
        else
        {
            _cache_buffer[key].push_back(value);
        }
    }
};

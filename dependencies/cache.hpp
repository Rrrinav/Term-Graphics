#include <cstddef>
#include <unordered_map>
#include <vector>

#include "shapes.hpp"

// This cache class is designed to used and printed automatically
// User just has to use the flag 'cache' in the function call of draw methods
class Automatic_cache
{
    std::unordered_map<size_t, std::vector<Point>> _cache_map;

public:
    Automatic_cache() = default;
    std::vector<Point> create_cache(size_t id, std::vector<Point> points)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
        {
            _cache_map[id] = points;
            return _cache_map[id];
        }
        else
        {
            for (auto &p : points) _cache_map[id].push_back(p);
        }
        return _cache_map[id];
    }

    std::vector<Point> get_cache(size_t id)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
            return std::vector<Point>();
        return _cache_map[id];
    }

    std::vector<Point> update_cache(size_t id, std::vector<Point> points)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
            return std::vector<Point>();
        _cache_map[id] = points;
        return _cache_map[id];
    }

    std::vector<Point> remove_cache(size_t id)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
            return std::vector<Point>();
        std::vector<Point> points = _cache_map[id];
        _cache_map.erase(id);
        return points;
    }

    std::vector<Point> clear_cache()
    {
        std::vector<Point> points;
        for (auto &it : _cache_map)
            for (auto &p : it.second) points.push_back(p);
        _cache_map.clear();
        return points;
    }

    std::vector<Point> *get_cache_pointer(size_t id)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
            return nullptr;
        return &_cache_map[id];
    }

    std::unordered_map<size_t, std::vector<Point>> get_cache_map() { return _cache_map; }
};

// This cache class is designed to used and printed manually by the user
// Interface will still remain renderer itself
class External_cache
{
    std::unordered_map<std::string, std::vector<Point>> _cache_map;

public:
    External_cache() = default;
    
    std::unordered_map<std::string, std::vector<Point>> get_cache_map() { return _cache_map; }

    std::vector<Point> create_cache(std::string id, std::vector<Point> points)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
        {
            _cache_map[id] = points;
            return _cache_map[id];
        }
        else
        {
            for (auto &p : points) _cache_map[id].push_back(p);
        }
        return _cache_map[id];
    }

    std::vector<Point> get_cache(std::string id)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
            return std::vector<Point>();
        return _cache_map[id];
    }

    std::vector<Point> insert_points(std::string id, std::vector<Point> points)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
            return std::vector<Point>();
        for (auto &p : points) _cache_map[id].push_back(p);
        return _cache_map[id];
    }
};

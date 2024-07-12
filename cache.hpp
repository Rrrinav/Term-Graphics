#include <cstddef>
#include <unordered_map>
#include <vector>

#include "shapes.hpp"

class Cache
{
    std::unordered_map<size_t, std::vector<Point>> _cache_map;

public:
    Cache() = default;
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
            for (auto &p : points)
            {
                _cache_map[id].push_back(p);
            }
        }
        return _cache_map[id];
    }

    std::vector<Point> get_cache(size_t id)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
        {
            return std::vector<Point>();
        }
        return _cache_map[id];
    }

    std::vector<Point> remove_cache(size_t id)
    {
        auto it = _cache_map.find(id);
        if (it == _cache_map.end())
        {
            return std::vector<Point>();
        }
        std::vector<Point> points = _cache_map[id];
        _cache_map.erase(id);
        return points;
    }

    std::unordered_map<size_t, std::vector<Point>> get_cache_map() { return _cache_map; }
};

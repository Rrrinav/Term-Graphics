#include <cstddef>
#include <unordered_map>
#include <vector>

#include "shapes.hpp"

class Cache
{
    std::unordered_map<size_t, std::vector<Point>> _text_cache;

public:
    Cache() = default;
    std::vector<Point> cache_text(size_t id, std::vector<Point> points)
    {
        auto it = _text_cache.find(id);
        if (it == _text_cache.end())
        {
            _text_cache[id] = points;
            return _text_cache[id];
        }
        else
        {
            for (auto &p : points)
            {
                _text_cache[id].push_back(p);
            }
        }
        return _text_cache[id];
    }

    std::vector<Point> get_text(size_t id)
    {
        auto it = _text_cache.find(id);
        if (it == _text_cache.end())
        {
            return std::vector<Point>();
        }
        return _text_cache[id];
    }

    std::vector<Point> remove_text(size_t id)
    {
        auto it = _text_cache.find(id);
        if (it == _text_cache.end())
        {
            return std::vector<Point>();
        }
        std::vector<Point> points = _text_cache[id];
        _text_cache.erase(id);
        return points;
    }

    std::unordered_map<size_t, std::vector<Point>> get_text_cache()
    {
      return _text_cache;
    }
};

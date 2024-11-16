﻿#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H
#include <SDL_render.h>
#include <vector>

#include "math/vec2.h"

static constexpr size_t kCircleVertexCount = 20;

class GraphicsManager
{
private:
  std::vector<SDL_Vertex> vertices_;
  std::vector<int> indices_;

public:
  GraphicsManager() = default;
  ~GraphicsManager() = default;

  std::vector<SDL_Vertex>& vertices() { return vertices_; }
  std::vector<int>& indices() { return indices_; }

  void AddVertex(math::Vec2f position, SDL_Color color);
  void Clear();
  void CreateCircle(math::Vec2f centre, float radius, SDL_Color color, bool rotation);
  void CreateAABB(math::Vec2f min, math::Vec2f max, SDL_Color color, bool fill_status);
  void CreateAABB(math::Vec2f centre, float half_size, SDL_Color color, bool fill_status);
  void CreatePolygon(const std::vector<math::Vec2f>& points, math::Vec2f center, SDL_Color color, bool fill_status);
};






#endif //SHAPE_MANAGER_H
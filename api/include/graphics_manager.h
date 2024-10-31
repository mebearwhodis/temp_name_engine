#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H
#include <SDL_render.h>
#include <vector>

#include "math/vec2.h"

static constexpr size_t kCircleVertexCount = 30;

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
  void CreateCircle(math::Vec2f center, float radius, SDL_Color color);
  void CreateAABB(math::Vec2f min, math::Vec2f max, SDL_Color color, bool fill_status);
  void CreatePolygon(std::vector<math::Vec2f> points, SDL_Color color, bool fill_status);
};






#endif //SHAPE_MANAGER_H

#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H
#include <SDL_render.h>
#include <vector>

#include "math/vec2.h"

static constexpr size_t kCircleVertexCount = 30;

class GraphicalShape
{
private:
  std::vector<SDL_Vertex> vertices_;
  std::vector<int> indices_;

public:
  GraphicalShape() = default;
  ~GraphicalShape() = default;

  std::vector<SDL_Vertex>& vertices() { return vertices_; }
  std::vector<int>& indices() { return indices_; }

  void AddVertex(math::Vec2f position, SDL_Color color);
  void Clear();
  void CreateCircle(math::Vec2f center, float radius, SDL_Color color);
};






#endif //SHAPE_MANAGER_H

#include "shape_manager.h"

#include "common.h"

void ShapeManager::AddVertex(const math::Vec2f position, const SDL_Color color)
{
    //const SDL_Vertex vertex = {position.x, position.y, color.r, color.g, color.b};
    SDL_Vertex vertex;
    vertex.position.x = position.x;
    vertex.position.y = position.y;
    vertex.color.r = color.r;
    vertex.color.g = color.g;
    vertex.color.b = color.b;
    vertex.color.a = color.a;
    vertices_.push_back(vertex);
}

void ShapeManager::Clear()
{
    vertices_.clear();
    indices_.clear();
}

void ShapeManager::CreateCircle(math::Vec2f center, float radius, SDL_Color color)
{
    //Track where the new circle's vertices start
    const size_t starting_index = vertices_.size();

    constexpr float angle_step = (2 * math::Pi) / kCircleVertexCount;

    //Add the center of the circle
    AddVertex(center, color);

    //Generate vertices
    for (size_t i = 0; i < kCircleVertexCount; i++)
    {
        const float angle = i * angle_step;
        const float x = center.x + radius * std::cos(angle);
        const float y = center.y + radius * std::sin(angle);

        AddVertex(math::Vec2f{x, y}, color);
    }

    //Generate indices
    for (size_t i = 0; i < kCircleVertexCount - 1; i++)
    {
        indices_.push_back(starting_index);          // Center vertex
        indices_.push_back(starting_index + i + 1);  // Current outer vertex
        indices_.push_back(starting_index + i + 2);  // Next outer vertex
    }


    //Last triangle, closing the circle
    indices_.push_back(starting_index);
    indices_.push_back(starting_index + kCircleVertexCount);
    indices_.push_back(starting_index + 1);

}

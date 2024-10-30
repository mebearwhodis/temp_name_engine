#include "graphics_manager.h"
#include "common.h"

void GraphicsManager::AddVertex(const math::Vec2f position, const SDL_Color color)
{
    SDL_Vertex vertex;
    vertex.position.x = position.x;
    vertex.position.y = position.y;
    vertex.color.r = color.r;
    vertex.color.g = color.g;
    vertex.color.b = color.b;
    vertex.color.a = color.a;
    vertices_.push_back(vertex);
}

void GraphicsManager::Clear()
{
    vertices_.clear();
    indices_.clear();
}

void GraphicsManager::CreateCircle(const math::Vec2f center, const float radius, const SDL_Color color)
{
    //Track where the new circle's vertices start
    const size_t starting_index = vertices_.size();

    constexpr float angle_step = (2 * math::Pi) / kCircleVertexCount;

    //Add the center of the circle
    AddVertex(center, color);

    //Generate vertices
    for (size_t i = 0; i < kCircleVertexCount; i++)
    {
        const float angle = static_cast<float>(i) * angle_step;
        const float x = center.x + radius * std::cos(angle);
        const float y = center.y + radius * std::sin(angle);

        AddVertex(math::Vec2f{x, y}, color);
    }

    //Generate indices
    for (size_t i = 0; i < kCircleVertexCount - 1; i++)
    {
        indices_.push_back(static_cast<int>(starting_index));          //Center vertex
        indices_.push_back(static_cast<int>(starting_index) + i + 1);  //Current outer vertex
        indices_.push_back(static_cast<int>(starting_index) + i + 2);  //Next outer vertex
    }


    //Last triangle, closing the circle
    indices_.push_back(static_cast<int>(starting_index));
    indices_.push_back(static_cast<int>(starting_index) + kCircleVertexCount);
    indices_.push_back(static_cast<int>(starting_index) + 1);

}

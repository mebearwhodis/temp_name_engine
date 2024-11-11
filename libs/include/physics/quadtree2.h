// #ifndef QUADTREE2_H
// #define QUADTREE2_H
//
// #include <array>
// #include <vector>
//
// #include "collider.h"
// #include "math/shape.h"
//
// #include "../../../cmake-build-relwithdebinfo/vcpkg_installed/x64-windows/include/SDL2/SDL_render.h"
//
// namespace physics
// {
//     struct QuadTreeNode2
//     {
//         math::AABB bounding_box_{};
//         std::array<QuadTreeNode2*, 4> children_{};
//         std::vector<Collider*> colliders_;
//     };
//
//     class QuadTree2
//     {
//     public:
//         explicit QuadTree2(const math::AABB& boundary);
//         ~QuadTree2();
//
//         void Insert(Collider* collider);
//         [[nodiscard]] std::vector<Collider*> Query(const math::AABB& range) const;
//         void Clear();
//         void Draw(SDL_Renderer* renderer) const;
//
//     private:
//         void Insert(Collider* collider, QuadTreeNode2* node, int depth);
//         void Query(const QuadTreeNode2* node, const math::AABB& range, std::vector<Collider*>& foundColliders) const;
//         void Subdivide(QuadTreeNode2* node);
//         static void DrawNode(SDL_Renderer* renderer, const QuadTreeNode2* node);
//
//         QuadTreeNode2* root_;
//         static constexpr std::size_t kMaxDepth = 4;
//         static constexpr std::size_t kMaxColliders = 8;
//     };
// }
//
// #endif // QUADTREE2_H

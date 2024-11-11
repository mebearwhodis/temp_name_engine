// #include <iostream>
// #include <physics/quadtree.h>
// namespace physics
// {
//     Quadtree::Quadtree(const math::AABB& boundary)
//     {
//         root_ = new QuadTreeNode2;
//         root_->bounding_box_ = boundary;
//     }
//
//     Quadtree::~Quadtree()
//     {
//         Clear();
//         delete root_;
//     }
//
//     void Quadtree::Insert(Collider* collider)
//     {
//         if (collider)
//         {
//             Insert(collider, root_, 0);
//         }
//     }
//
//     void Quadtree::Insert(Collider* collider, QuadTreeNode2* node, int depth)
//     {
//         if (node->colliders_.size() < kMaxColliders || depth == kMaxDepth)
//         {
//             node->colliders_.push_back(collider);
//             return;
//         }
//
//         // Perform subdivision if needed
//         if (node->children_[0] == nullptr)
//         {
//             Subdivide(node);
//
//             // Move existing colliders to child nodes if they fit
//             auto it = node->colliders_.begin();
//             while (it != node->colliders_.end())
//             {
//                 bool moved = false;
//                 for (auto* child : node->children_)
//                 {
//                     if (child && child->bounding_box_.Contains((*it)->GetBoundingBox().min_bound()) &&
//                         child->bounding_box_.Contains((*it)->GetBoundingBox().max_bound()))
//                     {
//                         child->colliders_.push_back(*it);
//                         it = node->colliders_.erase(it);
//                         moved = true;
//                         break;
//                     }
//                 }
//                 if (!moved)
//                 {
//                     ++it; // Move to the next collider if it wasn't transferred
//                 }
//             }
//         }
//
//         // Try to insert the new collider into one of the children nodes
//         for (auto* child : node->children_)
//         {
//             if (child && child->bounding_box_.Contains(collider->GetBoundingBox().min_bound()) &&
//                 child->bounding_box_.Contains(collider->GetBoundingBox().max_bound()))
//             {
//                 Insert(collider, child, depth + 1);
//                 return;
//             }
//         }
//
//         // If collider doesn't fully fit into any child, keep it in the current node
//         node->colliders_.push_back(collider);
//     }
//
//
//     void Quadtree::Subdivide(QuadTreeNode2* node)
//     {
//         math::Vec2f halfSize = (node->bounding_box_.max_bound() - node->bounding_box_.min_bound()) * 0.5f;
//         math::Vec2f center = node->bounding_box_.min_bound() + halfSize;
//
//         node->children_[0] = new QuadTreeNode2{math::AABB(node->bounding_box_.min_bound(), center)};
//         node->children_[1] = new QuadTreeNode2{math::AABB(math::Vec2f(center.x, node->bounding_box_.min_bound().y),
//                                                          math::Vec2f(node->bounding_box_.max_bound().x, center.y))};
//         node->children_[2] = new QuadTreeNode2{math::AABB(math::Vec2f(node->bounding_box_.min_bound().x, center.y),
//                                                          math::Vec2f(center.x, node->bounding_box_.max_bound().y))};
//         node->children_[3] = new QuadTreeNode2{math::AABB(center, node->bounding_box_.max_bound())};
//     }
//
//
//     std::vector<Collider*> Quadtree::Query(const math::AABB& range) const
//     {
//         std::vector<Collider*> foundColliders;
//         Query(root_, range, foundColliders);
//         return foundColliders;
//     }
//
//     void Quadtree::Query(const QuadTreeNode2* node, const math::AABB& range, std::vector<Collider*>& foundColliders) const
//     {
//         if (!node || !math::Intersect(node->bounding_box_, range))
//         {
//             return;
//         }
//
//         for (auto* collider : node->colliders_)
//         {
//             if (math::Intersect(collider->GetBoundingBox(), range))
//             {
//                 foundColliders.push_back(collider);
//             }
//         }
//
//         for (const auto* child : node->children_)
//         {
//             if (child) // Ensure child is not nullptr
//             {
//                 Query(child, range, foundColliders);
//             }
//         }
//     }
//
//
//     void Quadtree::Clear()
//     {
//         std::function<void(QuadTreeNode2*)> deleteNodes = [&](QuadTreeNode2* node)
//         {
//             for (auto* child : node->children_)
//             {
//                 if (child)
//                 {
//                     deleteNodes(child);
//                     delete child;
//                     child = nullptr;
//                 }
//             }
//             node->colliders_.clear();
//         };
//     }
//
//     void Quadtree::Draw(SDL_Renderer* renderer) const
//     {
//         DrawNode(renderer, root_);
//     }
//
//     void Quadtree::DrawNode(SDL_Renderer* renderer, const QuadTreeNode2* node) const
//     {
//         SDL_Rect rect;
//         rect.x = static_cast<int>(node->bounding_box_.min_bound().x);
//         rect.y = static_cast<int>(node->bounding_box_.min_bound().y);
//         rect.w = static_cast<int>(node->bounding_box_.max_bound().x - node->bounding_box_.min_bound().x);
//         rect.h = static_cast<int>(node->bounding_box_.max_bound().y - node->bounding_box_.min_bound().y);
//
//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         SDL_RenderDrawRect(renderer, &rect);
//
//         for (const auto* child : node->children_)
//         {
//             if (child)
//             {
//                 DrawNode(renderer, child);
//             }
//         }
//     }
// }

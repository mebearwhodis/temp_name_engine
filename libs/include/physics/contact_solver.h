#ifndef CONTACT_SOLVER_H
#define CONTACT_SOLVER_H
#include <array>
#include <utility>

#include "game_object.h"

namespace physics
{
    struct ContactSolver
    {
        std::array<GameObject*, 2> objects_;

        math::Vec2f contact_point_;
        math::Vec2f contact_normal_;

        float penetration_;

        void SetContactObjects(const GameObjectPair& pair)
        {
            objects_[0] = pair.gameObjectA_;
            objects_[1] = pair.gameObjectB_;
        }

        void CalculateProperties()
        {
            const auto collider_a = objects_[0]->collider();
            const auto collider_b = objects_[1]->collider();

            switch(collider_a.GetShapeType())
            {
                case math::ShapeType::kCircle:
                    switch(collider_b.GetShapeType())
                    {
                        //case Circle AABB
                        case math::ShapeType::kAABB:
                            break;
                        //case Circle Circle
                        case math::ShapeType::kCircle:
                            break;
                        //case Circle Polygon??
                        case math::ShapeType::kPolygon:
                            break;
                        case math::ShapeType::kNone:
                        case default:
                            break;
                    }
                case math::ShapeType::kAABB:
                    switch(collider_b.GetShapeType())
                    {
                        //case AABB AABB
                        case math::ShapeType::kAABB:
                            break;
                        //case AABB Circle: SWAP
                        case math::ShapeType::kCircle:
                            std::swap(objects_[0], objects_[1]);
                            CalculateProperties();
                            break;
                        //case AABB Polygon??
                        case math::ShapeType::kPolygon:
                            break;
                        case math::ShapeType::kNone:
                        case default:
                            break;
                    }
                    break;
            case math::ShapeType::kPolygon:
                switch(collider_b.GetShapeType())
                {
                    //case Polygon AABB: SWAP
                    case math::ShapeType::kAABB:
                        std::swap(objects_[0], objects_[1]);
                        CalculateProperties();
                        break;
                    //case Polygon Circle: SWAP
                    case math::ShapeType::kCircle:
                        std::swap(objects_[0], objects_[1]);
                        CalculateProperties();
                        break;
                    //case Polygon Polygon??
                    case math::ShapeType::kPolygon:
                        break;
                    case math::ShapeType::kNone:
                    case default:
                        break;
            }
                    break;
                case math::ShapeType::kNone:
                case default:
                    break;
            }
        }

        void ResolveVelocities()
        {

        }

        void ResolvePositions()
        {

        }

        void ResolveContact()
        {
            CalculateProperties();
            ResolveVelocities();
            ResolvePositions();
            //Calculate the relative velocity between the bodies
           //math::Vec2f relative_velocity = pair.gameObjectB_->body().velocity() - pair.gameObjectA_->body().velocity();

            //
        }
    };
}
#endif //CONTACT_SOLVER_H

#ifndef SHAPE_H
#define SHAPE_H
#include <algorithm>
#include <vector>
#include "vec2.h"

namespace math
{
    enum class ShapeType
    {
        kAABB,
        kCircle,
        kPolygon,
        kNone
    };

    class AABB
    {
    private:
        Vec2f min_bound_ = Vec2f::Zero();
        Vec2f max_bound_ = Vec2f::Zero();

    public:
        constexpr AABB(const Vec2f min_bound, const Vec2f max_bound) : min_bound_(min_bound), max_bound_(max_bound){}
        AABB() = default;

        [[nodiscard]] constexpr Vec2f min_bound() const { return min_bound_; }
        [[nodiscard]] constexpr Vec2f max_bound() const { return max_bound_; }

        void set_min_bound(const Vec2f bound) { min_bound_ = bound; }
        void set_max_bound(const Vec2f bound) { max_bound_ = bound; }

        [[nodiscard]] constexpr bool Contains(const Vec2f point) const
        {
            if (point.x < min_bound_.x) return false;
            if (point.x > max_bound_.x) return false;
            if (point.y < min_bound_.y) return false;
            if (point.y > max_bound_.y) return false;
            return true;
        }
        [[nodiscard]] AABB GetBoundingBox() const {
            return *this;
        }
        // [[nodiscard]] constexpr Vec2f GetHalfSize() const { return (max_bound_ - min_bound_) / 2; }
        [[nodiscard]] static constexpr ShapeType GetShapeType() { return ShapeType::kAABB; }

        bool operator==(const AABB& other) const {
            return min_bound_ == other.min_bound_ && max_bound_ == other.max_bound_;
        }
    };

    class Circle
    {
    private:
        Vec2f centre_ = Vec2f::Zero();
        float radius_ = 0.0f;

    public:
        constexpr Circle(const Vec2f center, const float radius) : centre_(center), radius_(radius){}

        explicit constexpr Circle(const float radius) : centre_(Vec2f::Zero()), radius_(radius){}

        [[nodiscard]] constexpr Vec2f centre() const { return centre_; }
        [[nodiscard]] constexpr float radius() const { return radius_; }

        void set_centre(const Vec2f center) { centre_ = center; }
        void set_radius(const float radius) { radius_ = radius; }

        [[nodiscard]] bool Contains(const Vec2f point) const
        {
            return (point - centre_).SquareMagnitude() <= radius_ * radius_;
        }

        [[nodiscard]] AABB GetBoundingBox() const
        {
            const Vec2f min = centre_ - Vec2f(radius_, radius_);
            const Vec2f max = centre_ + Vec2f(radius_, radius_);
            const AABB box(min, max);
            return box;
        }
        [[nodiscard]] static constexpr ShapeType GetShapeType() { return ShapeType::kCircle; }

        bool operator==(const Circle& other) const {
            return centre_ == other.centre_ && radius_ == other.radius_;
        }
    };

    class Polygon
    {
        private:
        std::vector<Vec2f> vertices_;

        public:
        explicit constexpr Polygon(const std::vector<Vec2f>& vertices) : vertices_(vertices){}

        [[nodiscard]] constexpr std::vector<Vec2f> vertices() const { return vertices_; }

        void set_vertices(const std::vector<Vec2f>& vertices) { vertices_ = vertices; }

        [[nodiscard]] constexpr int VertexCount() const { return vertices_.size(); }

        [[nodiscard]] AABB GetBoundingBox() const {
            Vec2f min = vertices_[0];
            Vec2f max = vertices_[0];

            for (const auto& vertex : vertices_) {
                min.x = std::min(min.x, vertex.x);
                min.y = std::min(min.y, vertex.y);
                max.x = std::max(max.x, vertex.x);
                max.y = std::max(max.y, vertex.y);
            }

            return AABB(min, max);
        }
        [[nodiscard]] static constexpr ShapeType GetShapeType() { return ShapeType::kPolygon; }

        bool operator==(const Polygon& other) const {
            return vertices_ == other.vertices_;
        }
    };

    //Find the point of a segment that is closest to a specified point
    constexpr Vec2f ClosestPointOnSegment(const Vec2f segment_start, const Vec2f segment_end, const Vec2f compare_point)
    {
        const Vec2f segment = segment_end - segment_start;
        const Vec2f point_to_start = compare_point - segment_start;
        const float t = std::clamp(point_to_start.Dot(segment) / segment.SquareMagnitude(), 0.0f, 1.0f);
        const Vec2f closest_point = segment_start + segment * t;

        return closest_point;
    }

    //Intersections between shapes
    [[nodiscard]] constexpr bool Intersect(const AABB& aabb_a, const AABB& aabb_b)
    {
        if(aabb_a.max_bound().x < aabb_b.min_bound().x || aabb_a.min_bound().x > aabb_b.max_bound().x) return false;
        if(aabb_a.max_bound().y < aabb_b.min_bound().y || aabb_a.min_bound().y > aabb_b.max_bound().y) return false;
        return true;
    }

    [[nodiscard]] constexpr bool Intersect(const Circle& circle_a, const Circle& circle_b)
    {
        const Vec2f delta = circle_a.centre() - circle_b.centre();
        const float distanceSquared = delta.SquareMagnitude();
        const float radiusSum = circle_a.radius() + circle_b.radius();
        return distanceSquared < radiusSum * radiusSum;
        //return (circle_b.centre() - circle_a.centre()).SquareMagnitude() <= (circle_b.radius() + circle_a.radius()) * (circle_b.radius() + circle_a.radius());
    }

    [[nodiscard]] constexpr bool Intersect(const Polygon& polygon_a, const Polygon& polygon_b)
    {
        for (const auto polygons = {polygon_a, polygon_b}; const auto& polygon : polygons)
        {
            const auto& vertices = polygon.vertices();
            const size_t vertex_count = polygon.VertexCount();

            for (size_t i = 0; i < vertex_count; ++i)
            {
                //Compute the edge and perpendicular axis
                Vec2f edge = vertices[(i + 1) % vertex_count] - vertices[i];
                const Vec2f axis = edge.Perpendicular();

                //Project all vertices of polygon_a onto the axis
                float min_a = vertices[0].Dot(axis);
                float max_a = min_a;
                for (const auto& vertex : polygon_a.vertices())
                {
                    float projection = vertex.Dot(axis);
                    min_a = std::min(min_a, projection);
                    max_a = std::max(max_a, projection);
                }

                //Project all vertices of polygon_b onto the axis
                float min_b = polygon_b.vertices()[0].Dot(axis);
                float max_b = min_b;
                for (const auto& vertex : polygon_b.vertices())
                {
                    float projection = vertex.Dot(axis);
                    min_b = std::min(min_b, projection);
                    max_b = std::max(max_b, projection);
                }

                //Check for an overlap on this axis
                if (max_a < min_b || max_b < min_a)
                {
                    //No overlap on this axis means no intersection
                    return false;
                }
            }
        }

        //If no separating axis was found, the polygons intersect
        return true;
    }

    [[nodiscard]] constexpr bool Intersect(const AABB& aabb, const Circle& circle)
    {
        const auto center = circle.centre();
        //Check if the AABB contains the circle's centre
        if(aabb.Contains(center)) return true;

        //If not, expand the AABB bounds by the radius and check if those contain the centre
        const auto radius = circle.radius();
        const auto min_bound = aabb.min_bound();
        const auto max_bound = aabb.max_bound();

        const auto min_bound_x = min_bound - Vec2f(radius, 0);
        const auto max_bound_x = max_bound + Vec2f(radius, 0);

        const auto min_bound_y = min_bound - Vec2f(0, radius);
        const auto max_bound_y = max_bound + Vec2f(0, radius);

        const AABB extended_x(min_bound_x, max_bound_x);
        const AABB extended_y(min_bound_y, max_bound_y);

        if(extended_x.Contains(center)) return true;
        if(extended_y.Contains(center)) return true;

        //If not, check if the circle contains one of the corners of the AABB
        if(circle.Contains(min_bound)) return true;
        if(circle.Contains(Vec2f(min_bound.x, max_bound.y))) return true;
        if(circle.Contains(max_bound)) return true;
        if(circle.Contains(Vec2f(max_bound.x, min_bound.y))) return true;

        //If not, they don't intersect
        return false;
    }

    [[nodiscard]] constexpr bool Intersect(const AABB& aabb, const Polygon& polygon)
    {
        const std::vector<Vec2f> aabb_vertices = {
            aabb.min_bound(),
            Vec2f(aabb.min_bound().x, aabb.max_bound().y),
            aabb.max_bound(),
            Vec2f(aabb.max_bound().x, aabb.min_bound().y)
        };
        const Polygon aabb_polygon(aabb_vertices);
        return Intersect(polygon, aabb_polygon);
    }

    [[nodiscard]] constexpr bool Intersect(const Circle& circle, const Polygon& polygon)
    {
        const auto vertices = polygon.vertices();
        const auto center = circle.centre();
        auto radius = circle.radius();

        //Check if any vertex of the polygon is inside the circle
        for(const auto& vertex : vertices)
        {
            if(circle.Contains(vertex)) return true;
        }

        //Check if the circle intersects with any of the polygon's edges
        for(size_t i = 0, j = polygon.VertexCount() - 1; i < polygon.VertexCount(); j = i++)
        {
            const Vec2f start = vertices[i];
            const Vec2f end = vertices[j];

            if(const Vec2f closest_point = ClosestPointOnSegment(start, end, center); circle.Contains(closest_point)) return true;
        }
        //If no intersection is found, return false
        return false;
    }

    [[nodiscard]] constexpr bool Intersect(const Circle& circle, const AABB& aabb){ return Intersect(aabb, circle); }

    [[nodiscard]] constexpr bool Intersect(const Polygon& polygon, const Circle& circle) { return Intersect(circle, polygon); }

    [[nodiscard]] constexpr bool Intersect(const Polygon& polygon, const AABB& aabb) { return Intersect(aabb, polygon); }
}

#endif //SHAPE_H

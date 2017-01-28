/**
 * Coordinates (respectively): +x, -x, +y, -y => right, left, up, down
 */


#ifndef QUADTREE_H
#define QUADTREE_H

#include <memory>
#include <limits>
#include <vector>
#include "units_coords.hpp"
#include "node.hpp"

namespace stransit {

    constexpr double dinfinity = std::numeric_limits<double>::infinity();

    struct quadtree_bounds {
        double minX, minY, maxX, maxY;
        bool intersects(const quadtree_bounds& other) {
            return minX <= other.maxX && maxX >= other.minX &&
                minY <= other.maxY && maxY >= other.minY;
        }

        bool contains(const vec2& v) {
            return v.x >= minX && v.x < maxX &&
                                        v.y >= minY && v.y < maxY;
        }

        static quadtree_bounds from_location_radius(const vec2& location, miles radius) {
            degrees delta_angle = delta_geo_angle(radius);
            return {location.x - delta_angle, location.y - delta_angle,
                    location.x + delta_angle, location.y + delta_angle};
        }
    };

    constexpr quadtree_bounds infinite_quadtree_bounds = {-dinfinity, -dinfinity, dinfinity, dinfinity};

    template <typename Element, typename Coords>
    struct quadtree_node {
        quadtree_node(Element element) : elem(element), bounds(infinite_quadtree_bounds) {}
        quadtree_node(Element element, quadtree_bounds bounds) : elem(element),
                                                                 bounds(bounds) {}
        quadtree_node(Element element,
                      std::unique_ptr<quadtree_node> top_left,
                      std::unique_ptr<quadtree_node> top_right,
                      std::unique_ptr<quadtree_node> bottom_left,
                      std::unique_ptr<quadtree_node> bottom_right) :
            top_left(top_left), top_right(top_right), bottom_left(bottom_left),
            bottom_right(bottom_right), elem(element), bounds(infinite_quadtree_bounds) {}

        std::unique_ptr<quadtree_node> top_left, top_right, bottom_left, bottom_right;
        Element elem;
        quadtree_bounds bounds;
    };

    /**
     * quadtree implementation that only allows insertion and range searching
     * Coords(Element e) -> vec2
     */
    template <typename Element, typename Coords>
    class quadtree {
        Coords m_get_coords;
        std::unique_ptr<quadtree_node<Element, Coords>> m_root;

        enum search_section {
            section_top_left, section_top_right, section_bottom_left, section_bottom_right
        };
        static search_section get_search_section(vec2 from, vec2 to);
    public:
        quadtree();
        void insert(Element);
        std::vector<Element> search_range(quadtree_bounds search_bounds);
    };

}

using namespace stransit;

template <typename Element, typename Coords>
typename quadtree<Element, Coords>::search_section
quadtree<Element, Coords>::get_search_section(vec2 from, vec2 to) {
    if (to.x < from.x) {
        if (to.y < from.y) {
            return section_bottom_left;
        } else {
            return section_top_left;
        }
    } else {
        if (to.y < from.y) {
            return section_bottom_right;
        } else {
            return section_top_right;
        }
    }
}

template <typename Element, typename Coords>
stransit::quadtree<Element, Coords>::quadtree() :m_get_coords(), m_root(nullptr) {}

template <typename Element, typename Coords>
void stransit::quadtree<Element, Coords>::insert(Element elem) {
    if (m_root == nullptr) {
        m_root = std::make_unique<quadtree_node<Element, Coords>>(elem);
    } else {
        std::unique_ptr<quadtree_node<Element, Coords>>* ptr_to_update = nullptr;
        quadtree_node<Element, Coords>* current_node = m_root.get();
        quadtree_bounds current_bounds = current_node->bounds;
        while (current_node != nullptr) {
            vec2 current_coords = m_get_coords(current_node->elem);
            switch (get_search_section(current_coords,
                                       m_get_coords(elem))) {
            case section_top_left:
                current_bounds.maxX = current_coords.x;
                current_bounds.minY = current_coords.y;
                ptr_to_update = &current_node->top_left;
                break;
            case section_top_right:
                current_bounds.minX = current_coords.x;
                current_bounds.minY = current_coords.y;
                ptr_to_update = &current_node->top_right;
                break;
            case section_bottom_left:
                current_bounds.maxX = current_coords.x;
                current_bounds.maxY = current_coords.y;
                ptr_to_update = &current_node->bottom_left;
                break;
            case section_bottom_right:
                current_bounds.minX = current_coords.x;
                current_bounds.maxY = current_coords.y;
                ptr_to_update = &current_node->bottom_right;
                break;
            }
            current_node = ptr_to_update->get();
        }
        (*ptr_to_update) = std::make_unique<quadtree_node<Element, Coords>>(elem, current_bounds);
    }
}


template <typename Element, typename Coords>
std::vector<Element>
stransit::quadtree<Element, Coords>::search_range(quadtree_bounds search_bounds) {
    std::vector<Element> result;
    if (m_root == nullptr) {
        return result;
    }

    std::vector<quadtree_node<Element, Coords>*> to_search;
    to_search.push_back(m_root.get());

    while(!to_search.empty()) {
        quadtree_node<Element, Coords>* qnode = to_search.back();
        to_search.pop_back();
        if (qnode == nullptr) {
            continue;
        }

        if (search_bounds.intersects(qnode->bounds)) {
            to_search.push_back(qnode->top_left.get());
            to_search.push_back(qnode->top_right.get());
            to_search.push_back(qnode->bottom_left.get());
            to_search.push_back(qnode->bottom_right.get());

            if (search_bounds.contains(m_get_coords(qnode->elem))) {
                result.push_back(qnode->elem);
            }
        }

    }

    return result;
}


#endif /* QUADTREE_H */

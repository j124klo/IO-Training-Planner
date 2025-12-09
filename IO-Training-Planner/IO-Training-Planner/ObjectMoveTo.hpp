#pragma once

#include <concepts>
#include <cmath>

#include <SFML/Graphics.hpp>

using namespace std;

template <typename T>
concept MovableSFMLObject = requires(T obj, sf::Vector2f v) {
    { obj.getPosition() } -> convertible_to<sf::Vector2f>;
    { obj.setPosition(v) } -> same_as<void>;
    { obj.move(v) } -> same_as<void>;
};

template <MovableSFMLObject T>
void ObjMoveTo(T& obj, const sf::Vector2f& target_location, float speed, float dt) {
    sf::Vector2f current = obj.getPosition();
    sf::Vector2f direction = target_location - current;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance <= speed * dt) {
        obj.setPosition(target_location);
    }
    else {
        sf::Vector2f normDir = direction / distance;
        obj.move(normDir * speed * dt);
    }
}

//  processor registers level error witout this, just what type of bs is this?
//  fr tho, this is for checking if final location is reached
inline sf::Vector2f floorV2f(sf::Vector2f vector)
{
    return sf::Vector2f(floor(vector.x), floor(vector.y));
}
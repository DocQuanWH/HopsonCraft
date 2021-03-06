#include "Player.h"

#include "../Input/Key_Binds.h"
#include "../Input/Function_Toggle_Key.h"
#include "../Display.h"

#include <iostream>

void Player::input()
{
    keyBoardInput();
    mouseInput();
}


void Player::keyBoardInput ()
{
    Vector3 change;
    float speed = 0.35;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        speed *= 10;
    }

    if(m_isInLiquid) speed /= 1.5;

    float yaw   = glm::radians(rotation.y);
    float yaw90 = glm::radians(rotation.y + 90);

    if (sf::Keyboard::isKeyPressed(Key_Binds::getKey(Key_Binds::Control::Player_Forwards)))
    {
        change.x -= glm::cos(yaw90) * speed;
        change.z -= glm::sin(yaw90) * speed;
    }

    if (sf::Keyboard::isKeyPressed(Key_Binds::getKey(Key_Binds::Control::Player_Back)))
    {
        change.x += glm::cos(yaw90) * speed;
        change.z += glm::sin(yaw90) * speed;
    }

    if (sf::Keyboard::isKeyPressed(Key_Binds::getKey(Key_Binds::Control::Player_Left)))
    {
        change.x -= glm::cos(yaw) * speed;
        change.z -= glm::sin(yaw) * speed;
    }

    if (sf::Keyboard::isKeyPressed(Key_Binds::getKey(Key_Binds::Control::Player_Right)))
    {
        change.x += glm::cos(yaw) * speed;
        change.z += glm::sin(yaw) * speed;
    }

    if (sf::Keyboard::isKeyPressed(Key_Binds::getKey(Key_Binds::Control::Player_Up)))
    {
        change.y += speed;
    }

    if (sf::Keyboard::isKeyPressed(Key_Binds::getKey(Key_Binds::Control::Player_Down)))
    {
        change.y -= speed;
    }

    static Function_Toggle_Key key([&]()
    {
        m_isFlying = !m_isFlying;
        m_isOnGround = false;
    },
    sf::Keyboard::F, sf::seconds(1.0));
    key.checkInput();

    addForce(change);
}


void Player::mouseInput ()
{

    static bool lock = false;
    static Function_Toggle_Key key([&](){ lock = !lock; }, sf::Keyboard::L, sf::seconds(1.0));
    key.checkInput();
    if(lock) return;


    static sf::Vector2i lastMousePosition = sf::Mouse::getPosition();

    auto mouseChange = sf::Mouse::getPosition() - lastMousePosition;

    rotation.y += mouseChange.x * 0.1;
    rotation.x += mouseChange.y * 0.1;

    constexpr int8_t BOUND = 82;

    if (rotation.x > BOUND)
    {
        rotation.x = BOUND;
    }
    else if (rotation.x < -BOUND)
    {
        rotation.x = -BOUND;
    }
    if (rotation.y < 0)
    {
        rotation.y = 360;
    }
    else if (rotation.y > 360)
    {
        rotation.y = 0;
    }

    auto centerX = Display::get().getSize().x / 4;
    auto centerY = Display::get().getSize().y / 4;

    sf::Mouse::setPosition({static_cast<int>(centerX),
                            static_cast<int>(centerY)}, Display::get());

    lastMousePosition = sf::Mouse::getPosition();
}

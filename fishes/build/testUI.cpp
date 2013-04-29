/*
 * testSfml2.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: agustin
 */

#include <vector>
#include <iostream>
#include <cmath>
#include <atomic>
#include <memory>

#include <boost/filesystem.hpp>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <common/debug/DebugUtil.h>

#include <core/utils/AnimatedSprite.h>
#include <core/utils/TextureAtlas.h>
#include <math/Matrix4.h>
#include <math/AABB.h>
#include <core/resource_manager/ResourceManager.h>
#include <common/Forwards.h>
#include <core/space_partition/SpacePartition.h>
#include <core/utils/MouseCursor.h>
#include <core/system/GlobalConfig.h>
#include <core/ui/UIManager.h>
#include <core/ui/Element.h>



class TestElement : public ui::Element {
public:
    TestElement(ui::UIManager *m) : Element(m) {};

    virtual void newEvent(const ui::EventInfo& event)
    {
        std::cout << "NewEvent: ";
        switch (event.event) {
        case ui::EventInfo::Event::None:
            std::cout << "ui::EventInfo::Event::None";
            break;
        case ui::EventInfo::Event::MouseButton:
            std::cout << "ui::EventInfo::Event::MouseButton";
            break;
        case ui::EventInfo::Event::MouseInside:
            std::cout << "ui::EventInfo::Event::MouseInside";
            break;
        case ui::EventInfo::Event::MouseMoving:
            std::cout << "ui::EventInfo::Event::MouseMoving";
            break;
        case ui::EventInfo::Event::MouseOutside:
            std::cout << "ui::EventInfo::Event::MouseOutside";
            break;
        default:
            ASSERT(false);
        }
        std::cout << std::endl;
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::Clock clock;

    // configure the sys::GlobalConfig
    sys::GlobalConfig gc;
    gc.setRenderWindow(window);

    utils::MouseCursor mouse(gc);
    gc.setMouseCursor(mouse);

    // create the ui system and some elements
    ui::UIManager uiManager(gc);
    TestElement elem1(&uiManager), elem2(&uiManager);
    math::AABBui aabb(0,0,100,100);
    elem1.setAABB(aabb);
    aabb.translate(math::Vector2ui(200,200));
    elem2.setAABB(aabb);
    uiManager.addMenu(&elem1);
    uiManager.addMenu(&elem2);

    resources::ResourceManager rm;
    std::cout << "Reading resources: " << rm.readResourcesFromFolder("./mediaTest")
      << std::endl;

    TexturePtr tex;
    ASSERT(rm.getResource("6x3.png", tex));
    ASSERT(tex.get());
    utils::TextureAtlas texAtlas;
    texAtlas.configure(tex, 6, 3);

    // configure the mouse
    mouse.setTexture(tex);
    mouse.setTextureAtlas(texAtlas);
    mouse.setMouseCursor(0);

    window.setMouseCursorVisible(false);


    float lastTime = 0.f;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // get the time frame
        const float nowTime = clock.getElapsedTime().asSeconds();
        const float timeFrame = nowTime - lastTime;
        lastTime = nowTime;

        const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        mouse.setMousePos(mousePos);
        window.draw(mouse);
        uiManager.update();

        // window display all
        window.display();

    }

    return 0;
}

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
#include <core/ui/Button.h>


static ui::Button *
createButton(const std::string& texName,
             const math::AABBui& aabb,
             ui::UIManager *manager,
             resources::ResourceManager& rm)
{
    TexturePtr texture;
    rm.getResource(texName, texture);
    if (texture.get() == 0) {
        return 0;
    }
    ui::Button* button = new ui::Button(manager);

    button->setAABB(aabb);
    button->configureButton(texture);


    return button;
}


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

    resources::ResourceManager rm;
    std::cout << "Reading resources: " << rm.readResourcesFromFolder("./mediaTest")
      << std::endl;

    // create the button
    math::AABBui aabb(10,10,300,200);
    ui::Button *button = createButton("button.png", aabb, &uiManager, rm);
    if (button) {
        uiManager.addMenu(button);
    }


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

    sf::Sprite removeMe;
    TexturePtr tex2;
    ASSERT(rm.getResource("button.png", tex2));
    ASSERT(tex2.get());
    removeMe.setTexture(*tex2.get());
    removeMe.setPosition(300, 300);
    const sf::Vector2u tex2Size = tex2->getSize();
    sf::IntRect rectRemoveMe;
    rectRemoveMe.width = tex2Size.x / 3;
    rectRemoveMe.height = tex2Size.y;
    rectRemoveMe.top = rectRemoveMe.left = 0;
    removeMe.setTextureRect(rectRemoveMe);
    removeMe.setScale(4,4);

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

        window.draw(removeMe);

        // window display all
        window.display();

    }

    return 0;
}

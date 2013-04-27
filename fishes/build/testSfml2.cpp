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

#include <core/utils/AnimatedSprite.h>
#include <core/utils/TextureAtlas.h>
#include <math/Matrix4.h>
#include <math/AABB.h>
#include <core/resource_manager/ResourceManager.h>
#include <common/Forwards.h>
#include <core/space_partition/SpacePartition.h>


int main()
{
    s_p::SpacePartition sp;
    sp.build(math::AABB<unsigned int>(0,0, 100,100), 4, 4);


    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::Clock clock;

    utils::AnimatedSprite sprite;
    resources::ResourceManager rm;
    std::cout << "Reading resources: " << rm.readResourcesFromFolder("./mediaTest")
      << std::endl;

    TexturePtr tex;
    ASSERT(rm.getResource("6x3.png", tex));
    ASSERT(tex.get());
    sprite.setTexture(tex);
    utils::TextureAtlas texAtlas;
    texAtlas.configure(tex, 6, 3);
    sprite.setTextureAtlas(texAtlas);



    // configure the animations
    utils::AnimatedSprite::AnimIndices anim;
    std::vector<utils::AnimatedSprite::AnimIndices> animVec;
    anim.begin = 0;
    anim.end = 5;
    anim.animTime = 4;
    animVec.push_back(anim);
    anim.begin = 6;
    anim.end = 11;
    anim.animTime = 0.5f;
    animVec.push_back(anim);
    anim.begin = 12;
    anim.end = 13;
    anim.animTime = 1.f;
    animVec.push_back(anim);
    sprite.createAnimTable(animVec);
    sprite.setAnim(0);
    sprite.setLoop(true);
    sprite.sprite().setPosition(20, 20);


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

            // check for input
            if (event.type == sf::Event::KeyPressed){
                switch(event.key.code){
                case sf::Keyboard::Num1:
                    sprite.setAnim(0);
                    sprite.setLoop(true);
                    break;
                case sf::Keyboard::Num2:
                    sprite.setAnim(1);
                    sprite.setLoop(true);
                    break;
                case sf::Keyboard::Num3:
                    sprite.setAnim(2);
                    sprite.setLoop(false);
                    break;
                default:
                    break;
                }
            }
        }

        window.clear();

        // get the time frame
        const float nowTime = clock.getElapsedTime().asSeconds();
        const float timeFrame = nowTime - lastTime;
        lastTime = nowTime;

        sprite.update(timeFrame);

        window.draw(sprite.sprite());

        // window display all
        window.display();

    }

    return 0;
}

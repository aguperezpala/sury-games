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

#include <ui/AnimatedSprite.h>
#include <math/Matrix4.h>
#include <math/AABB.h>
#include <core/resource_manager/ResourceManager.h>
#include <common/Forwards.h>
#include <core/space_partition/SpacePartition.h>


static std::atomic<unsigned int> mCounter;

static void
matrixTest(void)
{
    mCounter++;

    math::Matrix4 m = math::Matrix4::IDENTITY;
    math::AABBf aabb;
    aabb.tl.x = 0;
    aabb.tl.y = 10;
    aabb.br.x = 10;
    aabb.br.y = 0;

    std::cout << m << std::endl;
    std::cout << aabb << std::endl;

    float angle  = -45 * 3.141592654f / 180.f;
    float cosine = static_cast<float>(std::cos(angle));
    float sine   = static_cast<float>(std::sin(angle));
    float sxc    = 1 * cosine;
    float syc    = 1 * cosine;
    float sxs    = 1 * sine;
    float sys    = 1 * sine;
    float tx     = -5 * sxc - 5 * sys + 0;
    float ty     =  5 * sxs - 5 * syc + 0;

    math::Matrix4 m2( sxc, sys, tx,
                            -sxs, syc, ty,
                             0.f, 0.f, 1.f);
    std::cout << "m2:\n" << m2 << std::endl;
    math::AABBf aabb2(aabb);
    m2.transformAABB(aabb2);
    std::cout << "aabb2 transformed by m2\n" << aabb2 << std::endl;
    m.transformAABB(aabb);
    std::cout << "aabb transformed by m\n" << aabb << std::endl;

    m2 *= m;
    std::cout << "m2 after mult with m\n" << m2 << std::endl;

}



int main()
{
    s_p::SpacePartition sp;
    sp.build(math::AABB<unsigned int>(0,0, 100,100), 4, 4);


    matrixTest();
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::Clock clock;

    ui::AnimatedSprite sprite;

    if (!sprite.build("./mediaTest/6x3.png", 6, 3)){
        std::cout << "Error building the sprite" << std::endl;
        return -1;
    }

    // configure the animations
    ui::AnimatedSprite::AnimIndices anim;
    std::vector<ui::AnimatedSprite::AnimIndices> animVec;
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
    sprite.setPosition(0,0);

    sprite.setPosition(100, 100);

    resources::ResourceManager rm;
    std::cout << "Reading resources: " << rm.readResourcesFromFolder("./mediaTest")
        << std::endl;
    sf::Sprite normalSprite;
    TexturePtr tex;
    ASSERT(rm.getResource("6x3.png", tex));
    ASSERT(tex.get());
    normalSprite.setTexture(*tex.get());



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

        window.draw(sprite);
        window.draw(normalSprite);

        // window display all
        window.display();

    }

    return 0;
}

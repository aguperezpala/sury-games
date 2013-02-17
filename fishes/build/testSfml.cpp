#include <vector>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ui/AnimatedSprite.h>



int main()
{
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

        // window display all
        window.display();

    }

    return 0;
}

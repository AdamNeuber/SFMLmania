#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
using std::cout, std::endl, std::cin, std::vector;

std::default_random_engine generator;
std::uniform_int_distribution<int> chance(0, 1000);
std::uniform_int_distribution<int> row(0, 3);

bool chekCollisonWithTarget(vector<sf::CircleShape> &arr, sf::Shape &button)
{
    for (int t = 0; t < arr.size(); t++)
    {
        if (arr[t].getGlobalBounds().findIntersection(button.getGlobalBounds()))
        {
            arr.erase(arr.begin() + t);
            return true;
        }
    }
    return false;
}

void gen(vector<sf::CircleShape> &vec)
{
    if (chance(generator) == 0)
    {
        sf::CircleShape target(80.f);
        target.setPosition({row(generator) * 213.f, -100.f});
        vec.push_back(target);
    }
}

int clamp(int max, int min, int value)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

int countHeld(vector<sf::Keyboard::Key> vec)
{
    int res = 0;
    for (auto key : vec)
    {
        if (sf::Keyboard::isKeyPressed(key))
            res++;
    }
    return res;
}

int main()
{
    int HEALTH;
    cout << "Enter your desired max HP" << endl;
    cin >> HEALTH;

    int damage;
    cout << "Enter your desired damage" << endl;
    cin >> damage;

    int speed;
    cout << "Enter your desired speed (about 1000 is recomended)(<=0 is gonna be boring)" << endl;
    cin >> speed;
    int hp = HEALTH;

    sf::RectangleShape hpBar({HEALTH, 30.f});
    hpBar.setFillColor(sf::Color::Red);

    sf::RectangleShape killZone({800.f, 1.f});
    killZone.setPosition({0, 900.f});

    sf::Clock time;

    sf::RenderWindow window(sf::VideoMode({800, 800}), "OSUmania but shit");

    vector<sf::CircleShape> targets;

    vector<sf::CircleShape> askl;
    for (size_t i = 0; i < 4; i++)
    {
        sf::CircleShape button(80.f);
        button.setPosition({i * 213.f, 640.f});
        askl.push_back(button);
    }
    vector<sf::Keyboard::Key> keys = {sf::Keyboard::Key::A, sf::Keyboard::Key::S, sf::Keyboard::Key::K, sf::Keyboard::Key::L};
    while (window.isOpen())
    {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (auto key = event->getIf<sf::Event::KeyPressed>())
            {
                for (int i = 0; i < keys.size(); i++)
                {
                    if (key->code == keys[i])
                    {
                        askl[i].setFillColor(sf::Color::Red);

                        if (chekCollisonWithTarget(targets, askl[i]))
                        {
                            hp = clamp(HEALTH, 0, hp + damage);
                        }
                        else
                        {
                            hp -= damage;
                        }
                    }
                }
            }
            if (auto key = event->getIf<sf::Event::KeyReleased>())
            {
                for (int i = 0; i < keys.size(); i++)
                {
                    askl[i].setFillColor(sf::Color::White);
                }
            }
        }

        float dt = time.restart().asSeconds();

        if (hp < 1)
            window.close();

        gen(targets);
        hpBar.setSize({hp * 1.f, 30.f});
        for (auto &target : targets)
        {
            target.move(sf::Vector2f(0.f, speed) * dt);
        }
        if (chekCollisonWithTarget(targets, killZone))
            hp -= damage;

        window.clear();
        for (auto i : targets)
        {
            window.draw(i);
        }
        for (auto i : askl)
        {
            window.draw(i);
        }
        window.draw(hpBar);
        window.display();
    }
}
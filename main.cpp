#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

static int windowW = 600;
static int windowH = 400;

class Particle
{
public:
    bool highlight;

    Particle(int x, int y)
    {
        circle.setRadius(4);
        circle.setOrigin(4, 4);
        circle.setPosition(x, y);
        circle.setFillColor(sf::Color::Black);
        highlight = false;
    }
    float getX() { return circle.getPosition().x; }
    float getY() { return circle.getPosition().x; }

    void move()
    {
        circle.move(std::uniform_int_distribution<>(-1, 1)(eng), std::uniform_int_distribution<>(-1, 1)(eng));
    }
    void render(sf::RenderWindow& window)
    {
        if (highlight)
        {
            circle.setFillColor(sf::Color::Red);
        }
        else
        {
            circle.setFillColor(sf::Color::Black);
        }
        window.draw(circle);
    }

    sf::FloatRect getBounds() { return circle.getGlobalBounds(); }

    bool intersects(Particle* p)
    {
        return (this->circle.getGlobalBounds().intersects(p->getBounds()));
    }

private:
    sf::CircleShape circle;
    int             velX, velY;
    std::mt19937    eng {std::chrono::high_resolution_clock::now().time_since_epoch().count()};
};

class Point
{
public:
    Point(float x, float y, Particle* Data)
    {
        this->x    = x;
        this->y    = y;
        this->Data = Data;
    }

    Particle* getData() { return Data; }

    ~Point() { this->Data = NULL; }

    float getX()
    {
        return x;
    }
    float getY()
    {
        return y;
    }

private:
    float     x, y;
    Particle* Data;
};

class Rectangle
{
public:
    Rectangle() {}
    Rectangle(int x, int y, int w, int h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    void setPosition(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void printCoordinates()
    {
        std::cout << "X: " << x << "\n"
                  << "Y: " << y << "\n"
                  << "W: " << w << "\n"
                  << "H: " << h << std::endl;
    }

    bool contains(Point* p)
    {
        return ((p->getX() >= this->x - this->w) && (p->getX() <= this->x + this->w) && (p->getY() >= this->y - this->h) && (p->getY() <= this->y + this->h));
    }

    bool intersects(Rectangle* range)
    {
        return (!(range->getX() - range->getW() > this->x + this->w || range->getX() + range->getW() < this->x - this->w || range->getY() - range->getH() > this->y + this->h || range->getY() + range->getH() < this->y - this->h));
    }

    float getX()
    {
        return x;
    }
    float getY()
    {
        return y;
    }
    float getW()
    {
        return w;
    }
    float getH()
    {
        return h;
    }

private:
    float x, y, w, h;
};

class QuadTree
{
public:
    sf::RectangleShape rect;
    size_t             cont;
    float              foo;

private:
    Rectangle*          boundary;
    int                 capacity;
    std::vector<Point*> points;
    QuadTree*           northwest;
    QuadTree*           northeast;
    QuadTree*           southeast;
    QuadTree*           southwest;
    bool                divided;

public:
    QuadTree()
    {
        boundary  = NULL;
        northwest = NULL;
        northeast = NULL;
        southeast = NULL;
        southwest = NULL;
        divided   = false;
    }
    QuadTree(Rectangle* boundary, int capacity)
    {
        this->boundary = boundary;
        this->capacity = capacity;
        divided        = false;

        rect.setSize(sf::Vector2f(boundary->getW() * 2, boundary->getH() * 2));
        rect.setOrigin((boundary->getW() * 2) / 2, (boundary->getH() * 2) / 2);
        rect.setPosition(boundary->getX(), boundary->getY());
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(1);
        rect.setOutlineColor(sf::Color::Black);

        boundary  = NULL;
        northwest = NULL;
        northeast = NULL;
        southeast = NULL;
        southwest = NULL;
    }

    ~QuadTree()
    {
        for (int i = 0; i < (int)points.size(); i++)
        {
            delete points[i];
        }
        points.clear();

        if (northwest != NULL)
            delete northwest;

        if (northeast != NULL)
            delete northeast;

        if (southeast != NULL)
            delete southeast;

        if (southwest != NULL)
            delete southwest;
    }

    void Query(Rectangle* range, std::vector<Point*>& found)
    {

        if (!this->boundary->intersects(range))
        {
            return;
        }
        else
        {
            for (int i = 0; i < (int)points.size(); i++)
            {
                if (range->contains(points[i]))
                {
                    found.push_back(points[i]);
                }
            }

            if (this->divided)
            {
                this->northwest->Query(range, found);

                this->northeast->Query(range, found);

                this->southwest->Query(range, found);

                this->southeast->Query(range, found);
            }
        }
    }

    void show1(sf::RenderWindow& window)
    {
        window.draw(rect);
        if (divided)
        {
            northeast->show1(window);
            northwest->show1(window);
            southeast->show1(window);
            southwest->show1(window);
        }
    }

    void subdivide()
    {
        Rectangle* ne   = new Rectangle(boundary->getX() + boundary->getW() / 2,
            boundary->getY() - boundary->getH() / 2,
            boundary->getW() / 2, boundary->getH() / 2);
        this->northeast = new QuadTree(ne, capacity);

        Rectangle* nw   = new Rectangle(boundary->getX() - boundary->getW() / 2,
            boundary->getY() - boundary->getH() / 2,
            boundary->getW() / 2, boundary->getH() / 2);
        this->northwest = new QuadTree(nw, capacity);

        Rectangle* se   = new Rectangle(boundary->getX() + boundary->getW() / 2,
            boundary->getY() + boundary->getH() / 2,
            boundary->getW() / 2, boundary->getH() / 2);
        this->southeast = new QuadTree(se, capacity);

        Rectangle* sw   = new Rectangle(boundary->getX() - boundary->getW() / 2,
            boundary->getY() + boundary->getH() / 2,
            boundary->getW() / 2, boundary->getH() / 2);
        this->southwest = new QuadTree(sw, capacity);

        this->divided = true;
    }

    bool insertion(Point* p)
    {
        //boundary->printCoordinates();
        if (!this->boundary->contains(p))
        {
            return false;
        }
        if ((int)this->points.size() < capacity)
        {
            this->points.push_back(p);
            return true;
        }
        if (!this->divided)
        {
            this->subdivide();
        }


        if (this->northeast->insertion(p))
            return true;
        else if (this->northwest->insertion(p))
            return true;
        else if (this->southeast->insertion(p))
            return true;
        else if (this->southwest->insertion(p))
            return true;

        return false;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Quadtree collision");
    // window.setFramerateLimit(60);
    std::mt19937 eng {std::chrono::high_resolution_clock::now().time_since_epoch().count()};

    std::vector<Particle*> p;

    Rectangle* boundary = new Rectangle(300, 200, 600, 400);

    for (int i = 0; i < 1000; i++)
    {
        p.push_back(new Particle(std::uniform_int_distribution<>(0, (windowW - 20))(eng), std::uniform_int_distribution<>(0, (windowH - 20))(eng)));
    }

    sf::Clock clock;
    sf::Time  time;

    std::vector<Point*> others;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        time = clock.getElapsedTime();
        clock.restart().asSeconds();
        printf("%f\n", 1.0f / time.asSeconds());

        QuadTree* qtree = new QuadTree(boundary, 4);

        for (int i = 0; i < (int)p.size(); i++)
        {
            Point* point = new Point(p[i]->getX(), p[i]->getY(), p[i]);
            qtree->insertion(point);

            p[i]->move();
            p[i]->highlight = false;
        }

        for (int i = 0; i < (int)p.size(); i++)
        {
            Rectangle* range = new Rectangle(p[i]->getX(), p[i]->getY(), 8, 8);
            qtree->Query(range, others);

            for (int j = 0; j < (int)others.size(); j++)
            {
                if (p[i] != others[j]->getData() && p[i]->intersects(others[j]->getData()))
                {
                    p[i]->highlight = true;
                }
            }
            p[i]->render(window);
            delete range;
            others.clear();
        }
        delete qtree;
        window.display();
    }

    for (int i = 0; i < (int)p.size(); i++)
        delete p[i];

    delete boundary;

    return 0;
}

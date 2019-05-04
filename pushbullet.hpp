#ifndef PUSHBULLET_HPP
#define PUSHBULLET_HPP


#define API "https://api.pushbullet.com/v2/"

class Pushbullet
{
    char *token_;
public:
    Pushbullet(const char *token);
    ~Pushbullet();
    void print();
    void push(char *data);
    void push_note(const char *title, const char *body);
};

#endif // PUSHBULLET_HPP

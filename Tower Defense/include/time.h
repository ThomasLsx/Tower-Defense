#ifndef TIME_H
#define TIME_H

class Time {
private:
    float deltaTime;
    float elapsed;

public:
    Time();

    void tick(float dt);
    float getDelta() const;
    float getElapsed() const;
    void reset();
};

#endif // TIME_H

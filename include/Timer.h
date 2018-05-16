//
// Created by ftfnunes on 19/04/18.
//

#ifndef T1_TIME_H
#define T1_TIME_H


class Timer {
public:
    Timer();

    void Update(float dt);

    void Restart();

    float Get();
private:
    float time;
};


#endif //T1_TIME_H

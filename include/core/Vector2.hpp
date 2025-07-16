#pragma once

class Vector2
{

public:
    static const Vector2 UP;
    static const Vector2 DOWN;
    static const Vector2 LEFT;
    static const Vector2 RIGHT;
    static const Vector2 ZERO;

    int x;
    int y;



    Vector2(int x = 0, int y = 0);

    Vector2& operator += (const Vector2& v);

    static double Distance(Vector2&, Vector2&);
};

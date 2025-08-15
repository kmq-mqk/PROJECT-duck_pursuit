#pragma once
#include <raylib.h>
#include <string>

class Score {
public: 
    Score(int start = 10000, double tickSeconds = 0.01, int perTick = 7);

    void Start();
    void Pause(bool p);
    void Reset(int newStart = -1);
    void Update();
    void Draw(int x, int y, int fontSize = 32, Color color = BLACK) const;
    
    int  Value() const { return value; }
    bool IsFinished() const { return value <= 0; }

private:
    int startValue;     // điểm bắt đầu
    int value;          // điểm hiện tại
    double tick;        // số giây mỗi lần trừ
    int per;            // mỗi tick trừ bao nhiêu điểm
    double acc;         // bộ tích lũy thời gian
    bool running;       // đang đếm không
    double lastTime;    // mốc thời gian lần trước (sử dụng GetTime())
};
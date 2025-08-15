#include "score.hpp"
#include <raylib.h> // Để dùng GetTime(), DrawText, TextFormat

// Khởi tạo điểm bắt đầu, thời gian mỗi tick, và số điểm trừ mỗi tick
Score::Score(int start, double tickSeconds, int perTick)
: startValue(start), value(start), tick(tickSeconds), per(perTick),
  acc(0.0), running(false), lastTime(0.0) {}

// Bắt đầu hoặc tiếp tục đếm điểm
void Score::Start() {
    running = true;
    lastTime = GetTime(); // Lưu thời gian hiện tại làm mốc
}

// Tạm dừng hoặc tiếp tục (Pause(true) để dừng, Pause(false) để tiếp tục)
void Score::Pause(bool p) {
    if (p) running = false;
    else { running = true; lastTime = GetTime(); }
}

// Reset điểm; nếu newStart < 0 thì reset về giá trị startValue ban đầu
void Score::Reset(int newStart) {
    if (newStart >= 0) startValue = newStart;
    value = startValue;
    acc = 0.0;
    running = false;
    lastTime = 0.0;
}

// Cập nhật điểm theo thời gian
void Score::Update() {
    if (!running || value <= 0) return; // Nếu dừng hoặc điểm <= 0 thì không làm gì

    double now = GetTime();
    acc += (now - lastTime); // Tích lũy thời gian trôi qua
    lastTime = now;

    // Trừ điểm mỗi khi đủ tick thời gian
    while (acc >= tick && value > 0) {
        value -= per;
        if (value < 0) value = 0; // Không cho điểm âm
        acc -= tick;
    }
}

static std::string FormatScore(int v) {
    std::string s = std::to_string(v);
    int insertPos = s.length() - 3;
    while (insertPos > 0) {
        s.insert(insertPos, ",");
        insertPos -= 3;
    }
    return s;
}

void Score::Draw(int x, int y, int fontSize, Color color) const {
    std::string txt = "SCORE: " + FormatScore(value);
    DrawText(txt.c_str(), x, y, fontSize, color);
}

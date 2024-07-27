#include <cmath>
#include <thread>
#include "thread"
#include "vofa_bridge/vofa_bridge.h"

using namespace std::chrono_literals;

int main() {
    using namespace vpie;
    auto& v = VofaBridge::Get();
    v.Record(1, 2, 3, 4, 5);
    v.Record(2, 3, 4, 5, 6);
    v.Record(3, 4, 5, 6, 7);
    for (auto&& i = 1; i < 500; ++i) {
        auto num = static_cast<float>(i) / 10.f;
        v.PushData(std::sin(num));
        v.PushData(std::cos(num));
        v.PushData(std::tan(num));
        v.SendData();
        std::this_thread::sleep_for(100ms);
    }
    std::this_thread::sleep_for(1000ms);
    v.ClearData();

    for (auto&& i = 1; i < 500; ++i) {
        auto num = static_cast<float>(i) / 10.f;
        v.AssignData(std::sin(num), std::cos(num), std::tan(num));
        v.SetData(3, i);
        v.SendData();
        std::this_thread::sleep_for(100ms);
    }
    std::this_thread::sleep_for(1000ms);
    v.ClearData();

    v.SendOnce(3, 1, 4);
    std::this_thread::sleep_for(1000ms);
    v.SendOnce(6, 2, 8);
    std::this_thread::sleep_for(1000ms);
    v.SendOnce(9, 4, 2);

    while (true) {
        auto& v = VofaBridge::Get();
        v.SendOnce(1, 2, 3);
        std::this_thread::sleep_for(10ms);
    }

    return 0;
}
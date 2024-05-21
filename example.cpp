#include <cmath>
#include <thread>
#include "thread"
#include "vofa_bridge.hpp"

using namespace std::chrono_literals;

int main() {
    using namespace vpb;
    VofaBridge vb = VofaBridge::Get();
    for (auto&& i = 1; i < 500; ++i) {
        auto num = static_cast<float>(i) / 10.f;
        vb.PushData(std::sin(num));
        vb.PushData(std::cos(num));
        vb.PushData(std::tan(num));
        vb.SendData();
        std::this_thread::sleep_for(100ms);
    }
    std::this_thread::sleep_for(1000ms);
    vb.ClearData();

    for (auto&& i = 1; i < 500; ++i) {
        auto num = static_cast<float>(i) / 10.f;
        vb.AssignData(std::sin(num), std::cos(num), std::tan(num));
        vb.SetData(3, i);
        vb.SendData();
        std::this_thread::sleep_for(100ms);
    }
    std::this_thread::sleep_for(1000ms);
    vb.ClearData();

    vb.SendOnce(3, 1, 4);
    std::this_thread::sleep_for(1000ms);
    vb.SendOnce(6, 2, 8);
    std::this_thread::sleep_for(1000ms);
    vb.SendOnce(9, 4, 2);

    return 0;
}
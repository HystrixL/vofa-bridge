#include <thread>
#include "vofa_bridge.hpp"
#include "thread"

using namespace std::chrono_literals;

int main() {
    using namespace vpb;
    VofaBridge vb = VofaBridge::Get();
    int i = 0;
    while (true) {
        ++i;
        vb.PushData(i,2.0,'a');
        vb.SendData();
        std::this_thread::sleep_for(500ms);
    }
}
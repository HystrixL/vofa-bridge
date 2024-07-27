#include <vofa_bridge/vofa_bridge.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

struct DataFrame {
    DataFrame(const long& tp, const std::vector<float>& data) : tp_(tp), data_(data) {}
    long tp_{};
    std::vector<float> data_{};
};

std::vector<DataFrame> Read(std::ifstream& fs) {
    std::vector<DataFrame> res{};
    std::string line;
    while (std::getline(fs, line)) {
        std::istringstream iss(line);
        long tp{};
        iss >> tp;

        std::vector<float> numbers;
        float number;

        while (iss >> number) {
            numbers.push_back(number);
        }

        res.emplace_back(tp, numbers);
    }

    return res;
}

void Resend(const std::vector<DataFrame>& data_frames) {
    auto& vb = vpie::VofaBridge::Get();
    for (const DataFrame& data_frame : data_frames) {
        static long last_tp = data_frame.tp_;
        long now_tp = data_frame.tp_;

        std::this_thread::sleep_for(std::chrono::nanoseconds{now_tp - last_tp});
        for (const float& f : data_frame.data_) {
            vb.PushData(f);
        }
        vb.SendData();

        last_tp = now_tp;
    }
}

int main() {
    std::ifstream fs{"vofa.txt"};
    auto r = Read(fs);
    Resend(r);
    fs.close();
}
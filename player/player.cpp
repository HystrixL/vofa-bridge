#include <fmt/core.h>
#include <vofa_bridge/vofa_bridge.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

struct DataFrame {
    DataFrame() = default;
    long tp_{};
    std::vector<float> data_{};
};

void ReadAndSend(std::ifstream& fs) {
    auto& vb = vpie::VofaBridge::Get();
    std::string line;
    while (std::getline(fs, line)) {
        DataFrame data_frame{};
        std::istringstream iss(line);

        iss >> data_frame.tp_;

        float number;
        while (iss >> number) {
            data_frame.data_.push_back(number);
        }

        static long last_tp = data_frame.tp_;
        long now_tp = data_frame.tp_;

        std::this_thread::sleep_for(std::chrono::nanoseconds{now_tp - last_tp});
        for (const float& f : data_frame.data_) {
            vb.PushData(f);
        }
        vb.SendData();

        last_tp = now_tp;

        fmt::print("play 1 frame.\n");
    }
}

int main() {
    std::ifstream fs{"vofa_record.txt"};
    ReadAndSend(fs);
    fs.close();
}
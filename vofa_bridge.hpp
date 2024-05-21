#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <memory>
#include <string_view>
#include <vector>
#include "array"

namespace vpb {

template <typename T>
struct Singleton {
    constexpr static T& Get() {
        static T instance{};
        return instance;
    }

   protected:
    constexpr Singleton() noexcept = default;

    constexpr Singleton(const Singleton&) noexcept = default;

    Singleton<T>& operator=(const Singleton&) noexcept = default;

    constexpr Singleton(Singleton&&) noexcept = default;

    Singleton<T>& operator=(Singleton&&) noexcept = default;
};

class VofaBridge : public Singleton<VofaBridge> {
   private:
    template <std::size_t len>
    struct [[gnu::packed]] Frame {
        std::array<float, len> data{};
        const unsigned char tail[4]{};
    };

    void Open(std::string_view ip, unsigned short port) {
        sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);

        memset(&addr_serv_, 0, sizeof(addr_serv_));
        addr_serv_.sin_family = AF_INET;
        addr_serv_.sin_addr.s_addr = inet_addr(ip.data());
        addr_serv_.sin_port = htons(port);
    }

    void Close() const { close(sock_fd_); }

   public:
    /**
     * @brief 直接发送一次数据
     *
     * @param data 发送的数据
     */
    void SendOnce(auto&&... data) {
        Frame<sizeof...(data)> frame{};
        frame.data = {static_cast<float>(data)...};
        frame.tail = data_tail_;
        sendto(sock_fd_, reinterpret_cast<char*>(&frame), sizeof(frame), 0, reinterpret_cast<sockaddr*>(&addr_serv_),
               sizeof(addr_serv_));
    }

    /**
     * @brief 向缓冲区尾部插入指定数据
     *
     * @param data 插入的数据
     */
    void PushData(auto&&... data) {
        (..., data_buffer_.push_back(static_cast<float>(data)));
    }
    /**
     * @brief 重新赋值缓冲区所有数据
     *
     * @param data 赋值的数据
     */
    void AssignData(auto&&... data) {
        data_buffer_.resize(sizeof...(data));
        data_buffer_ = {static_cast<float>(data)...};
    }

    /**
     * @brief 将缓冲区指定下标位置设置为给定数据
     *
     * @param index 设置的下标
     * @param data 设置的数据
     */
    void SetData(std::size_t index, auto&& data) {
        if (index > data_buffer_.size() - 1)
            data_buffer_.resize(index + 1);
        data_buffer_[index] = static_cast<float>(data);
    }

    /**
     * @brief 发送缓冲区内的所有数据
     */
    void SendData() {
        if (data_buffer_.empty())
            return;
        std::unique_ptr<char> ptr = std::make_unique<char>(sizeof(float) * data_buffer_.size() + data_tail_.size());
        std::memcpy(ptr.get(), data_buffer_.data(), sizeof(float) * data_buffer_.size());
        std::memcpy(ptr.get() + sizeof(float) * data_buffer_.size(), data_tail_.data(), data_tail_.size());
        sendto(sock_fd_, ptr.get(), sizeof(float) * data_buffer_.size() + data_tail_.size(), 0,
               reinterpret_cast<sockaddr*>(&addr_serv_), sizeof(addr_serv_));
        ClearData();
    }

    /**
     * @brief 清空缓冲区内的所有数据
     */
    void ClearData() {
        data_buffer_.clear();
        data_buffer_.resize(0);
    }

    /**
     * @brief 重置与vofa+的连接
     *
     * @param ip vofa+监听ip
     * @param port vofa+监听端口
     */
    void Reset(std::string_view ip, unsigned short port) {
        Close();
        Open(ip, port);
    }

    VofaBridge() { Reset("127.0.0.1", 1347); };

    ~VofaBridge() { Close(); }

   private:
    int sock_fd_{};
    sockaddr_in addr_serv_{};
    std::vector<float> data_buffer_{};
    constexpr static std::array<unsigned char, 4> data_tail_{0x00, 0x00, 0x80, 0x7f};
};
}  // namespace vpb
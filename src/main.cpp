#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

int main() {
    std::vector<int> vec;
    vec.resize(10);
    std::iota(vec.begin(), vec.end(), 1);
    std::ranges::for_each(vec, [](int i) { std::cout << i << std::endl; });
}

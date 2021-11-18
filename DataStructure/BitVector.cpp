#include <vector>

using namespace std;
using uint = unsigned int;
// This implementation of bit vector is not succinct, (Memory usage is not N + o(N) bits)
// but compact (Memory usage is 2N = O(N) bits)
class BitVector {
    int size;
    int block_num;
    const static int block_width;
    std::vector<uint> blocks;

    explicit BitVector(const std::vector<bool> a) {
        size = (int)a.size();
        block_num = (size + block_width - 1) / block_width;

    }
};
const int BitVector::block_width = 32;
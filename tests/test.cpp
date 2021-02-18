#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
//#include "partition_and_pq.h"
int main(int argc, char** argv) {
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    size_t num_pq_chunks = 4;
    size_t dim = 128;
    char * pq_pivots_path = "aaa";
    size_t train_size = 10000;
    std::vector<float> train_data;
    for(int i =0;i<train_size; i++) {
        for(int j =0;j<dim;j++) {
            train_data.push_back(dis(gen));
        }
    }
    /*generate_pq_pivots(train_data.data(), train_size, (uint32_t) dim, 256,
                       (uint32_t) num_pq_chunks, 15, pq_pivots_path);*/


    size_t low_val = (size_t) std::floor((double) dim / (double) num_pq_chunks);
    size_t high_val = (size_t) std::ceil((double) dim / (double) num_pq_chunks);
    size_t max_num_high = dim - (low_val * num_pq_chunks);
    size_t cur_num_high = 0;
    size_t cur_bin_threshold = high_val;
    std::cout<<low_val<<","<<high_val<<"\n";

    std::vector<std::vector<uint32_t>> bin_to_dims(num_pq_chunks);
    std::unordered_map<uint32_t, uint32_t> dim_to_bin;
    std::vector<float> bin_loads(num_pq_chunks, 0);

    // Process dimensions not inserted by previous loop
    for (uint32_t d = 0; d < dim; d++) {
        if (dim_to_bin.find(d) != dim_to_bin.end())
        continue;
        auto  cur_best = num_pq_chunks + 1;
        float cur_best_load = std::numeric_limits<float>::max();
        for (uint32_t b = 0; b < num_pq_chunks; b++) {
            if (bin_loads[b] < cur_best_load && bin_to_dims[b].size() < cur_bin_threshold) {
                cur_best = b;
                cur_best_load = bin_loads[b];
            }
        }
        std::cout << " Pushing " << d << " into bin #: " << cur_best
                    << std::endl;
        bin_to_dims[cur_best].push_back(d);
        if (bin_to_dims[cur_best].size() == high_val) {
            cur_num_high++;
            if (cur_num_high == max_num_high)
                cur_bin_threshold = low_val;
        }
    } // end for d
    std::for_each(bin_to_dims[0].begin(), bin_to_dims[0].end(), [](uint32_t d){std::cout<<d<<",";});
    std::cout<<std::endl;

    std::vector<uint32_t> rearrangement;
    std::vector<uint32_t> chunk_offsets;
    rearrangement.clear();
    chunk_offsets.clear();
    chunk_offsets.push_back(0);

    for (uint32_t b = 0; b < num_pq_chunks; b++) {
        std::cout << "[ ";
        for (auto p : bin_to_dims[b]) {
        rearrangement.push_back(p);
        std::cout << p << ",";
        }
        std::cout << "] " << std::endl;
        if (b > 0)
        chunk_offsets.push_back(chunk_offsets[b - 1] +
                                (unsigned) bin_to_dims[b - 1].size());
    }
    chunk_offsets.push_back(dim);

    std::for_each(chunk_offsets.begin(), chunk_offsets.end(), [](uint32_t d){std::cout<<d<<",";});
    std::cout<<std::endl;

    for (auto p : rearrangement)
        std::cout << p << " ";
    std::cout << std::endl;
}
#include <iostream>
#include <immintrin.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <random>
#include <error.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

namespace fs = std::filesystem;


bool create_file(){
    std::cout << "1. File\n";

    if(fs::is_directory("./test_opt")){
        std::ifstream file("./test_opt/numbers.txt");
        if(file) {
            return true;
        }else{
            std::ofstream newfile("./test_opt/numbers.txt");
            return true;
        }
    }else{
        fs::create_directory("./test_opt");
        std::ofstream newfile("./test_opt/numbers.txt");
        return true;
    }
}

bool generate_numbers(){

    std::cout << "2. Numbers\n";

    unsigned short num{0};
    std::fstream file("./test_opt/numbers.txt");

    for(int i = 0;i <= 1000000; i++){
        num = rand() % 99+1;
        file << num << " ";
    }
    return true;
}

//64 000 - 68 000 microseconds
void first_case(){
    std::fstream file("./test_opt/numbers.txt");
    unsigned short a,b;

    auto start = std::chrono::high_resolution_clock::now();

    while(file >> a >> b){
        unsigned short result = a * b;
        result -=1;
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto res = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

    std::cout << res.count() << std::endl;
}

//1500~
void second_case(){
    std::fstream file("./test_opt/numbers.txt");
    std::vector<unsigned short> numbers;
    unsigned short num;

    while(file >> num){
        numbers.push_back(num);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for(size_t i = 0; i <= 500000; i += 2){
        unsigned short res = numbers[i]*numbers[i+1];
        res -=1;
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto res = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

    std::cout << res.count() << std::endl;

}
//6000~ 400-+
void third_case(){
    const char* file_path = "./test_opt/numbers.txt";
    int fd = open(file_path, O_RDONLY);  // Open the file in read-only mode
    if (fd == -1) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
    }

    // Get the file size
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        std::cerr << "Failed to get file size" << std::endl;
        close(fd);
    }

    // Map the file into memory
    void* mapped_data = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_data == MAP_FAILED) {
        std::cerr << "Failed to map file into memory" << std::endl;
        close(fd);
    }

    // Use the mapped data (e.g., parse numbers)
    const char* data = static_cast<const char*>(mapped_data);
    std::stringstream ss(data);
    std::vector<std::pair<unsigned short, unsigned short>> numbers;
    unsigned short a,b;

    while(ss >> a >> b){
        numbers.emplace_back(a,b);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for(const auto& pair : numbers){
        unsigned short result = pair.first*pair.second;
        result -=1;
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto res = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

    std::cout << res.count() << std::endl;

    // Unmap the file
    if (munmap(mapped_data, file_size) == -1) {
        std::cerr << "Failed to unmap file" << std::endl;
    }

    // Close the file
    close(fd);
}

//700-800
void forth_case(){
            // Read numbers from file
        std::fstream file("./test_opt/numbers.txt");
        std::vector<unsigned short> numbers;
        unsigned short num;

        while (file >> num) {
            numbers.push_back(num);
        }

        // Ensure the size of the vector is a multiple of 16 (for 16 x 16-bit integers in __m256i)
        if (numbers.size() % 16 != 0) {
            numbers.resize(numbers.size() + (16 - numbers.size() % 16), 0);
        }

        auto start = std::chrono::high_resolution_clock::now();

        // Process numbers in chunks of 16 (8 pairs)
        for (size_t i = 0; i < numbers.size(); i += 16) {
            // Load 16 x 16-bit integers into two __m256i vectors
            __m256i vec_a = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&numbers[i]));
            __m256i vec_b = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&numbers[i + 8]));

            // Multiply the vectors (16-bit multiplication)
            __m256i vec_mul = _mm256_mullo_epi16(vec_a, vec_b);

        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto res = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << res.count() << std::endl;
}

int main(int argc, char **argv) {

    try{
        create_file();
        generate_numbers();
    }catch(fs::filesystem_error e){
        std::cout << e.what() << std::endl;
    }

    first_case();
    second_case();
    third_case();
    forth_case();

    return 0;
}

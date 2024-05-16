/*
 * @Author: Ivan Chichvarin ichichvarin@humanplus.ru
 * @Date: 2024-05-02 23:07:13
 * @LastEditors: Ivan Chichvarin ichichvarin@humanplus.ru
 * @LastEditTime: 2024-05-09 17:38:04
 * @FilePath: /moveSemantics/move_semantics.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    vector<typename RandomIt::value_type> pool;
    size_t vector_size = static_cast<size_t>(std::distance(first,last));
    size_t step_count = 0;  
    pool.reserve(vector_size);
    RandomIt iterator = first;
  
    pool.push_back(std::move(*iterator));
    
    size_t cur_pos = (step_size) % vector_size;
    std::advance(iterator, cur_pos);

    while(step_count != vector_size-1){

        while(std::find(pool.begin(), pool.end(), *iterator) != pool.end()) {//contains
            if(std::next(iterator, 1)!= last){
                    std::advance(iterator, 1);
            }else{
                    iterator = first;
            }
        }
  
        pool.push_back(std::move(*iterator));
        
        if(++step_count == vector_size-1){
            RandomIt poolIterator = pool.begin();
            while(first!=last){
                *(first++) = move(*poolIterator++);
            }
            break;
        }
        
        cur_pos = 0;
        if(std::next(iterator, 1)!= last){
            std::advance(iterator, 1);
        }else{
            iterator = first;
        }
        
        while(cur_pos != step_size-1){
            while(std::find(pool.begin(), pool.end(), *iterator) != pool.end()) {//contains
                if(std::next(iterator, 1)!= last){
                    std::advance(iterator, 1);
                }else{
                    iterator = first;
                }
            }
            if(std::next(iterator, 1)!= last){
                std::advance(iterator, 1);
            }else{
                    iterator = first;
            }
             cur_pos++;
            }
            
        }
}

vector<int> MakeTestVector() {
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        assert(numbers_copy == numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        assert(numbers_copy == vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем
// об этом далее в нашем курсе

struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator==(const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator<<(ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

void TestAvoidsCopying() {
    vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    assert(numbers == expected);
}

int main() {
    TestIntVector();
    TestAvoidsCopying();
    return 0;
}
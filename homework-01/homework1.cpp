#pragma once
#include <iostream>

// range‑based loop
template <typename Container>
void print_range(const Container& cont)
{
    for (const auto& elem : cont)          
        std::cout << elem << ' ';
    std::cout << '\n';
}

// auto iterator

template <typename Container>
void print_iter(const Container& cont)
{
    for (auto it = cont.begin(); it != cont.end(); ++it)   
        std::cout << *it << ' ';
    std::cout << '\n';
}
  
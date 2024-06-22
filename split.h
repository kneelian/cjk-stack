#pragma once

template<typename T> std::vector<T> split(const T& input, const T& delimiters) 
{
    std::vector<T> temporary;
    size_t start = 0;

    auto pos = input.find_first_of(delimiters, start);
    while(pos != size_t(-1)) {
        if(pos != start) // ignore empty tokens
        {
            temporary.emplace_back(
                input, 
                start, 
                pos - start
            ); 
        }
        start = pos + 1;
        pos = input.find_first_of(delimiters, start);
    }

    if(start < input.length()) // ignore trailing delimiter
    { 
        temporary.emplace_back(
            input, 
            start, 
            input.length() - start
        ); 
    }
    return temporary;
}
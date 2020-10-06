/** This is a very naive solotions for searching needle in a haystack.
  * At every index, it checks if the substring is equal to the needle.
  * Complexity is O(N*K), where N = haystack length and K = needle length. 

#include <string>
#include <iostream>
#include <cassert>
#include <vector>

/** Returns true if the next needle.size chars are equal in haystack from position i. */ 
bool match(std::string haystack, std::string needle, size_t i)
{
    for(size_t j = 0; j < needle.size(); ++ i, ++ j)
        if(haystack[i] != needle[j])
            return false;
    return true;
}

std::vector<int> contains(std::string haystack, std::string needle)
{
    std::vector<int> res;
    for(size_t i = 0; i < haystack.size() - needle.size() + 1; ++ i)
        if(match(haystack, needle, i))
            res.emplace_back(i);
    return res;
}

int main(void)
{
    assert(match("Hello", "Hello", 0));
    assert(match("Hello", "Hell", 0));
    assert(!match("Hell", "Hello", 0));
    assert(contains("Hello hello", "el") == std::vector<int>({1, 7}) );
    assert(contains("Hello hello", "hel") == std::vector<int>({6}) );
    assert(contains("Hello hello", "o h") == std::vector<int>({4}) );
    assert(contains("Hello hello", "oxh") == std::vector<int>({}) );
    assert(contains("Hello hello", "lo") == std::vector<int>({3, 9}) );
    assert(contains("Hello hello", "o") == std::vector<int>({4, 10}) );
}
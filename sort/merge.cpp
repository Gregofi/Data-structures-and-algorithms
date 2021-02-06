#include <vector>
#include <iterator>
#include <iostream>


std::vector<int> merge(const std::vector<int> &v1, const std::vector<int> &v2) 
{
    auto it1 = v1.begin();
    auto it2 = v2.begin();
    std::vector<int> v;
    v.reserve(v1.size() + v2.size());
    while(it1 != v1.end() || it2 != v2.end())
    {
        if(it1 == v1.end() && it2 != v2.end()) 
        {
            v.push_back(*it2); 
            ++ it2;
        }
        else if(it2 == v2.end() && it1 != v1.end())
        {
            v.push_back(*it1);
            ++ it1;
        }
        else if(*it1 <= *it2){
            v.push_back(*it1);
            ++ it1;
        }
        else if(*it1 > *it2)
        {
            v.push_back(*it2);
            ++ it2;
        }
    }
    return v;
}

std::vector<int> merge_sort(const std::vector<int> &v)
{
    if(v.size() <= 1)
        return v;
    auto v1 = merge_sort(std::vector<int>(v.begin(), std::next(v.begin(), v.size() / 2)));
    auto v2 = merge_sort(std::vector<int>(std::next(v.begin(), v.size() / 2), v.end()));
    return merge(v1, v2);
}

/* ======================================================================== */

#include <chrono> 
using namespace std::chrono; 

int main(void)
{
    std::ios_base::sync_with_stdio(false);
    std::vector<int> r;
    r.reserve(1000000);
    while(!std::cin.eof())
    {
        int i;
        std::cin >> i;
        r.push_back(i);
    }
    auto start = high_resolution_clock::now();
    merge_sort(r);
    auto stop = high_resolution_clock::now();
    std::cout << duration_cast<milliseconds>(stop - start).count() / 1000. << std::endl; 
}

#include <cstddef>
#include <cassert>

template <typename T>
void swap(T &x, T &y) noexcept
{
  T tmp = x;
  x = y;
  y = tmp;
}

class bin_heap
{
public:
  bin_heap() : size(0){}
  ~bin_heap() = default;  
  void insert(int i);
  int get_min();
  void extract_min();
private:
  void bubble_up(size_t i);
  void bubble_down(size_t i);
  static const int MAX_SIZE = 1000000;
  int arr[MAX_SIZE];
  size_t size;
};

int bin_heap::get_min()
{
  return arr[1];
}

void bin_heap::insert(int i)
{
  size ++;
  arr[size] = i;
  bubble_up(size);
}

void bin_heap::bubble_up(size_t i)
{
  while(i / 2 > 0)
  {
    if(arr[i / 2] > arr[i])
    {
      swap(arr[i / 2], arr[i]);
      i /= 2;
    }
    else
      break;
  }
}

void bin_heap::bubble_down(size_t i)
{
  while(size >= i*2)
  {
    size_t n;
    if(size < i*2 + 1)
      n = i*2;
    else 
      n = i*2 + (arr[i*2] > arr[i*2 + 1]);
    swap(arr[i], arr[n]);
    i = n;
  }
}

void bin_heap::extract_min()
{
  swap(arr[1], arr[size]);
  size --;
  bubble_down(1);
}

/* ====================== TESTS ====================== */
#include <random>

void basic_tests()
{
  bin_heap h1;
  h1.insert(1);
  h1.insert(2);
  h1.insert(3);
  assert(h1.get_min() == 1);
  h1.extract_min();
  assert(h1.get_min() == 2);
  h1.extract_min();
  assert(h1.get_min() == 3);
  h1.extract_min();

  h1.insert(4);
  assert(h1.get_min() == 4);
  h1.insert(27);
  assert(h1.get_min() == 4);
  h1.insert(3);
  assert(h1.get_min() == 3);
  h1.insert(1);
  assert(h1.get_min() == 1);
}

#include <set>
#define TEST_SIZE 500000
void random_test()
{
  std::random_device rd;
  std::mt19937 e{rd()};

  std::multiset<int> ms;
  bin_heap h;
  for(size_t i = 0; i < TEST_SIZE; ++ i)
  {
    int x = e();
    ms.insert(x);
    h.insert(x);
    if(i % 5 == 0)
    {
      assert(h.get_min() == *ms.begin());
      h.extract_min();
      ms.erase(ms.begin());
    }
  }
}

int main()
{
  basic_tests();
  random_test();
}
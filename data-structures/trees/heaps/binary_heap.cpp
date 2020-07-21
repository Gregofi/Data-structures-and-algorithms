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
  bin_heap() = default;
  ~bin_heap() = default;  
  void insert(int i);
  int get_min();
  void extract_min();
private:
  void bubble_up(size_t i);
  void bubble_down(size_t i);
  static const int MAX_SIZE = 100000;
  int arr[MAX_SIZE];
  size_t size;
};

int bin_heap::get_min()
{
  return arr[1];
}

void bin_heap::insert(int i)
{
  arr[size] = i;
  bubble_up(arr[size]);
  size ++;
}

void bin_heap::bubble_up(size_t i)
{
  while(i > 0)
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
  while(true)
  {
    if(size < i*2)
      break;
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
  bubble_down(arr[1]);
  size --;
}

/* ====================== TESTS ====================== */

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

}


int main()
{

}
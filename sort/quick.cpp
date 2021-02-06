#include <cstdio>
#include <utility>
#include <vector>

int partition(std::vector<int> &arr, int lo, int hi)
{
  int pivot = arr[hi];
  int i = lo;
  for(int j = lo; j < hi; ++ j)
  {
    if(arr[j] < pivot)
    {
      std::swap(arr[i], arr[j]);
      ++ i;
    }
  }
  std::swap(arr[i], arr[hi]);
  return i;
}

void quicksort(std::vector<int> &arr, int lo, int hi)
{
  if(lo < hi)
  {
    int p = partition(arr, lo, hi);
    quicksort(arr, lo, p - 1);
    quicksort(arr, p + 1, hi);
  }
}

int main(void)
{
  std::vector<int> arr = {3, 1, 2, 5, 6, 4};
  quicksort(arr, 0, arr.size() - 1);
  for(int i = 0; i < arr.size(); ++ i)
    printf("%d ", arr[i]);
  puts("");
}
#include <utility>
#include <cassert>

struct node
{
  node(int val);
  ~node();
  node *r, *l;
  int val;
};

node::node(int val) : r(nullptr), l(nullptr), val(val)
{

}

node::~node()
{
  delete r;
  delete l;
}

class bst
{
public:
  bst();
  ~bst();
  bool insert(int i);
  bool erase(int i);
  bool contains(int i);
private:
  node *root;
};

bst::bst() : root(nullptr)
{

}

bst::~bst()
{
  delete root;
}

bool bst::insert(int i)
{
  node **n = &root;
  while(*n)
  {
    if((*n)->val > i)
      n = &(*n)->l;
    else if((*n)->val < i)
      n = &(*n)->r;
    else
      break;
  }
  *n = new node(i);
  return true;
}

bool bst::erase(int i)
{
  node **n = &root;
  while(*n)
  {
    if((*n)->val > i)
      n = &(*n)->l;
    else if((*n)->val < i)
      n = &(*n)->r;
    else
    {
      node *v = *n;
      if((*n)->r && (*n)->l)
      {
        n = &(*n)->r;
        /* Find the leftmost node in right subtree */
        while((*n)->l)
          n = &(*n)->l;
        std::swap((*n)->val, v->val);
        v = (*n);
      }
      if((*n)->l)
      {
        (*n) = (*n)->l;
        v->l = nullptr;
      }
      else if((*n)->r)
      {
        (*n) = (*n)->r;
        v->r = nullptr;
      }
      else
        (*n) = nullptr;
      
      delete v; 
      return true;
    }
  }
  return false;
}

bool bst::contains(int i)
{
  node *n = root;
  while(n)
  {
    if(n->val > i)
      n = n->l;
    else if(n->val < i)
      n = n->r;
    else
      return true;
  }
  return false;
}

void basic_test(void)
{
  bst t1;
  assert(t1.insert(12));
  assert(t1.insert(7));
  assert(t1.insert(233));
  assert(t1.contains(12));
  assert(t1.contains(7));
  assert(t1.contains(233));
  assert(!t1.contains(0));

  assert(t1.insert(5));
  assert(t1.insert(6));
  assert(t1.insert(3));
  assert(t1.contains(5));
  assert(t1.contains(6));
  assert(t1.contains(3));

  assert(t1.erase(5));
  assert(t1.contains(3));
  assert(t1.contains(6));
  assert(!t1.contains(5));

  assert(t1.erase(12));
  assert(t1.contains(7));
  assert(t1.contains(233));
  assert(t1.contains(3));
  assert(t1.contains(6));
  assert(!t1.contains(12));

  bst t2;
  t2.insert(5);
  t2.insert(1);
  t2.insert(10);
  t2.insert(8);
  t2.erase(8);
  assert(t2.contains(10));
  assert(!t2.contains(8));
}


#include <vector>
#include <random>

#define TEST_SIZE 4000

void random_test(void)
{
  std::random_device rd;
  std::mt19937 e{rd()};
  std::vector<int> vals;
  bst t1;
  for(size_t i = 0; i < TEST_SIZE; ++i)
  {
    int r = e();
    vals.push_back(r);
    t1.insert(r);
    if(i % 4 == 0)
    {
      int index = e() % vals.size();
      t1.erase(vals[index]);
      vals.erase(std::next(vals.begin(), index));
    }
  }

  for(const auto & x : vals)
    assert(t1.contains(x));
}

int main(void)
{
  basic_test();
  random_test();
}
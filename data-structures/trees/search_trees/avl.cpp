#include <algorithm>
#include <cassert>
#include <string>
#include <ostream>

struct node
{
  node(int val);
  ~node();
  node *r, *l;
  int val;
  int balance;
  int height;
  void dump(std::ostream &, std::string, std::string) const;
};

node::node(int val) : r(nullptr), l(nullptr), val(val), balance(0), height(1)
{

}

node::~node()
{
  delete r;
  delete l;
}


class avl
{
public:
  avl();
  ~avl();
  bool insert(int i);
  bool erase(int i);
  bool contains(int i) const;
  void pretty_print(std::ostream &os) const;
  void check_balance(node *n = nullptr) const;
private:
  node* rebalance_insert(node *);
  node* rebalance_erase(node *);
  node* right_rot(node *pivot);
  node* left_rot(node *pivot);
  static void update_height(node *n);
  int get_balance(node *n) const;
  int get_height(node *n) const;
  bool _insert(int, node **);
  bool _erase(int, node **);
  bool _contains(int val, node *n) const;
  node *root;
};

void avl::check_balance(node *n) const
{
  if(!n)
    return;
  auto abs = [](int num)
  {
    return num > 0 ? num : -num;
  };
  if(!n)
    n = root;
  assert(abs(get_balance(n)) < 2);
  check_balance(n->r);
  check_balance(n->l);
}

void avl::pretty_print(std::ostream &os) const
{
  root->dump(os, "", " ");
}

avl::avl() : root(nullptr)
{

}

avl::~avl()
{
  delete root;
}

void avl::update_height(node *n)
{
  if(!n)
    return;
  n->height = 1 + std::max(n->l ? n->l->height : 0, n->r ? n->r->height : 0);
}

int avl::get_balance(node *n) const
{
  return (n->r ? n->r->height : 0 ) - ( n->l ? n->l->height : 0 );
}

node* avl::rebalance_insert(node *n)
{
  if(get_balance(n) < -1)
  {
    if(n->l->r && n->l->r->val > n->l->val)
      n->l = left_rot(n->l);
    n = right_rot(n);
  }
  if(get_balance(n) > 1)
  {
    if(n->r->l && n->r->l->val > n->r->val)
      n->r = right_rot(n->r);
    n = left_rot(n);
  }
  update_height(n->r);
  update_height(n->l);
  update_height(n);
  return n;
}

node* avl::rebalance_erase(node *n)
{
  if(get_balance(n) < -1)
  {
    if(n->l && get_balance(n->l) > 0)
      n->l = left_rot(n->l);
    n = right_rot(n);
  }
  if(get_balance(n) > 1)
  {
    if(n->r && get_balance(n->r) < 0)
      n->r = right_rot(n->r);
    n = left_rot(n);
  }

  update_height(n->r);
  update_height(n->l);
  update_height(n);

  return n;
}

node* avl::right_rot(node *pivot)
{
  node *new_pivot = pivot->l;
  pivot->l = pivot->l->r;
  new_pivot->r = pivot;
  return new_pivot;
}

node* avl::left_rot(node *pivot)
{
  node *new_pivot = pivot->r;
  pivot->r = pivot->r->l;
  new_pivot->l = pivot;
  return new_pivot;
}

bool avl::insert(int i)
{
  return _insert(i, &root);
}

bool avl::_insert(int val, node **n)
{
  if(!*n)
  {
    *n = new node(val);
    return true;
  }
  bool ret;
  if((*n)->val < val)
  {
    ret = _insert(val, &(*n)->r);
    (*n)->balance = get_balance(*n); 
  }
  else if((*n)->val > val)
  {
    ret = _insert(val, &(*n)->l);
  }
  else
    return false;

  update_height(*n);

  *n = rebalance_insert(*n);

  return ret;
}

bool avl::erase(int val)
{
  return _erase(val, &root);
}

bool avl::_erase(int val, node **n)
{
  bool res;
  if(!*n)
    return false;
  if((*n)->val > val)
    res = _erase(val, &(*n)->l);
  else if((*n)->val < val)
    res = _erase(val, &(*n)->r);
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
  update_height(*n);

  *n = rebalance_erase(*n);
  return res;
}

bool avl::contains(int val) const
{
  return _contains(val, root);
}

bool avl::_contains(int val, node *n) const
{
  if(!n)
    return false;
  if(n->val < val)
    return _contains(val, n->r);
  if(n->val > val)
    return _contains(val, n->l);
  return true;
}

void node::dump(std::ostream &os, std::string prefix, std::string children) const
{
  os << prefix << "+-" << val << "(" << height << ")" << std::endl;
  if(l)
    l->dump( os, prefix + children, r ? "| " : "  " );
  if(r)
    r->dump( os, prefix + children, "  " );
}

/* ==================== TESTS ======================= */

#include <vector>
#include <random>
#include <iostream>
#include <set>

#define TEST_SIZE 70000

void basic_test(void)
{
  avl t1;
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

  avl t2;
  t2.insert(5);
  t2.insert(1);
  t2.insert(10);
  t2.insert(8);
  t2.erase(8);
  assert(t2.contains(10));
  assert(!t2.contains(8));

  avl t3;
  t3.insert(10);
  t3.insert(5);
  t3.insert(7);
  t3.pretty_print(std::cout);
  std::cout << "\n";
  t3.insert(15);
  t3.insert(9);
  t3.insert(8);
  t3.pretty_print(std::cout);
  std::cout << "\n";
  t3.insert(20);
  t3.insert(25);
  t3.pretty_print(std::cout);
  std::cout << "\n";

  avl t4;
  t4.insert(10);
  t4.insert(15);
  t4.insert(5);
  t4.insert(7);
  t4.erase(15);
  t4.pretty_print(std::cout);
  std::cout << "\n";
  t4.check_balance();
}



void random_test(void)
{
  std::random_device rd;
  std::mt19937 e{rd()};
  std::set<int> vals;
  std::set<int> removed_vals;
  avl t1;
  for(size_t i = 0; i < TEST_SIZE; ++i)
  {
    int r = e();
    vals.insert(r);
    t1.insert(r);
    #ifndef PERFORMANCE_TEST
    if(removed_vals.count(r))
      removed_vals.erase(r);
    t1.check_balance();
    #endif
  
    if(i % 4 == 0)
    {
      int index = e() % vals.size();
      t1.erase(*std::next(vals.begin(), index));
      #ifndef PERFORMANCE_TEST
      removed_vals.insert(*std::next(vals.begin(), index));
      #endif
      vals.erase(std::next(vals.begin(), index));
    }
  }
  #ifndef PERFORMANCE_TEST
  for(auto x : vals)
    assert(t1.contains(x));

  for(auto x : removed_vals)
    assert(!t1.contains(x));
  #endif
}



int main(void)
{
  basic_test();
  random_test();
}
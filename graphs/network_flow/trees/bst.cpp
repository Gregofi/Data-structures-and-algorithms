

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
  bool insert(int i);
  bool erase(int i);
private:
  node *root;
};

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
      node **toDel = n;
      if((*n)->r && (*n)->l)
      {
        n = &(*n)->r;
        /* Find the leftmost node in right subtree */
        while((*n)->l)
          n = &(*n)->l;
        swap((*n)->val, (*toDel)->val);
        toDel = n;
      }
      if((*toDel)->l)
      {
        (*toDel) = (*toDel)->l;
        (*n)->l = nullptr;
      }
      else if((*toDel)->r)
      {
        (*toDel) = (*toDel)->r;
        (*n)->r = nullptr;
      }
      *n = nullptr;
      delete *n; 
      return true;
    }
  }
  return false;
}
/** Finds Maximum flow and minimum cut for given network, input is
 *  M N, where M is number of vertexes and N is number of edges,
 *  x y z ,N times, where x and y is edge (x, y) and z is capacity of (x, y)
 *  c1, number of sources.
 *  s, c1 times, source vertex.
 *  c2, number of sinks.
 *  k, c2 times, sink vertex.
 */ 


#include <iostream>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <sstream>
#include <vector>
#include <array>
#include <queue>
#include <map>
#include <set>

#define MAX_P 302
#define MAX_R 302
size_t SOURCE = 0;
size_t SINK = 0;

std::array<std::array<int, MAX_P>, MAX_P>   
get_graph ( std::map<std::set<size_t>, size_t > & rel_order,
            size_t & cnt_r, size_t & cnt_p)
{
  std::array<std::array<int, MAX_P>, MAX_P> graph;
  for(auto & x : graph)
    x.fill(0);
  std::cin >> cnt_p >> cnt_r;
  SOURCE = cnt_p;
  SINK = cnt_p + 1;
  for(size_t i = 0; i < cnt_r; ++i)
  {
    size_t u, v;
    int x;
    std::cin >> u >> v >> x;
    graph[u][v] = x;
    rel_order[std::set<size_t>{u, v}] = i;
  }

  size_t cnt_source;
  std::cin >> cnt_source;
  while(cnt_source --)
  {
    size_t source;
    std::cin >> source;
    graph[SOURCE][source] = 10000;
  }

  size_t cnt_sink;
  std::cin >> cnt_sink;
  while(cnt_sink --)
  {
    size_t sink;
    std::cin >> sink;
    graph[sink][SINK] = 10000;
  }
  cnt_p += 2;
  return graph;
}

bool               
get_path ( const std::array<std::array<int, MAX_P>, MAX_P> & resid_graph,
           std::array<size_t, MAX_P> & parent,
           size_t node_cnt )
{
  
  std::array<bool, MAX_P> visited = {};
  std::queue<int> q;
  q.push(SOURCE);
  while(q.size())
  {
    int n = q.front();
    q.pop();
    for(size_t i = 0; i < node_cnt; ++ i)
      if( resid_graph[n][i] && !visited[i] )
      {
        visited[i] = true;
        parent[i] = n;
        q.push(i);
      }
  }
  return visited[SINK];
}

void 
bfs ( const std::array<std::array<int, MAX_P>, MAX_P> &  resid_graph,
            std::array<bool, MAX_P> & visited,
            size_t                    node_cnt )
{
  std::queue<size_t> q;
  q.push(SOURCE);
  visited[SOURCE] = true;
  while( q.size() )
  {
    size_t n = q.front();
    q.pop();
    for(size_t i = 0; i < node_cnt; ++ i)
      if( resid_graph[n][i] && !visited[i] )
      {
        visited[i] = true;
        q.push(i);
      }
  }
}

int                
main ( void )                 
{
  size_t cnt_p, cnt_r;
  std::map<std::set<size_t>, size_t > rel_order;
  std::array<std::array<int, MAX_P>, MAX_P> graph = get_graph(rel_order, cnt_r, cnt_p);
  /* Residual graph */
  std::array<std::array<int, MAX_P>, MAX_P> resid_graph = graph;

  std::array<size_t, MAX_P> parent;
  int max_flow = 0;
  /* While augumenting path exists */
  while(get_path(resid_graph, parent, cnt_p))
  {
    int flow = INT_MAX;
    /* Trace backward from sink to source and obtain path upgrade */
    for(size_t u = SINK; u != SOURCE; u = parent[u])
    {
      size_t v = parent[u];
      flow = std::min(flow, resid_graph[v][u]);
    }
    max_flow += flow;
    /* Upgrade path */
    for(size_t i = SINK; i != SOURCE; i = parent[i])
    {
      size_t u = parent[i];
      resid_graph[i][u] += flow;
      resid_graph[u][i] -= flow;
    }
  }

  std::array<bool, MAX_P> visited;
  bfs(resid_graph, visited, cnt_p);
  std::cout << "Max flow: " << max_flow << "\n";
  std::string output = "Edges to cut:\n";
  for(size_t i = 0; i < cnt_p; ++ i)
    for(size_t j = 0; j < cnt_p; ++ j)
      if( visited[i] && !visited[j] && graph[i][j] )
        output += std::to_string(i) + " " + std::to_string(j) + "\n";
  if(output.back() == ' ')
    output.pop_back();
  std::cout << output << "\n";
}


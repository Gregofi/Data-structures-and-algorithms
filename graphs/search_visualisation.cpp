#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <chrono>
#include <thread>
#include <deque>
#include <algorithm>


/* Two ncurses functions that prints menu and returns a selected choice */

void print_menu ( WINDOW *menu_win, int highlight, 
                  const std::string & m, const std::vector<std::string> & sel)
{
    int x, y, i;
    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    mvwprintw(menu_win, y++, x, "%s", m.c_str());
    for(i = 0; i < (int)sel.size(); ++i)
    {   if(highlight == i + 1) /* Highlight the present choice */
        {   wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, x, "%s", sel[i].c_str());
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", sel[i].c_str());
        ++y;
    }
    wrefresh(menu_win);
}

int render_menu( const std::string & m, const std::vector<std::string> & sel )
{
  clear();
  WINDOW * menu_win;
  int mid_y;
  int mid_x;
  /* Get middle of the screen */
  getmaxyx(stdscr, mid_y, mid_x);
  mid_y /= 2; mid_x /= 2;
  int highlight = 1;
  int choice = 0;
  int c;
  int n_choices = sel.size();
  /* Get longest menu item width */
  int menu_height = sel.size() + 5;
  int menu_width = m.size();
  for(auto const & x : sel)
    if((int)x.size() > menu_width) 
      menu_width = x.size();
  menu_width += 6;
  /* Create window */
  menu_win = newwin(menu_height, menu_width, mid_y - menu_height / 2, mid_x - menu_width / 2);
  keypad(menu_win, TRUE);
  refresh();
  while( true )
  {   
    print_menu(menu_win, highlight, m, sel);
    c = wgetch(menu_win);
    switch(c)
    {   case KEY_UP:
            if(highlight == 1)
                highlight = n_choices;
            else
                --highlight;
            break;
        case KEY_DOWN:
            if(highlight == n_choices)
                highlight = 1;
            else 
                ++highlight;
            break;
        /* Case of user pressing enter */
        case 10:
            choice = highlight;
            break;
    }
    if(choice != 0) /* User chosen, come out of the infinite loop */
        break;
  }
  werase(menu_win);
  wrefresh(menu_win);
  delwin(menu_win);
  return choice;
}

/* ======================================================================================== */

static const int SLEEP_DELAY = 10;

using std::pair;
using std::vector;

struct graph
{
  graph() : c('#')
  {

  }
  graph(char c) : c(c)
  {

  }
  char c;
};

struct pack
{
  pair<int, int> begin, end;
  vector<vector<graph> > g;
};

/* Reads maze from FILE*, returns pack struct */
pack get_maze(FILE *f)
{
  pair<int, int> begin, end;
  int y = 1;
  vector<vector<graph> > g(2);
  char c;
  g[y].emplace_back();
  while((c = fgetc(f)) != EOF)
  {
    if(c == '\n')
    {
      g.emplace_back();
      g[y].emplace_back();
      y ++;
      g[y].emplace_back();
      continue;
    }
    g[y].emplace_back(c);
  }
  g.emplace_back();
  for(size_t i = 0; i < g.size(); ++ i)
    for(size_t j = 0; j < g[i].size(); ++ j)
      if(g[i][j].c == 'S')
        begin = std::make_pair(i, j);
      else if(g[i][j].c == 'E')
        end = std::make_pair(i, j);
  return {begin, end, g};
}

/** @brief Draws a maze given in argument, clears the whole screen when called.
 *  @param graph Graph to be drawn.
 *  @param visited Visited nodes in maze, these are colored with blue when drawn.
 *  @param begin Coords where the search starts, marked with S when drawn.
 *  @param end Coords where the the search ends, marked with E when drawn.
 *  @param nodes_expanded Number of nodes expanded so far.
 *  @param path_length length of the path from S to E.
 *  @param backtrack Vector of the path from S to E, these are colored yellow when drawn.
 */ 
void draw_maze(const vector<vector<graph> > & graph, 
               const vector<vector<bool> > & visited, 
               pair<int, int> begin,
               pair<int, int> end,
               size_t nodes_expanded,
               size_t path_length = 0,
               const vector<vector<bool> > & backtrack  = {} )
{
  clear();

  for(int j = 1; j < (ssize_t)graph.size() - 1; ++ j)
    for(int i = 1; i < (ssize_t)graph[j].size() - 1; ++ i)
    {
      pair coords = std::make_pair(j, i);
      if(coords == begin)
        mvaddch(j - 1, i - 1, 'S');
      else if(coords == end)
        mvaddch(j - 1, i - 1, 'E');
      else if(backtrack.size() && backtrack[j][i])
      {
        attron(COLOR_PAIR(2));
        mvaddch(j - 1, i - 1, ' ');
        attroff(COLOR_PAIR(2));        
      }
      else if(visited[j][i])
      {
        attron(COLOR_PAIR(1));
        mvaddch(j - 1, i - 1, ' ');
        attroff(COLOR_PAIR(1));        
      }
      else
        mvaddch(j - 1, i - 1, graph[j][i].c);
    }
  mvprintw(graph.size() + 2, 2, "nodes expanded: %llu", nodes_expanded);
  mvprintw(graph.size() + 3, 2, "path length: %llu", path_length);
  refresh();
}

std::vector<pair<int, int> > get_neigh(int x, int y, const vector<vector<graph> > & g)
{
  std::vector<pair<int, int> > res;
  if(g[x + 1][y].c != '#')
      res.emplace_back(std::make_pair(x + 1, y));
  if(g[x - 1][y].c != '#')
      res.emplace_back(std::make_pair(x - 1, y));
  if(g[x][y + 1].c != '#')
      res.emplace_back(std::make_pair(x, y + 1));
  if(g[x][y - 1].c != '#')
      res.emplace_back(std::make_pair(x, y - 1));
  return res;
}

void sort_greedy(int x, int y, vector<pair<int, int> > & vec)
{
  std::sort(vec.begin(), vec.end());
  for(const auto & p : vec)
    if(p.first == x)
    {
      /* Handle y */
      std::sort(vec.begin(), vec.end(), [](std::pair<int, int> x, std::pair<int, int> y){return x.second < y.second;});
      if(vec.begin()->second > y)
        std::reverse(vec.begin(), vec.end());
      return;
    }
  if(vec.begin()->first > x)
    std::reverse(vec.begin(), vec.end());
}


/* Draws BFS or DFS to console screen, based on a given graph
   graph is a maze in following format. If greedy is true then
   neighbours are first expanded if they're the closest to end,
   works only for dfs. */
void draw_dfs(const vector<vector<graph> > & g, pair<int, int> begin, pair<int, int> end, bool greedy = false, bool bfs = false)
{ 
  vector<vector<bool> > visited(g.size(), std::vector<bool>(g[1].size(), false));
  /* Entered vector keeps track of nodes that we're actually entered,
     while the visited vector keeps nodes that we're added to deque */
  vector<vector<bool> > entered(g.size(), std::vector<bool>(g[1].size(), false));
  vector<vector<pair<int, int> > > backtrack(g.size(), std::vector<pair<int, int> >(g[1].size()));
  size_t nodes_expanded = 0;

  std::deque<pair<int, int> > pending;
  pending.emplace_back(begin.first, begin.second);
  visited[begin.first][begin.second] = true;
  pair<int, int> node;
  while(pending.size() && node != end)
  {
    /* If bfs we're using the deque as front */
    if(bfs)
    {
      node = pending.front();
      pending.pop_front();
    }
    /* Else we're using the deque as stack */
    else
    {
      node = pending.back();
      pending.pop_back();
    }
    entered[node.first][node.second] = true;
    nodes_expanded ++;
    auto neighbours = get_neigh(node.first, node.second, g);
    if(greedy)
      sort_greedy(end.first, end.second, neighbours);
    for(auto neigh : neighbours)
      if(!visited[neigh.first][neigh.second] && g[neigh.first][neigh.second].c != '#')
      {
        pending.emplace_back(neigh.first, neigh.second);
        visited[neigh.first][neigh.second] = true;
        backtrack[neigh.first][neigh.second] = std::make_pair(node.first, node.second);
      }
    draw_maze(g, entered, begin, end, nodes_expanded);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DELAY));
  }

  node = end;
  vector<vector<bool> > bc(g.size(), std::vector<bool>(g[1].size(), false));
  size_t path_length = 0;
  while(node != begin)
  {
    path_length ++;
    bc[node.first][node.second] = true;
    node = backtrack[node.first][node.second];
    draw_maze(g, entered, begin, end, nodes_expanded, path_length, bc);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DELAY));
  }
}

void init_ncurses(void)
{
  initscr();
  raw();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_BLUE);
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
}

void destroy_ncurses(void)
{
  endwin();
}

int main(void)
{
  init_ncurses();
  FILE *f = fopen("maze.txt", "r");
  if(!f)
  {
    destroy_ncurses();
    perror("Error when opening file");
    return EXIT_FAILURE;
  }
  vector<vector<graph> > g;
  auto p = get_maze(f);
  g = p.g;

  int sel;
  while((sel = render_menu("Select algorithm", {"DFS", "Greedy-DFS", "BFS", "Quit"})) != 4)
  {
    switch(sel)
    {
      case 1:
        draw_dfs(g, p.begin, p.end, false, false);
        break;
      case 2:
        draw_dfs(g, p.begin, p.end, true, false);
        break;
      case 3:
        draw_dfs(g, p.begin, p.end, false, true);
        break;
    }
    getch();
  }

  // draw_maze(g, vector<vector<bool>>(N, vector<bool>(N)));
  destroy_ncurses();
}
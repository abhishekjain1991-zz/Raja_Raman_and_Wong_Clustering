#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <stack>
#include <set>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <time.h>
#include <stdio.h>



#define d_constant 3
using namespace std;
class node_data
{
public:
    string data;
    unordered_map<string,int> node_data_array;
    vector<string> reversed_array;
    int visited;
    int topo_visited;
    node_data(string c):data(c)
    {
        visited=0;
        topo_visited=0;
    }
    
    unordered_set <string> clusters;
    vector <string> reverse_links;
};


extern unordered_map<string,node_data> links;
extern char Model_name[100];
extern vector<string> temp_r;
extern unordered_set<string> p_inputs;
extern unordered_set<string> p_outputs;
extern unordered_map<string,int> labels;
extern unordered_map<string,vector<string> > clusters;
extern unordered_set<string> f_cluster_helper;
extern vector<string> optimizer;
extern queue<string> f_clusters;
extern vector<string> cluster_nodes;
extern unordered_map<string,int>::iterator visited;
extern unordered_map<string,int>::iterator ittemp;
extern multimap<int, string, greater<int> > temp_store;
extern string file_name;
extern int s_constraint;
void read_file(string f_name);
void toposort(string node_name);
void toposort_util();
void visit(string og);
void update_1();
void show_cluster();
int calculate_max(int a,int b);
int calculate_labels(string str);
void make_labels();
void rt_and_ic();
void create_final_cluster_set();
void show_labels();
int max_label();
void create_json();

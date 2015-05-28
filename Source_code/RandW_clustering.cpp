
#include "RandW_clustering.h"

using namespace std;

unordered_map<string,node_data> links;
char Model_name[100];
vector<string> temp_r;
unordered_set<string> p_inputs;
unordered_set<string> p_outputs;
unordered_map<string,int> labels;
unordered_map<string,vector<string> > clusters;
unordered_set<string> f_cluster_helper;
vector<string> optimizer;
queue<string> f_clusters;
vector<string> cluster_nodes;
unordered_map<string,int>::iterator visited;
unordered_map<string,int>::iterator ittemp;
multimap<int, string, greater<int> > temp_store;

void read_file(string f_name)
{
    ifstream inFile (f_name.c_str());
    string line;
    string sub_line;
    string item;
    size_t found;
    bool f_slash=false;
    char op;
    vector<string> :: iterator t1;
    string l_op;
    string l_op1;
    while (getline (inFile, line))
      {
        istringstream linestream(line);
        if(f_slash)
        {
            if (op=='i')
                {
                f_slash=false;
                    goto ip;       
            }
            else
            {
                f_slash=false;
                goto po;    
            }
        }
        found = line.find(".model");
              if(found!=string::npos)
                {
                  getline(linestream,item,' ');
                  while(getline (linestream, item, ' '))
                  {
                      const char *sub = item.c_str();
                      strcat(Model_name,sub);
                      strcat(Model_name," ");
                  }
                  continue;
                }
              found = line.find(".end");
              if(found != string::npos)
                  continue;
              found = line.find(".inputs");
              if(found != string::npos)
              {     
                    getline(linestream,item,' ');
                    ip:
                     while (getline (linestream, item, ' '))
                        {
                        if(item!="\\")
                            {
                                links.insert(pair<string,node_data>(item,node_data(item)));
                                p_inputs.insert(item);    
                            }
                        else
                        {
                            f_slash=true;
                            op='i';
                        }
                        }
                     continue;
              }
              found = line.find(".outputs");
              if(found != string::npos)
              {
                     getline(linestream,item,' ');
                     po:
                     while (getline (linestream, item, ' '))
                        {
                         if(item!="\\")
                         {
                           links.insert(pair<string,node_data>(item,node_data(item)));
                           links.find(item)->second.visited=1;
                           p_outputs.insert(item);  
                         }
                         else
                         {
                             f_slash=true;
                             op='o';
                         }
                        }
                     continue;
              }
              found = line.find(".latch");
              if(found!=string::npos)
              {
                  getline(linestream,item,' ');
                  if(item==".latch"){
                  while(1)
                  {
                   getline(linestream,item,' ');
                   if(item!="")
                    {
                       break;
                   }
                  }
                  l_op=item;
                  if(links.find(item)==links.end())
                  {links.insert(pair<string,node_data>(item,node_data(item)));}
                           p_outputs.insert(item);
                           
                  getline(linestream,item,' ');
                  if(links.find(item)==links.end())
                  {links.insert(pair<string,node_data>(item,node_data(item)));}
                           p_inputs.insert(item);  
                           links.find(item)->second.node_data_array.insert(pair<string,int>(l_op,1));
                           links.find(l_op)->second.reverse_links.push_back(item);
                           links.find(l_op)->second.reversed_array.push_back(item);
                   	           
                  }
                  else
                  {
                      istringstream linestream2(line);
                      
                      getline(linestream2,item,'\t');
                      getline(linestream2,item,'\t');
                      l_op1=item;
                      if(links.find(item)==links.end())
                      {links.insert(pair<string,node_data>(item,node_data(item)));}
                           p_outputs.insert(item);
                           getline(linestream2,item,'\t');
                           if(links.find(item)==links.end())
                           {links.insert(pair<string,node_data>(item,node_data(item)));}
                           p_inputs.insert(item);
                           links.find(item)->second.node_data_array.insert(pair<string,int>(l_op1,1));
                           links.find(l_op1)->second.reverse_links.push_back(item);
                           
                           
                  }
                     
                  continue;
              }
              
              found=line.find(".names");
              if(found != string::npos)
              {
                  getline(linestream,item,' ');
                  while (getline (linestream, item, ' '))
                  {
                      if(links.find(item)==links.end())
                        {
                          links.insert(pair<string,node_data>(item,node_data(item)));
                        }
                      temp_r.push_back(item);
                  }
                  t1 = temp_r.begin();
                  while(t1!=(temp_r.end()-1))
                  {
                      links.find(*t1)->second.node_data_array.insert(pair<string,int>(temp_r[temp_r.size()-1],1));
                      links.find(temp_r[temp_r.size()-1])->second.reverse_links.push_back(*t1);
                      links.find(temp_r[temp_r.size()-1])->second.reversed_array.push_back(*t1);
                      t1++;
                  }
                  temp_r.clear();
                  continue;
              }
              //ignore a string that does not start with anything mentioned above     
      }    
}

void toposort(string node_name)
{
    links.find(node_name)->second.topo_visited=1;
    unordered_map<string, int>::iterator zora2= links.find(node_name)->second.node_data_array.begin();
    while(zora2!=links.find(node_name)->second.node_data_array.end())
    {
        if(links.find(zora2->first)->second.topo_visited==0)
            toposort(zora2->first);
        zora2++;
    }
    optimizer.push_back(node_name);
}

void toposort_util()
{
    unordered_map <string,node_data> :: iterator zora;
    zora=links.begin();
    while(zora!=links.end())
    {
        if(zora->second.topo_visited==0)
            toposort(zora->first);
        zora++;
    }
}

void visit(string og)
{
   links.find(og)->second.visited=1;
   ittemp=links.find(og)->second.node_data_array.begin();
        while(ittemp!=links.find(og)->second.node_data_array.end())
        { 
            if(links.find(og)->second.node_data_array.find(ittemp->first)->second!=1)
                {
                    //ittemp++;
                    //continue;
                    //can go with break to reduce time, need to check how insertion in unordered map works
                    break;
                    }
            
            visited=links.find(ittemp->first)->second.node_data_array.begin();
            while(visited!=links.find(ittemp->first)->second.node_data_array.end())
            {
                if(links.find(og)->second.node_data_array.find(visited->first)==links.find(og)->second.node_data_array.end())   
                { 
                    links.find(og)->second.node_data_array.insert(pair<string,int>(visited->first,1+visited->second));
                   links.find(visited->first)->second.reversed_array.push_back(og);  
                }
                else if(links.find(og)->second.node_data_array.find(visited->first)->second>=1+(visited->second))
                {
                    break;
                }
                else if(links.find(og)->second.node_data_array.find(visited->first)->second<1+(visited->second))
                { 
                    links.find(og)->second.node_data_array.find(visited->first)->second=1+visited->second;
                }
                    visited++;
            }
            ittemp++;
        }           
}

void update_1()
{
    vector<string>::iterator rit = optimizer.begin();
    while(rit!=optimizer.end())
    {
        if(links.find(*rit)->second.visited==0 )
            visit(*rit);
        rit++;
    }
}
void show_cluster()
{
    cout<<"clusters for now are "<<endl;
    vector<string> :: iterator it = cluster_nodes.begin();
    unordered_set <string> :: iterator it2;
    while(it!=cluster_nodes.end())
    {
       cout<<*it<<" ";
       it2=links.find(*it)->second.clusters.begin();
       while(it2!=links.find(*it)->second.clusters.end())
       {
           cout<<*it2<<" ";
           it2++;
       }
       cout<<endl;
       it++;  
    }
 
}

int calculate_max(int a,int b)
{
    return (a>b)? a : b;
}


int calculate_labels(string str)
{
    
    vector<string>::iterator it  = links.find(str)->second.reversed_array.begin();
    int temp_sum=0;
    int l1 = 0;
    int l2 = 0;
    int found_l1 = 0;
    int found_l2 = 0;
    while(it !=  links.find(str)->second.reversed_array.end())
    {
        
        temp_sum = (labels.find(*it)->second)+(links.find(*it)->second.node_data_array.find(str)->second);
        temp_store.insert(pair<int ,string >(temp_sum,*it));
        it++;
    }
    multimap<int, string, greater<int> >::iterator it2 = temp_store.begin();
    if((temp_store.size()+1) <= s_constraint)
    {
        while(it2!=temp_store.end())
        {

            if(l1=it2->first)
                {
                   l1 = it2->first;

                }
            links.find(str)->second.clusters.insert(it2->second);
            it2++;
        }
        temp_store.clear();
        return l1;  
    }
    else
    {
        int count = 0;
        while(count<(s_constraint-1))
        {
            
            if(l1<it2->first)
                {
                   l1 = it2->first;

                }
            links.find(str)->second.clusters.insert(it2->second);
            it2++;
            count++;
        }
        l2=d_constant+(it2->first);
        int final = max(l1,l2);
        temp_store.clear();
        return final;
    }
    

}


void make_labels()
{
    unordered_set <string> :: iterator is = p_inputs.begin();
    vector<string> temp;
    while(is != p_inputs.end())
    {
        labels.insert(pair<string,int>(*is,1)); 
        is++;
    }
    vector<string>::reverse_iterator rit = optimizer.rbegin();
    int l;
    while(rit!=optimizer.rend())
    {
        if(labels.find(*rit)==labels.end())
        {
            l=calculate_labels(*rit);
            labels.insert(pair<string,int>(*rit,l));
        }
        rit++;
    }
}


void rt_and_ic()
{
    string top=f_clusters.front();
    f_clusters.pop();
    unordered_set<string>::iterator iz = links.find(top)->second.clusters.begin();
    vector<string>::iterator iz_temp;
   
    while(iz!=links.find(top)->second.clusters.end())
    {
        iz_temp=links.find(*iz)->second.reverse_links.begin();
        while(iz_temp!=links.find(*iz)->second.reverse_links.end())
        {
            if(f_cluster_helper.find(*iz_temp)==f_cluster_helper.end() && links.find(top)->second.clusters.find(*iz_temp)==links.find(top)->second.clusters.end())
            {
                f_clusters.push(*iz_temp);
                f_cluster_helper.insert(*iz_temp);   
            }
            iz_temp++;
        }
        iz++;
    }
    cluster_nodes.push_back(top);
}
void create_final_cluster_set()
{
    unordered_set<string> :: iterator it1 = p_outputs.begin();
    while(it1!=p_outputs.end())
    {
        f_clusters.push(*it1);
        f_cluster_helper.insert(*it1);
        it1++;
    }
    while(f_clusters.size())
    {
        rt_and_ic();
    }   
}

void show_labels()
{
    cout<<"labels are as follows "<<endl;
    unordered_map<string,int> :: iterator l_it= labels.begin();
    while(l_it!=labels.end())
    {
        cout<<l_it->first<<" "<<l_it->second<<endl;
        l_it++;
    }
}

int max_label()
{
    unordered_map<string,int>::iterator labels_it = labels.begin();
    int max=0;
    while(labels_it!=labels.end())
    {
        if(max<labels_it->second)
            max=labels_it->second;
        labels_it++;
    }
    return max;
}

void create_json()
{
    ofstream jsonf(file_name+".json");
    jsonf<<"{\n";
    jsonf<<"\"name\":\"data\",\n";
    jsonf<<"\"children\":[\n";
    jsonf<<"{\n";
    jsonf<<"\"name\":\"RAJA RAMAN AND WONG CLUSTERING\",\n";
    jsonf<<"\"children\":[\n";
    //all nodes
    jsonf<<"{\n";
    jsonf<<"\"name\":\"NODES\",\n";
    jsonf<<"\"children\":[\n";
    unordered_map<string,node_data>::iterator it=links.begin();
    while(it!=links.end())
    {
          jsonf<<"{\"name\": \""<<it->first<<"\", \"size\": 4614"; 
          it++;
          if(it!=links.end())
              jsonf<<"},\n";
          else
              jsonf<<"}\n";
    }
    jsonf<<"]\n";
    jsonf<<"},\n";
    //all labels
    jsonf<<"{\n";
    jsonf<<"\"name\":\"LABELS\",\n";
    jsonf<<"\"children\":[\n";
    unordered_map<string,int>::iterator it2=labels.begin();
    while(it2!=labels.end())
    {
          jsonf<<"{\"name\": \" Node : "<<it2->first<<" Label: "<<it2->second<<"\", \"size\": 4614"; 
          it2++;
          if(it2!=labels.end())
              jsonf<<"},\n";
          else
              jsonf<<"}\n";
    }
    jsonf<<"]\n";
    jsonf<<"},\n";
    jsonf<<"{\n";
    jsonf<<"\"name\":\"CLUSTERS\",\n";
    jsonf<<"\"children\":[\n";
    vector<string>::iterator it3=cluster_nodes.begin();
    while(it3!=cluster_nodes.end())
    {
          jsonf<<"{\n\"name\": \"Cluster "<<*it3<<"\",\n";
          jsonf<<"\"children\":[\n";
          unordered_set <string> :: iterator it4 = links.find(*it3)->second.clusters.begin();
          while(it4!=links.find(*it3)->second.clusters.end())
          {
            jsonf<<"{\"name\": \""<<*it4<<"\", \"size\": 2614"; 
            it4++;
            if(it4!=links.find(*it3)->second.clusters.end())
                  jsonf<<"},\n";
            else
                  jsonf<<"}\n";
           }
          jsonf<<"]\n";
          it3++;
          if(it3!=cluster_nodes.end())
              jsonf<<"},\n";
          else
              jsonf<<"}\n";
    }
    jsonf<<"]\n";
    jsonf<<"}\n";
    jsonf<<"]\n";
    jsonf<<"}\n";
    jsonf<<"]\n";
    jsonf<<"}";
}


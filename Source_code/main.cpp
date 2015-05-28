/* 
 * File:   main.cpp
 * Author: abhishekjain91
 *
 * Created on March 17, 2015, 4:20 PM
 */


#include "RandW_clustering.h"

using namespace std;


int s_constraint=4;
string file_name;
int main (int argc, char* argv[])
{
    
    if(argc<2)
	{	cout<<"Sorry fil_name arguement mising"<<endl;
		cout<<"please enter file name followed by cluster size (optional) as command line arguments next time. "<<endl;
		exit(0);	
	}
    file_name=string(argv[1]);
    if(argc>2)
    s_constraint=atoi(argv[2]);
    clock_t t;
    t = clock();
    read_file(file_name+".blif");
    cout<<"Model name is "<<Model_name<<endl;

    toposort_util();

    cout<<"done toposort and file reading"<<endl;

    update_1();

    cout<<"done matrix"<<endl;

    make_labels();

    cout<<"done labeling"<<endl;

    create_final_cluster_set();
    t = clock() - t;
    cout<<"done clustering"<<endl;
    //show_cluster();
    printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
    cout<<"number of individual nodes is "<<links.size()<<endl;
    cout<<"number of individual clusters is "<<cluster_nodes.size()<<endl;
    cout<<"cluster size used is "<<s_constraint<<endl;
    cout<<"max label size is  "<<max_label()<<endl;
    create_json();
    return 0;
 }




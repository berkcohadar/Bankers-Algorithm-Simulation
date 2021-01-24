#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
vector<pair<int, int> > entries;
vector<int> order,allocation,remainings;
vector<bool> finish;

void isValid(int process, int resources,int free){
    if (resources <= free && order[0] == process && entries[process].second + resources <= entries[process].first) cout<<"Request CAN be granted. ";
    else cout<<"Request CANNOT be granted. ";
}

void execute(){
    cout<<"A correct process execution order is ";
    for(int i = 0; i < order.size(); i++){
        if (i+1==order.size()) cout<<order[i]<<". ";
        else cout<<order[i]<<"->";
    }
}

void request(int free){
    int proc,res;
    cout<<"Which process requests the resource?"<<endl;
    cin>>proc;
    if (proc>entries.size()-1) {
        cout<<"Unvalid Process"<<endl;
        return;
    }
    cout<<"How many resources does process "<<proc<<" request?"<<endl;
    cin>>res;
    isValid(proc,res,free);
}

bool safeState(int free,int changed){
    for(int i = 0; i < remainings.size(); i++){
        if (finish[i] == 0 && remainings[i]<=free){ // free=2 ,4-1=3 GIRMEDI, 6-4=2, 2<=free o zaman girersin
            finish[i] = 1; // finishi true yaptim
            free+= allocation[i]; // yaptigi processi freeye ekledim, yaptim saldim
            changed++; // kac process bitti
            order.push_back(i); // processin calisma sirasi, B->A->C , 1->0->2
            return safeState(free,changed); // Recursion
        }
    }
    if (changed == finish.size()) return true;
    return false;
}

int main(int argc, char* argv[]){
    ifstream infile(argv[1]);
    if (!infile) cout << "File is not valid." << endl;
    string line;
    int resources,allocated=0;
    
    while (getline(infile, line) ) {
            istringstream iss(line);
            int a, b;
            if (!(iss >> a >> b)) {resources=a; continue;}
            allocated+=b;
            entries.push_back(make_pair(a, b));
            allocation.push_back(b);
            remainings.push_back(a-b);
    }
    for(int i = 0; i < entries.size(); i++) finish.push_back(false); // [false,false,false]
    int free = resources - allocated;
    string input;
    if (safeState(free,0)) cout<<"Input state is SAFE. ";
    else {
        cout<<"Input state is UNSAFE."<<endl;
        return 0;
    }
    while (true){
        cout<<"Enter E to execute, R to request and Q to terminate."<<endl;
        cin>>input;
        if (input =="E" || input =="e") execute();
        else if (input =="R" || input =="r") request(free);
        else if (input =="Q" || input =="q") break;
        else cout<<"Unknown Command."<<endl;
    }
	return 0;
}

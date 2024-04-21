#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
using namespace std;

string executeCommand(const string& command) {
    string output;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cerr << "errore" << endl;
        return output;
    }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output+=buffer;
    }
    pclose(pipe);
    return output;
}

vector<string> getTopProcesses(int n) {
    string command = "ps -eo pid,%mem,comm --sort=-%mem | head -n "+to_string(n+1)+" | tail -n " + to_string(n);
    string output = executeCommand(command);
    istringstream iss(output);
    vector<string> topProcesses;
    string line;
    getline(iss,line);
    while (getline(iss,line)){
        topProcesses.push_back(line);
    }
    return topProcesses;
}

void displayProcessInfo(const vector<string>& processes){
    if (processes.empty()) {
        cout << "No processes found." <<endl;
        return;
    }

    cout<< "Top 5 memory-consuming processes:"<<endl;
    for (size_t i=0; i<processes.size(); ++i) {
        cout<<i+1<<". "<< processes[i] <<endl;
    }
}

void killProcess(const string& pid) {
    string killCommand = "kill -9 " + pid;
    system(killCommand.c_str());
}

int main() {
    vector<string> topProcesses = getTopProcesses(5);
    displayProcessInfo(topProcesses);

    cout<< "Numero del processo da killare (0 per uscire): ";
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= static_cast<int>(topProcesses.size())) {
        stringstream ss(topProcesses[choice - 1]);
        string pid;
        ss >> pid;
        killProcess(pid);
        cout << "Processo killato." <<endl;
    } else {
        cout << "Nessun processo killato." <<endl;
    }

    return 0;
}

#include "mapper.h"
using namespace std;
//gets the name of the wifi card (will always pick the first one (0 then 1 then 2...))
string getWifiCard(){
    string command = "ifconfig ";
    string tmpflname = "DeleteMe.txt";
    command += " > " + tmpflname;
    system(command.c_str()); // cmd > Deleteme.txt
    string out;
    string temp;
    ifstream fin(tmpflname);
    fin >> temp;
    while(fin >> temp && temp.substr(0,4) != "wlan");
    command = "rm -f " + tmpflname;
    system(command.c_str());
    return temp.substr(0, temp.length() - 1); // Removes the :
}//DT

// initializes (std::map<string,string> devices)
void makeKnownMap(){
    ifstream known(resc + "known.txt");
    while(known){
        string tempName;
        string fullName;
        known >> tempName;
        if (!known) break;
        if (tempName.substr(tempName.size() - 2) == "n="){
            known >> tempName;
            while(tempName.substr(tempName.size() - 2) != "m="){
                fullName += tempName + " ";
                known >>tempName;
            }
            string tempMac;
            known >> tempMac;
            known_devices[tempMac] = fullName;
        }
    }
}//DT

//Requires "/" to be at the end of relative file location
void generateConfigFile(const string & relative_file_location ){
    ofstream optionsFile(relative_file_location + "mapper.conf");
    optionsFile << "#\n#\nSOF\n";
    optionsFile << "";
}

map<string, string> options;

//Requires "/" for  relative file location
bool initOptions(){
    ifstream input(program_root + "mapper.conf");
    return false;
}



int main (int argc, char ** argv){
    //Resc location
    try{
        initOptions();
    }
    catch (...) {
        generateConfigFile("../");
    }


    //Wifi card





    return 0;
}
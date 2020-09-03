 
#ifndef P1_LIBRARY_H
#define P1_LIBRARY_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cctype>
#include <fstream>
#include <cassert> 
#include <algorithm>
#include <vector>
#include <map>
using namespace std;


//    const int a = 1;
//    const int b = 2;
//    int const   *  p = &a; // datatype * p = &a; datatype is const int: "int const" == "const int"
//    p = &b; // Ok
//    *p = 4; // Not Ok






//Helper functions -----------------------------------------------------------------------------------------------------



//Returns the input string in upper case
string toUpperCase(const string & input){
    string new_input = input;
    transform(new_input.begin(), new_input.end(), new_input.begin(), ::toupper);
    return new_input;
}//D

//Returns the input string in lower case
string toLowerCase(const string & input){
    string new_input = input;
    transform(new_input.begin(), new_input.end(), new_input.begin(), ::tolower);
    return new_input;
}//D

//runs a valid command with sudo before it
void sudo(const string & command){
    string out;
    //out  =("echo ");
    //out += password;
    //out += (" | ");
    out += ("sudo ");
    //out += (" -S ");
    out += command;
    system(out.c_str());
}//DT

//Returns if a command is in the format "%whitespace sudo"
bool isSudo(const string  & command){
    size_t size = command.length();
    for(size_t i = 0; i < size; ++i){

        if(command[i] == 's'){
            string comp = command.substr(i, 4);
            return (comp == "sudo");
        }
        if(command[i] != ' ' && command[i] != '\n') return false;
    }
    return false;
}//DT

//Gets the output of a command, needs the command to be valid
string getCommandOutput(string command){
    string tmpflname = "DeleteMe.txt";
    command += " > " + tmpflname;

    system(command.c_str()); // %cmd > Deleteme.txt

    string out;
    string temp;
    ifstream fin(tmpflname);
    for(int i = 0; fin >> temp; ++i) out += temp + " ";
    command = "rm -f " + tmpflname;
    system(command.c_str());
    return out;
}//DT

//Writes a command to an output file
void writeCommandOutputToFile(const string  & fileName, string command){
    command += " > " + fileName;
    system(command.c_str()); // cmd > fileName
}//DT






//Returns whether or not the mac address inputted is a mac address with colons
static bool isMacAddressWithColons(const string & in){
    string new_in = toUpperCase(in);
    if(int(new_in.length()) != 17) return false;
    for(int i = 0; i < int(new_in.length()); i++){
        char temp = new_in[i];
        if( (i-2)%3 == 0 && temp != ':'&& temp != ' ' )return false;
    }
    return true;
}//D

//Strips colons from a valid mac address with colons.
static string stripColons(const string & mac){

    /*9A:65:33:17:49:EE
      012           2
      9A65:33:17:49:EE
      01234         4
      9A6533:17:49:EE
      0123456       6
      9A653317:49:EE
      012345678     8
      9A65331749:EE
      01234567890   10 */

    string newMac = toUpperCase(mac);
    if(isMacAddressWithColons(newMac)){
        for(int i = 2; i < 11; i+=2){
            newMac = newMac.substr(0, i) + newMac.substr(i + 1);
        }
    }
    return newMac;
}//DT


//Gets vendor of a mac address in colon or plain format
string getVendor(const string & address){
    //Only checks the first 6 char
    string cmd = "grep " + stripColons(address).substr(0,6) + " /usr/share/nmap/nmap-mac-prefixes";

    string out;
    out = getCommandOutput(cmd);
    //74AC5F Qiku Internet Network Scientific (Shenzhen)
    //01234567
    if(out.length() > 0) return out.substr(7);
    return "No Vendor could be found!";
}//DT

//GLOBALS --------------------------------------------------------------------------------------------------------------
// Location of resc folder
const string program_root = "~/";
const string resc = program_root + "resc/";
map<string, string> known_devices; // known_devices[Mac] = Name

















#endif
#include "mapper.h"

using namespace std;


void setup(){
    system("clear");
    user = getCommandOutput("whoami");
    make_known_map();// initializes (std::map<string,string> devices)
}

void cleanup(){
    sudo("rm -f " +resc + "/stored/out* ");
    string wcard = getWifiCard();
    if (wcard.substr(wcard.size() - 3) == "mon"){
        sudo("airmon-ng stop " + getWifiCard());
    } else{
        cout << "Card aleready not in mode";
    }
}


//Gets password from the user
void update_password(){
    cout<<"ENTER SUDO PASSWORD FOR ACCOUNT \"" << user <<"\":";
    cin >> password;
    system("clear");
}

int finish(const int  & ercode){
    cleanup();
    return ercode;
}
void airodump_read(map<string, int > & scanned_devices, ifstream & outfile){
    while(outfile){
        string temp;
        string trash;
        outfile >> temp; // Station MAC,

        // First time seen,
        outfile >> trash; // date
        outfile >> trash; // time

        // Last time seen
        outfile >> trash; // date
        outfile >> trash; // time

        outfile >> trash; //-Power,

        //Takes the - sign and the comma out of trash and puts it into scanned_devices:
        if (!temp.empty() && !trash.empty()){
            scanned_devices[temp.substr(0, temp.size() - 1)] =
                    stoi(trash.substr(1, trash.length() -1) );
        }

        //Continues the ifstream until a new line is present.
        getline(outfile, trash);
    }

}
bool check_runtime(){
    string unit = runTime.substr(runTime.length()-1);
    if ( !(unit == "m" || unit == "s" || unit == "h" )) return false;
    for (size_t i = 0; i < runTime.length() - 1; i++)
        if (isdigit(runTime[i]) == 0)
            return false;
    return true;
}


int main(int argc, char** argv){
    //SETUP :: \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\;

    setup();



    //START USER INPUT: ------------------------------------------------------------------------------------------------
    if (argc >1) {
        password = *(argv + 1); //can take password as argument
        if (argc == 3) {
            runTime = toLowerCase(string (argv[2]) ); // sets second argument to be runtime
            if (!check_runtime()) runTime = "60s";
        }
    } else{
        update_password();
    }
    sudo("airmon-ng start " + getWifiCard() );

    sudo("timeout "+ runTime + " airodump-ng -b a -I 10 -w " + resc + "/stored/out --berlin 60  " + getWifiCard());
    // program              5ghz band, 1 second interface times, out-01.csv, 30 seconds before station is removed,

    system("clear");

    //Make sure this runs and only runs after &runTime  has passed
    ifstream outfile(resc + "/stored/out-01.csv");

    string trash;
    string tempMac;
    string power;

    while(outfile && trash != "ESSIDs"){
        outfile >> trash;
    }
    if (!outfile){
        return finish(3);
    }

    //outfile should now read stations
    map<string, int> scanned_devices; //mac address, power
    airodump_read(scanned_devices, outfile);

    ofstream output(resc + "/output.txt");
    system("clear");
    cout << "Finished scanning stations, now printing:" << endl;
    int i = 0;
        vector<pair<string , int>> known_scanned;
    for(const auto& item : scanned_devices){
        cout << i << "m= " << item.first << endl;
        cout << i << "p= " << item.second << endl;
        cout << endl;
        ++i;
        if (!known_devices[item.first].empty()){
            known_scanned.emplace_back(item);
        }
    }
    cout << endl;
    cout << "Known devices present:" << endl;
    if (!known_scanned.empty()){
        for(const auto& item : known_scanned){
            cout << "Name: ";
            string tname =  known_devices[item.first];
            cout << tname << endl;
            cout << "Power: ";
            int tpower = item.second;
            cout << tpower << endl << endl;

            output << "Name: " << tname << endl << "Power: " << tpower << endl << endl;
        }
    }
    else{
        cout << "None" << endl;
        output << "None" << endl;
    }


    return finish(0);
}



#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct _Config{
    string fullname;
    string favouritefruit;
    bool needspeeling;
    bool seedsremoved;
    vector<string> otherfamily;
};
typedef struct _Config Config;

void readConf(Config& conf, string str){
    string::size_type begin = str.find_first_not_of(" \f\t\v");
    size_t pos;
    if (begin == string::npos) return;
    if(str[begin] == '#' || str[begin] == ';') return;

    string option = str.substr(begin, str.find(" "));
    transform(option.begin(), option.end(), option.begin(), ::tolower);
    string value = str.substr(str.find(" ") + 1, str.length());

    if (option == "fullname"){
        conf.fullname = value;
    } else if (option == "favouritefruit"){
        conf.favouritefruit = value;
    } else if (option == "needspeeling") {
        conf.needspeeling = true;
    } else if(option == "seedsremoved") {
        conf.seedsremoved = true;
    } else if (option == "otherfamily") {
        while((pos = value.find(",")) != string::npos){
            string v = value.substr(0, pos);
            conf.otherfamily.push_back(v.substr(v.find(" ") + 1, v.length()));
            value.erase(0, pos + 1);
        }
        conf.otherfamily.push_back(value.substr(value.find(" ") + 1, value.length()));
    }
}

void parseFile(ifstream& in, Config& conf) {
    conf.needspeeling = false;
    conf.seedsremoved = false;
    string str;

    while(!in.eof()){
        while (getline(in, str)){
            readConf(conf, str);
        }
    }
}

void displayConf(Config& config){
    cout << "Full Name: " << config.fullname << endl;
    cout << "Favorite Fruit: " << config.favouritefruit << endl;
    cout << "Needs Peeling: " << (config.needspeeling ? "True" : "False") << endl;
    cout << "Seeds Removed: " << (config.seedsremoved ? "True" : "False") << endl;
    cout << "Other Families: " << config.otherfamily[0] << ", " << config.otherfamily[1] << endl;
}

int main(int argc, char* argv[]){
    ifstream fs;
    Config conf;
    fs.open("conf.txt");
    parseFile(fs, conf);
    displayConf(conf);
    fs.close();
    return 0;
}

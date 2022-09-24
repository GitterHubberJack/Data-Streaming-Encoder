
#include "StructGenerator.h"
//
#include <fstream>
#include <iostream>
#include <stdlib.h> 
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <sstream>  
#include <vector>

using namespace std;

size_t
extractvalue (string& str, string pattern ) {
    size_t index = str.find(pattern);
    string str2 = str.substr(index + pattern.size());
    return atoi(str2.c_str()); 
}

vector <string>
tokenize(string s, char del) {
    vector<string> vect;
    stringstream ss(s);
    string word;
    while (!ss.eof()) {
        getline(ss, word, del);
        vect.push_back(word);
    }
    return vect;
}


StructGenerator::StructGenerator(string& fileName) {
    _fileName    = fileName;
    _outFileName = "pkg_msg.sv";
    _maxSize = 0;
    _maxNumFields = 0;
}

void
StructGenerator::parseSpecFile( ) {
    ifstream ifile;
    ifile.open(_fileName.c_str(), ios::in);
    if (!ifile.is_open()) {
        cout << "Could not open file <" << _fileName << "> . Please verify that the file exists and that you have the permission to read it." << endl;
    }
    const char space = ' ';
    string line = "";
    string msg_name = "";
    size_t index = 0;

    while (getline(ifile, line) ) {
        // Retrieving msg category
        if(!line.empty() && line.at(0) != space) {
            line.erase(remove(line.begin(),line.end(), ' '), line.end());
            line.erase(remove(line.begin(),line.end(), ':'), line.end());
            msg_name = line;
            boost::to_upper(msg_name);
            // retrieving msg type
            if(getline(ifile, line) ) {
                if (line.size() >= 13) {  
                    size_t value = extractvalue(line, "msg_type:");
                    if (value > _maxMsgType) {
                        _maxMsgType = value;
                    }
                    _supportedMsgTypes.push_back(make_pair(msg_name, value)); 
                }
 
                while (getline(ifile, line) ) {
                    if (line.find("fields") != string::npos) {
                        continue;
                    }
                    if(line.empty()) {
                        break;
                    } else {
                        line = line.substr(6);
                        vector<string> vect;
                        vect = tokenize(line, ':');
                        if (vect.size() != 4) {
                           cout << "An error occured druing parsing. \nLine: " << line << endl;
                        } else {
                            string name = vect[0];
                            name.erase(remove(name.begin(), name.end(), ' '), name.end());
                            //
                            vector<string> vect2;
                            vect2 = tokenize(vect[2], ',');
                            size_t size = atoi(vect2[0].c_str()); 
                            size_t cycle = atoi(vect[3].c_str());
                           _fieldsMap[msg_name].push_back(name);
                           _valuesMap[msg_name + name] = make_pair(size, cycle);
                            _groups[msg_name][cycle][name] = cycle;
                            if (size > _maxSize) {
                                _maxSize = size;
                            }
                            //
                        } 

                                
                    size_t value = extractvalue(line, "size:\"");
                        
                    }

                }

            }
                
        }
    }
    ifile.close();
}

void
StructGenerator::generateStruct() {
    ofstream ofile;
    stringstream o;
    string filename = _outDir + "pkg_msg.sv";
    ofile.open(filename.c_str(), ios::app);
    if (!ofile.is_open()) {
        cout << "Could not open file <" << _fileName << "> . Please verify that you have the permission to write in it." << endl;
    }
    o << "package st_encoder_pkg;" << endl;
    o << "    typedef logic[7:0] msg_type_t;" << endl;
    o << endl;
    o << "    typedef enum logic[7:0] {" << endl; 
    bool first = true;
    for (vector<pair <std::string, size_t> >::iterator it = _supportedMsgTypes.begin(); it != _supportedMsgTypes.end(); it++) {
        if (first) {
             first = false;
        } else {
            o << ", " << endl;
        }
        o << "        " << (*it).first << " = " << (*it).second ;
    }
    o << endl;
    o << "    } msg_type_supported_t;" << endl;
    o << endl;
    o << endl;
    for (map <string, vector<string > >::iterator it1 = _fieldsMap.begin(); it1 != _fieldsMap.end(); it1++) {
        string msgType = it1->first;
        if(it1->second.size() > _maxNumFields) {
            _maxNumFields = it1->second.size();
        }
        o << " // New Struct: " << msgType << endl;
        o << "    typedef struct {" << endl;
        for (vector<string>::iterator it2= it1->second.begin(); it2 != it1->second.end(); it2++) {
            string field = *it2;
            size_t size = _valuesMap[msgType + field].first;
            o << "        logic " << field << "_avail;" << endl;
            o << "        logic[" << size - 1 << ":0] [7:0] " << field << ";" << endl;
        }
        o << "    } " << it1->first <<"_fields" << ";" <<  endl;
        o << endl;
    }
    o << endl;
    o << "endpackage" << endl;
    ofile << o.str() << endl;
    ofile.close();
}

void
StructGenerator::generate() {
    parseSpecFile();
    generateStruct();
}

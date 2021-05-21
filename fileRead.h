/**
*Copyright 2021 Boyd Kirkman
*
*This file is part of BlockBoard.
*
*    BlockBoard is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    BlockBoard is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with BlockBoard.  If not, see <https://www.gnu.org/licenses/>.
*/

/** THIS HEADER FILE IS USED FOR READING FILES SUCH AS THE IP ADDRESS FILE, LIST OF MINER IP'S, AND BLOCKCHAIN DATA THEN RETURN THE INFO IN A PROCESSABLE FORM. IT CONTAINS THE FOLLOWING FUNCTIONS:
*
* nodeIPs() --> This will return an string array of all IPv4 addresses listed in NetworkAddressList.txt.
*
* writeIP( IP(string) ) --> This will add the input IP (IPv4) address onto the end of NetworkAddressList.txt.
*
* inIPList( IP(string) ) --> This will return true if the input ip is in NetworkAddressList.txt, else it will return false.
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#ifndef FILEREAD_H
#define FILEREAD_H

namespace fileRead
{

//--------------------------------------------------------------Return a list of the known IPv4 addresses in the network~~~~~~~~~~~~~~~~~~~~~~~~~~~<<

vector<string> nodeIPs()
{
    vector<string> IPArray;
    string line;
    ifstream IPAddressList ("NetworkAddressList.txt");
    if (IPAddressList.is_open())
    {
        while (getline(IPAddressList,line))
        {
            IPArray.push_back(line);
        }
    }
    return IPArray;
}

//--------------------------------------------------------------Add an IPv4 address to the list of the known addresses in the network~~~~~~~~~~~~~~~~~~~~~~~~~~~<<

void writeIP(string IP)
{
    ofstream IPFile ("NetworkAddressList.txt");
    if (IPFile.is_open())
    {
        IPFile << IP << "\n";
        IPFile.close();
    }
}

//--------------------------------------------------------------Check if an ip is in the list of knwon ips~~~~~~~~~~~~~~~~~~~~~~~~~~~<<
boolean inIPList(string IP)
{
    boolean inFile = false;
    vector<string> IPArray;
    string line;
    ifstream IPAddressList ("NetworkAddressList.txt");
    if (IPAddressList.is_open())
    {
        while (getline(IPAddressList,line))
        {
            if (IP == line)
            {
                inFile = true;
            }
        }
    }
    return inFile;
}

} //namespace fileRead

#endif // FILEREAD_H

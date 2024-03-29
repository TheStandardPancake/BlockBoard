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

/** THIS HEADER FILE IS USED FOR CREATION AND STORAGE OF INDIVIDUAL BLOCK DATA AND PROVIDES THE FOLLOWING FUNCTIONS
* CreateHeader( previousHash(string), timeStamp(int), nonce(int), merkleTreeHash(string) ) --> This will store all the header data for an instance of the class (AKA each block)
* addPost( postData(string) ) --> This will take the post data as a string and store it in the block
* addTransactions( transactionList(int[11][4]) ) --> This will take a list of transaction data and store it in the block
* MerkleHash( post(string), transactions(int[])) --> This will hash all the post data and transaction data together and return a SHA256 string of the hashed result.
*/

#include "picosha2.h"
#include <string>

#ifndef BLOCK_H //include guard
#define BLOCK_H

namespace block
{
class Block
{
public:
    //____________________________The Header____________________________

    std::string previousHash;
    int timeStamp;
    int nonce;
    std::string merkleTreeHash;

    void CreateHeader(std::string pH, int tS, int n, std::string mTH)
    {
        previousHash = pH;
        timeStamp = tS;
        nonce = n;
        merkleTreeHash = mTH;
    }

    //_____________________________The Body_____________________________

    std::string postData; // A string to store the post data

    int transactionData[11][4]; //each transaction will be an array with {address from, amount transfered, address to, hashed transaction data by address from}

    void addPost(std::string input)
    {
        postData = input; // Store the input into the local post data variable
    }


    void addTransactions(int transactionList[11][4])
    {
        int arraySize = 11 * 4;
        std::copy(transactionList, transactionList + arraySize, transactionData); // store the input transactions into the local transactions variable
    }

    //--------------------------------------------------------------Hashing~~~~~~~~~~~~~~~~~~~~~~~~~~~<<

    std::string MerkleHash(std::string post, int transactions[])
    {
        std::string initialHashes[12];
        std::string secondHashBatch[6];
        std::string thirdHashBatch[3];
        std::string finalHash;

        //HASH 1: hash all of them using SHA256
        //Add Initial
        initialHashes[0] = picosha2::hash256_hex_string(post);

        for (int i = 1; i < 12; i++)
        {
            int inidvidualTransaction[4]; //create a variable to temporarily store each individual transaction
            std::copy(transactions[i], transactions[i] + 4, inidvidualTransaction);
            initialHashes[i] = picosha2::hash256_hex_string(std::to_string(inidvidualTransaction[0])+std::to_string(inidvidualTransaction[1])+std::to_string(inidvidualTransaction[2])+std::to_string(inidvidualTransaction[3]));
        }

        //pair up into pairs of 2, doesn't matter if a post goes with a transaction
        for (int i = 0; i < 6; i++)
        {
            secondHashBatch[i] = initialHashes[2*i]+initialHashes[2*i+1];
        }

        //HASH 2: Hash these pairs using SHA256
        for (int i = 0; i < 6; i++)
        {
            secondHashBatch[i] = picosha2::hash256_hex_string(secondHashBatch[i]);
        }

        //pair up into pairs again
        for (int i = 0; i < 3; i++)
        {
            thirdHashBatch[i] = secondHashBatch[2*i]+secondHashBatch[2*i+1];
        }

        //HASH 3: Hash these pairs using SHA256
        for (int i = 0; i < 3; i++)
        {
            thirdHashBatch[i] = picosha2::hash256_hex_string(thirdHashBatch[i]);
        }

        //FINAL HASH: Hash the three remainders together.
        finalHash = picosha2::hash256_hex_string((thirdHashBatch[0]+thirdHashBatch[1]+thirdHashBatch[2]));

        return finalHash;
    }

};
} // namespace block
#endif // BLOCK_H



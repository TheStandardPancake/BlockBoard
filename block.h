#include "picosha2.h"
#include <string>

/** THIS HEADER FILE IS USED FOR CREATION AND STORAGE OF INDIVIDUAL BLOCK DATA AND PROVIDES THE FOLLOWING FUNCTIONS
* CreateHeader( previousHash(string), timeStamp(int), nonce(int), merkleTreeHash(string) ) --> This will store all the header data for an instance of the class (AKA each block)
* addPost( postData(string) ) --> This will take the post data as a string and store it in the block
* addTransactions( transactionList(int[11][4]) ) --> This will take a list of transaction data and store it in the block
* MerkleHash( post(string), transactions(int[])) --> This will hash all the post data and transaction data together and return a SHA256 string of the hashed result.
*/


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
        timeStamp = tS
                    nonce = n;
        merkleTreeHash = mTH;
    }

    //_____________________________The Body_____________________________

    std::string postData;
    int transactionData[11][4]; //each transaction will be an array with {address from, amount transfered, address to, hashed transaction data by address from}

    void addPost(std::string input)
    {
        postData = input;
    }


    void addTransactions(int transactionList[11][4])
    {
        for (int i; i = 0; i++)
        {
            transactionData[i] = transactionList[i];
        }
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
            initialHashes[i] = picosha2::hash256_hex_string(std::to_string(transactions[i][0])+std::to_string(transactions[i][1])+std::to_string(transactions[i][2])+std::to_string(transactions[i][3]));
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



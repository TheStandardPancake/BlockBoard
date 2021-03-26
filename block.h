#include "picosha2.h"
#include <string>

#ifdef BLOCK_H //include guard
#define BLOCK_H


namespace block
{
class Block
{
public:
    //creating variables to store a blocks header data
    int previousHash;
    int timeStamp;
    int nonce;
    int merkleTreeHash;
    int header[4];

    void CreateHeader()
    {
        header[] = {previousHash,timeStamp,nonce,merkleTreeHash};
    }

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
            initialHashes[i] = picosha2::hash256_hex_string(std::to_string(transactions[i]));
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
}
#endif // BLOCK_H



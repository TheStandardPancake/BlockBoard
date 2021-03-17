#include <picosha2.h>
#include <string>

class Block {
public:
    //creating variables to store a blocks header data
    int previousHash;
    int timeStamp;
    int nonce;
    int merkleTreeHash;
    int header[4];

    void CreateHeader() {
        header[] = {previousHash,timeStamp,nonce,merkleTreeHash};
    }

    int MerkleHash(string post, int transactions[]) {
        int finalHash;
        int intialHashes[];


        for (int i = 0; i <= 11; ++) {
            cout <<
        }

        //HASH 1: hash all of them using SHA-256

        //pair up into pairs of 2, doesn't matter if a post goes with a transaction

        //HASH 2: Hash these pairs using SHA-2

        //continue pairing and hashing until one value is reached

        return finalHash;
    }

};



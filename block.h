#include <sha256.h>

class Block {
public:
    int previousHash;
    int timeStamp;
    int nonce;
    int merkleTreeHash;

    int [] ReturnHeader() {
        int [] header = {previousHash,timeStamp,nonce,merkleTreeHash}
        return header;
    }

    int [] ReturnBody() {
        int [] body = {}
        return body;
    }

};

#ifndef _CONSISTENT_HASHING_H
#define _CONSISTENT_HASHING_H

#include <string>
#include <vector>
#include <map>

#include "Types.h"
#include "Address.h"

#define FNV_PRIME32 (1 << 24 + 1 << 8 + 0x93)
#define FNV_BASIS32 0x811c9dc5

#define MURMUR_C1 0xcc9e2d51
#define MURMUR_C2 0x1b873593
#define MURMUR_N  0xe6546b64
#define MURMUR_P1 0x85ebca6b
#define MURMUR_P2 0xc2b2ae35
#define MURMUR_R1   15
#define MURMUR_R2   13
#define MURMUR_M     5
#define MURMUR_SEED 17

#define NVIRT (1 << 20)

class ConsistentHashing
{
    public:
    std::map<uint32_t, int> ipMap;
    std::map<uint32_t, int> kmHash;

    public:
    // fnvHash32: FNV hashing for strings
    // parameters: key-string type key
    // outputs   : fnv hash value
    uint32_t fnvHash32(std::string key)
    {
        uint32_t hv = FNV_BASIS32;
        for(int i = 0; i < key.length(); i++)
        {
            hv = (hv ^ key[i]) * FNV_PRIME32;
        }
        
        hv += hv << 13;
        hv ^= hv >> 7;
        hv += hv << 3;
        hv ^= hv >> 17;
        hv += hv << 5;
        
        if(hv < 0)
        {
            hv = -hv;
        }
        
        return hv;
    }

    // murmurHash32: murmur hashing for strings
    // parameters: key-string type key
    //             seed-seed value of hash
    // outputs   : murmur hash value
    uint32_t murmurHash32(const char *key, int len, int seed = 17)
    {
        int hv = seed;
        
        int *block = (int *)key;
        int nb = len / 4, k0 = 0;
        for(int i = 0; i < nb; i++)
        {
            k0 = block[i];
            k0 *= MURMUR_C1;
            k0 = (k0 << MURMUR_R1) | (k0 >> (32 - MURMUR_R1));
            k0 *= MURMUR_C2;
            hv ^= k0;
            hv = (hv << MURMUR_R2) | (hv >> (32 - MURMUR_R2));
            hv = hv * MURMUR_M + MURMUR_N;
        }
        
        int *remain = (int *)(key + nb * 4);
        int nr = len % 4, k1 = 0;
        for(int i = 0; i < nr; i++)
        {
            k1 ^= remain[i] << (8 * i);
        }
        k1 *= MURMUR_C1;
        k1 = (k1 << MURMUR_R1) | (k1 >> (32 - MURMUR_R1));
        k1 *= MURMUR_C2;
        hv ^= k1;
        
        hv ^= len;
        hv ^= (hv >> 16);
        hv *= MURMUR_P1;
        hv ^= (hv >> 13);
        hv *= MURMUR_P2;
        hv ^= (hv >> 16);
        
        return hv;
    }

    // shiftHash32: Thomas Wang's 32 bit Mix Function
    // parameters: key-int type key
    // outputs   : shift hash value
    uint32_t shiftHash32(uint32_t key)
    {
        uint32_t hv = key;
        hv += ~(hv << 15);
        hv ^= (hv >> 10);
        hv += (hv << 3);
        hv ^= (hv >> 6);
        hv += ~(hv << 11);
        hv ^= (hv >> 16);
        return hv;
    }

    // buildConsHash: build Consistent Hashing
    // parameters: ipv-machine's ip list
    // outputs   : void
    void buildConsHash(std::vector<Address> ipv)
    {
        for(int i = 0; i < ipv.size(); i++)
        {
            insertMachine(ipv[i], i);
        }
    }

    // kvHash: return machine's IP to which the item belong
    //           to operate this item, pass the ip to socket, 
    //           send an operation command, and the machine
    //           handle it in itself kv engine 
    // parameters: itemKey-key of item
    // outputs   : ip of machine
    int kvHash(uint32_t key)
    {
        uint32_t hv = shiftHash32(key);
        std::map<uint32_t, int>::iterator it = ipMap.upper_bound(hv);
        return (it == ipMap.end() ? ipMap.begin()->second : it->second);
    }

    // insertMachine: insert a new machine to Consistent
    //                hashing (need cooperation from Raft)
    // parameters: ip-ip address of new insert machine
    // outputs   : void
    // attention : haven't move items to new machine here!
    void insertMachine(Address ip, int mId)
    {
        uint32_t hv = murmurHash32(ip.hostname.c_str(), ip.hostname.length(), MURMUR_SEED);
        kmHash.insert({hv, mId});
        for(int i = 0; i < NVIRT; i++)
        {
            std::string vir = ip.hostname + std::to_string(i);
            hv = murmurHash32(vir.c_str(), vir.length(), MURMUR_SEED);
            kmHash.insert({hv, mId});
        }
    }

    // deleteMachine: delete a new machine from consistent
    //                hashing (need cooperation from Raft)
    // parameters: ip-ip address of machine to delete
    // outputs   : void
    //void deleteMachine(std::string ip);

};

#endif // _CONSISTENT_HASHING_H
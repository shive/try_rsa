// -*- coding: utf-8-with-signature-unix -*-
//=====================================================================================================================
//! @author hshibuya <goe@fuzz.co.jp>

#include <stdio.h>
#include <string.h>

template<int S, typename T=unsigned int>
class BigInt {
public:
    BigInt(T rhs=0) {
        *this = rhs;
    }
    bool iszero() const {
        for(int Li = 0; Li < S; ++Li){
            if(v[Li]){
                return false;
            }
        }
        return true;
    }
    BigInt& operator = (const T& rhs) {
        ::memset(v, 0, sizeof(v));
        v[0] = rhs;
        return *this;
    }
    BigInt& operator += (const BigInt& rhs) {
        unsigned long long tmp = 0;
        for(int Li = 0; Li < S; ++Li){
            tmp += static_cast<unsigned long long>(v[Li]) + rhs.v[Li];
            v[Li] = tmp & M;
            tmp >>= B;
        }
        return *this;
    }
    BigInt& operator -= (const BigInt& rhs) {
        unsigned long long tmp = 0;
        for(int Li = 0; Li < S; ++Li){
            tmp += rhs.v[Li];
            if(v[Li] >= tmp){
                v[Li] -= tmp;
                tmp = 0;
            } else {
                v[Li] += (1ULL << B) - tmp;
                tmp = 1;
            }
        }
        return *this;
    }
    BigInt& operator *= (const T& rhs) {
        unsigned long long tmp = 0;
        for(int Li = 0; Li < S; ++Li){
            tmp += static_cast<unsigned long long>(v[Li]) * rhs;
            v[Li] = tmp & M;
            tmp >>= B;
        }
        return *this;
    }
    BigInt& operator /= (const T& rhs) {
        unsigned long long tmp = 0;
        for(int Li = S - 1; Li >= 0; --Li){
            tmp += static_cast<unsigned long long>(v[Li]);
            v[Li] = tmp / rhs;
            tmp = (tmp % rhs) << B;
        }
        return *this;
    }
    T operator % (const T& rhs) const {
        BigInt tmp = *this;
        tmp /= rhs;
        tmp *= rhs;
        BigInt ret = *this;
        ret -= tmp;
        return ret.v[0];
    }
    void dump() const {
        const unsigned char* e = reinterpret_cast<const unsigned char*>(&v[0]) - 1;
        const unsigned char* p = e + sizeof(v);
        while(p != e){
            printf("%02x", *p);
            --p;
        }
        printf("\n");
    }
private:
    T v[S];

    static const int B = sizeof(T) * 8;
    static const unsigned long long M = (1ULL << B) - 1;
};

unsigned int
powmod(unsigned int cc, unsigned int k, unsigned int n) {
    unsigned int s = k;
    unsigned long long tmp = cc;
    unsigned long long ans = 1;
    while(s){
        if(s & 1){
            ans = (ans * tmp) % n;
        }
        s >>= 1;
        tmp = tmp * tmp % n;
    }
    return ans;
}

int
main(int, const char**) {
    const unsigned int N = 0xfa9ecbbfUL;
    const unsigned int K = 0x10001UL;
    const unsigned int U = 0xda17e869UL;

    const char origin[] = "古池や蛙とびこむ水の音"; // 22 bytes
    BigInt<6> org;
    ::memcpy(&org, origin, sizeof(origin));
    printf("origin: %s\n", (const char*)&org);
    org.dump();

    printf("encrypt ");
    BigInt<6> enc;
    while(!org.iszero()){
        printf(".");
        enc *= N;
        enc += powmod(org % N, U, N);
        org /= N;
    }
    printf("\rencrypt: (binary)\n");
    enc.dump();

    BigInt<6> dec;
    while(!enc.iszero()){
        dec *= N;
        dec += powmod(enc % N, K, N);
        enc /= N;
    }

    printf("decrypt: %s\n", (const char*)&dec);
    dec.dump();

    return 0;
}

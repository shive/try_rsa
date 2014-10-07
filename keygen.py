#!/bin/env python
# -*- mode: python; coding: utf-8-sig -*-
#======================================================================================================================
__author__ = 'hshibuya <goe@fuzz.co.jp>'

import sys
import random
from math import log2

def prime(base):
    n = base
    while True:
        try:
            for s in range(2, n - 1):
                if 0 == n % s:
                    raise Exception()
            return n
        except Exception:
            n += 1
            continue

def main():

    P = prime(0xfe00)
    Q = prime(0xfc80)
    N = P * Q
    K = 0x10001
    O = (P - 1) * (Q - 1)
    # K * U - O * V = 1
    # U = (O * V + 1) / K
    U = []
    for V in range(5000000):
        if 0 == (O * V + 1) % K:
            U.append(((O * V + 1) // K, V))
    # U, V = random.choice(U)
    U, V = sorted(U, key=lambda e: e[1])[0]

    print('P = %x' % P)
    print('Q = %x' % Q)
    print('N = %x / log2(N): %g' % (N, log2(N)))
    print('K = %x' % K)
    print('U = %x (V = %x)' % (U, V))

    # ### 鍵の検証
    # for d in range(N):
    #     print('\rCHECK:%08d:%08d:' % (N - 1, d), end='')
    #     dd = pow(d, U, N)       ### 秘密鍵で暗号化
    #     ddd = pow(dd, K, N)     ### 公開鍵で復号化
    #     assert d == ddd, '鍵の検証に失敗(%d,%d,%d)' % (d,dd,ddd)

    ### 暗号化
    ORIGIN = '古池や蛙とびこむ水の音'.encode('cp932')
    print('len(ORIGIN) =', len(ORIGIN), ORIGIN)

    origin_number = 0
    for cc in reversed(ORIGIN):
        origin_number = origin_number * 256 + cc
    print(' origin_number: %.3f %x' % (log2(origin_number) / 32, origin_number))
    
    encrypt_number = 0
    num = origin_number
    while num > 0:
        d = num % N
        dd = pow(d, U, N)
        num //= N
        encrypt_number = encrypt_number * N + dd
    print('encrypt_number: %.3f %x' % (log2(encrypt_number) / 32, encrypt_number))
    
    decrypt_number = 0
    num = encrypt_number
    while num > 0:
        d = num % N
        dd = pow(d, K, N)
        num //= N
        decrypt_number = decrypt_number * N + dd
    print('decrypt_number: %.3f %x' % (log2(decrypt_number) / 32, decrypt_number))

if __name__ == '__main__':
    main()


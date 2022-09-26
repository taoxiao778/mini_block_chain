from flask import Flask
from flask import request
import json
import  sys
import random
import hashlib

# data={'prevhash':'','nounce':10,'height':0,'transcation':'abcdef'}
# data=json.dumps(data)
# hl=hashlib.md5()
# hl.update(data.encode(encoding='UTF-8'))
# prevhash=hl.hexdigest()
# print(prevhash)

#构建区块
# def creat_block():
#     global height,blkchain
#     trans=select_trans()
#     if len(trans)==0:#交易池没有交易，暂不构建区块
#         print('暂不构建区块')
#         return 
#     if height==0:#创世区块
#         prevhash=''
#     else:
#         hl=hashlib.md5()
#         hl.update(blkchain[height-1].encode(encoding='utf-8'))
#         prevhash=hl.hexdigest()

#     # nounce=random.randint(0,100)#暂时 
    
#     data={'prevhash':prevhash,'nounce':nounce,'height':height,'transcation':trans}
#     blkchain.append(json.dumps(data))
#     height+=1

# alphabet = 'abcdefghijklmnopqrstuvwxyz'
# nonce = ''.join(random.sample(alphabet, 5))
# print(nonce)

def printarg():
    print(sys.argv[1])

printarg()

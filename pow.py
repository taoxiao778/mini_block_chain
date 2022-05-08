#挖矿 done
from flask import Flask
import json
import  sys
import random
import hashlib
import string
import time
import requests

url="http://127.0.0.1:"+sys.argv[1]+"/getNonce"

with open("target.txt", "r") as f:
    data = f.readlines()

def send_msg(url,msg):
    #向结点发送交易信息
    res=requests.post(url=url,data=msg)
    print(res.text)

while 1:
    find=0
    while 1:
        alphabet = 'abcdefghijklmnopqrstuvwxyz'
        nonce = ''.join(random.sample(alphabet, 5))
        # nonce = ''.join(random.sample(string.ascii_letters, 5))
        for i in data:
            i=i[:-1]
            if nonce==i:
                find=1
                break
        if find==1:
            print('find!')
            break
           
    msg={'msg':nonce}
    send_msg(url,msg)



# with open("target.txt","a") as f:
#     for i in range(1000):
#         alphabet = 'abcdefghijklmnopqrstuvwxyz'
#         nonce = ''.join(random.sample(alphabet, 5))
#         f.write(nonce)
#         f.write('\n')

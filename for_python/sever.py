#网络上的结点，接收client和其他sever发来的消息，打包区块 广播
from flask import Flask
from flask import request
import json
import  sys
import random
import hashlib
import time
import requests

transpool=[]#本地交易池 dict
blkchain=[]#该结点维护的区块链 json
height=0#当前区块的高度
used_nonce=[]#已经用过的nonce

#接收client的交易请求，存入本地交易池 done
app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def home():
    return '<h1>Hello World</h1>'

@app.route('/getMsg', methods=['POST'])
def getMsg():
    global transpool
    m = request.get_json()
    m = json.loads(m)
    transpool.append(m)
    print(m)
    
    return 'message successfully received'

@app.route('/getNonce', methods=['POST'])
def getNonce():#构建区块 done
    nounce = request.form['msg']
    print('找到的nonce是：',nounce)
    #判断该nonce是否已经被发布过
    if is_used(nounce):
        return 'no block is created'

    used_nonce.append(nounce)
    global height,blkchain
    trans=select_trans()
    if len(trans)==0:#交易池没有交易，暂不构建区块
        print('暂不构建区块')
        return 'no block is created'

    if height==0:#创世区块
        prevhash=''
    else:
        hl=hashlib.md5()
        hl.update(blkchain[height-1].encode(encoding='utf-8'))
        prevhash=hl.hexdigest()

    data={'prevhash':prevhash,'nounce':nounce,'height':height,'transcation':trans}
    blkchain.append(json.dumps(data))
    height+=1

    send_blk()
    return 'nounce successfully received'

@app.route('/getBlk', methods=['POST'])
def getBlk():#接收其他结点发来的区块，并验证nonce
    global transpool,height,blkchain
    m = request.get_json()
    m = json.loads(m)
    #验证nonce
    other_nonce=m['nounce']
    find=0
    with open("target.txt", "r") as f:
        data = f.readlines()
    for i in data:
        i=i[:-1]
        if other_nonce==i:
            find=1
            used_nonce.append(other_nonce)
            break
    #验证prevhash
    if height==0:
        check=1
    else:
        check=check_prevhash(m['prevhash'])

    if find==1 and check==1:#合法 该块加入本结点维护的区块链
        blkchain.append(json.dumps(m))
        height+=1
        #从交易池删除该块的交易
        transpool.remove(m['transcation'][0])
        transpool.remove(m['transcation'][1])

        print('交易池剩余交易：',transpool)
        return 'block successfully received'
    else:#不合法
        return 'block is refused'

#从交易池选交易打包 done 
def select_trans():
    global transpool
    if len(transpool)<2:
        print('当前交易池交易数量不足')
        return []
    transs=[]
    for i in range(2):
        index=random.randint(0,len(transpool)-1)
        s1 = transpool.pop(index)  # 删除第index个
        transs.append(s1)
    print('del: ',transs)
    # print('last: ',transpool)
    return transs

def send_msg(url,msg):
    #向结点发送信息
    res=requests.post(url=url,json=msg)
    print(res.text)

def send_blk():#向其他的结点发送刚刚打包的区块 done
    if sys.argv[1]=="1000":
        address="2000"
    elif sys.argv[1]=="2000":
        address="1000"

    url="http://127.0.0.1:"+address+"/getBlk"
    send_msg(url,blkchain[-1])

def is_used(nonce):#判断nonce是否用过 done
    for i in used_nonce:
        if nonce==i:
            return 1
    return 0

def check_prevhash(received_prevhash):#验证prevhash done
    global blkchain,height
    hl=hashlib.md5()
    hl.update(blkchain[height-1].encode(encoding='utf-8'))
    my_prevhash=hl.hexdigest()
    if my_prevhash==received_prevhash:
        return 1
    else:
        return 0

if __name__ == '__main__':
    #app.run()

    p = sys.argv[1]
    app.run(host='127.0.0.1', port=p)
    
    print(blkchain)


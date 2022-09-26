#模拟客户，每隔一段时间向网络中的结点发送交易信息 done
import json
import string
import random
import requests
import time

tx_id=0 #交易id
def creat_random_str():
    #生成长度为5的随机字符串（作为交易账户）
    s=''.join(random.choice(string.ascii_letters+string.digits) for _ in range(5))
    return s
def creat_trans():
    #创建一个交易
    global tx_id
    trans_in=creat_random_str()
    trans_out=creat_random_str()
    money=random.randint(1,500)
    msg={"tx_id":tx_id,"trans_out":trans_out,"trans_in":trans_in,"money":money}
    tx_id+=1
    return msg
def send_msg(url,msg):
    #向结点发送交易信息
    res=requests.post(url=url,json=msg)
    print(res.text)

while 1:
    msg=creat_trans()
    data=json.dumps(msg)
    # print(data)
    url="http://127.0.0.1:1000/getMsg"
    url2="http://127.0.0.1:2000/getMsg"
    send_msg(url,data)
    send_msg(url2,data)
    time.sleep(1.5)#请求过快会被封 怎么解决

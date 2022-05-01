//模拟一个终端客户，每隔一定时间（比如100毫秒）随机向另外两个程序发送交易请求或查询请求。
//交易数据从实验02的数据集中生成。每条请求将“插入”到对应节点的“客户消息队列”尾部
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

//交易
typedef struct Transaction
{
    int blockheight;
    char txid[100];
    int input_cnt;
    int output_cnt;
}Transaction;

//交易池链表
typedef struct TranList
{
    struct Transaction transaction;
    struct TranList *next;
    
}TranList,*TranListptr;

TranListptr T;//把T设为全局变量，表示交易池
int length=0;//length为交易池里交易的数量

//读csv文件，创建交易池 
TranListptr ReadTrans()
{
    FILE *fp=NULL;
    fp=fopen("D:\\codes\\vscode_c\\shujujiegou\\qukuailian\\2009transactions.csv","at+");
    char *line,*record;
    char buffer[1000];
    TranListptr r,p;
    T=(TranListptr)malloc(sizeof(TranList));
    T->next=NULL;
    // T->length=0;
    r=T;
    if(fp!=NULL)
    {
        printf("success open trans.csv\n");
        fseek(fp,0L,SEEK_SET);
        int i=0;
        while((line=fgets(buffer,sizeof(buffer),fp))!=NULL)//按行读csv
        {
            if(i==0)//跳过第一行
            {
                i++;
                continue;
            }
            p=(TranListptr)malloc(sizeof(TranList));
            p->next=NULL;//尾插法构建链表
            //依次读入数据
            record=strtok(line,",");
            p->transaction.blockheight=atoi(record);
            record=strtok(NULL,",");
            strcpy(p->transaction.txid,record);
            record=strtok(NULL,",");
            p->transaction.input_cnt=atoi(record);
            record=strtok(NULL,",");
            p->transaction.output_cnt=atoi(record);  

            r->next=p;
            r=p;
            length++;//交易池的交易数+1
            // free(p);
            // printf("len=%d\n",length); 
            // printf("t len=%d h=%d\n",T->length,T->transaction.blockheight);
        }
        fclose(fp);
    }
    else
    {
        printf("fail open tran.csv\n");
        exit(0);
    }
    return T;
}

char* TransToMsg(TranListptr p)//把p交易的内容转为字符串
{
    // 使用strcat把p的属性连成一个字符串
    char *msg;
    msg=(char*)malloc(sizeof(char)*200);
    strcpy(msg,"type:transaction,blockheight:");//打印不出height:???
    // printf("msg=%s\n",msg);
    char h[50];
    itoa(p->transaction.blockheight,h,10);
    // printf("h=%s\n",h);
    strcat(msg,h);
    // printf("1msg=%s\n",msg);
    strcat(msg,",txid:");
    strcat(msg,p->transaction.txid);
    strcat(msg,",input_cnt:");
    char in[5];
    itoa(p->transaction.input_cnt,in,10);
    strcat(msg,in);
    strcat(msg,",output_cnt:");
    char out[5];
    itoa(p->transaction.output_cnt,out,10);
    strcat(msg,out);
    // printf("msg=%s\n",msg);
    return msg;
}         

char *Get_Next_Num(char *tail_name)//获取下一个文件名
{
    char *name_num;
    name_num=(char*)malloc(sizeof(char)*20);
    // printf("tail_name:%s\n",tail_name);
    int i;
    for(i=0;i<strlen(tail_name);i++)//把当前最后一个文件的名拿出来
    {
        if(tail_name[i]!='.')
        {
            name_num[i]=tail_name[i];
        }
        else break;
    }
    name_num[i] = '\0';
    // printf("name_num=%s\n",name_num);
    char* new_name;
    new_name=(char*)malloc(sizeof(char)*20);
    // printf("atoi(name_num)=%d\n",atoi(name_num));
    itoa(atoi(name_num)+1,new_name,10);//把最后一个文件的名+1就是新文件名
    strcat(new_name,".txt");
    // printf("[GetNextNum]newName=%s\n",new_name);
    free(name_num);
    return new_name;

}

void Send_Msg(char *msg,char* dest_node)//向dest_node结点发送msg
{
    //获取目标结点的文件夹路径
    char dirpath[100]="D:\\codes\\vscode_c\\shujujiegou\\MiniBlockListSystem\\data\\";
    strcat(dirpath,dest_node);
    strcat(dirpath,"\\msg_clt\\");
    // printf("dirpath=%s\n",dirpath);
    //获取尾指针路径
    char tail_dirpath[100];
    strcpy(tail_dirpath,dirpath);
    strcat(tail_dirpath,"tail.txt");
    // printf("tail_dirpath=%s\n",tail_dirpath);
    //打开tail.txt，获取尾文件的名
    char *tail_name;
    FILE *fp;
    fp=fopen(tail_dirpath,"r");
    char buffer[10];
    tail_name=fgets(buffer,sizeof(buffer),fp);
    // printf("tailname=%s\n",tail_name);
    fclose(fp);
    //新建下一个文件名
    char *new_file_name=Get_Next_Num(tail_name);
    // printf("newfilename=%s\n",new_file_name);    
    char new_dirpath[100];
    strcpy(new_dirpath,dirpath);
    strcat(new_dirpath,new_file_name);
    // printf("newdirpath=%s\n",new_dirpath);
    //在新文件中写入
    FILE *fp2;
    fp2=fopen(new_dirpath,"w"); 
    fputs(msg,fp2);
    fclose(fp2);
    //向原本的尾结点写入next
    FILE *fp4;
    char last_dirpath[100];
    strcpy(last_dirpath,dirpath);
    strcat(last_dirpath,tail_name);
    // printf("last_dirpath=%s\n",last_dirpath);
    fp4=fopen(last_dirpath,"a");
    char *next;
    next=(char*)malloc(sizeof(char)*30);
    strcpy(next,",next:");
    strcat(next,new_file_name);
    // printf("new_filename2=%s\n",new_file_name);
    // printf("next=%s\n",next);
    fputs(next,fp4);
    fclose(fp4);
    //更新tail.txt
    FILE *fp3;
    fp3=fopen(tail_dirpath,"w");
    fputs(new_file_name,fp3);
    fclose(fp3);
    // free(next);
}

void Transcate_sendmsg(TranListptr T)
{
    //从交易池随机选一个交易
    // srand((unsigned int)time(NULL));
    int rand_val=rand()%length; 
    TranListptr p=T->next;
    for(int i=0;i<rand_val;i++)
    {
        p=p->next;
    }
    // printf("p h=%d rand=%d\n",p->transaction.blockheight,rand_val);
    //把p交易的内容转为字符串
    char *msg;
    msg=TransToMsg(p);
    // printf("[Tran_send]msg=%s\n",msg);
    //从B C随机选择一个结点
    // srand((unsigned int)time(NULL));
    int rand_val2=rand()%3;
    char *dest_node;
    if(rand_val2==0) dest_node="b";
    else if(rand_val2==1) dest_node="d";
    else dest_node="c";
    // printf("dext_node=%s\n",dest_node);
    //发送
    Send_Msg(msg,dest_node);
}

void Query_sendmsg(TranListptr T)
{
    // char *msg;
    // msg=(char*)malloc(sizeof(char)*100);
    char msg[100];
    strcpy(msg,"type:query,txid:");
    // printf("msg1=%s\n",msg);
    //从交易池随机选一个交易，把它的ID加到msg后面
    // srand((unsigned int)time(NULL));
    int rand_val=rand()%length; 
    TranListptr p=T->next;
    for(int i=0;i<rand_val;i++)
    {
        p=p->next;
    }
    strcat(msg,p->transaction.txid);
    // printf("msg2=%s\n",msg);
   
    //从B C随机选择一个结点
    // srand(time(0));
    int rand_val2=rand()%3;
    char *dest_node;
    if(rand_val2==0) dest_node="b";
    else if(rand_val2==1) dest_node="d";
    else dest_node="c";
    //发送
    Send_Msg(msg,dest_node);
}

int main()
{
    TranListptr tran_pools=ReadTrans();
    srand((unsigned int)time(NULL));
    // printf("tran=%s\n",tran_pools->next->next->transaction.txid);
    while(1)
    {
        
        double rand_val=((double)rand())/RAND_MAX;//创造一个0~1随机数
        printf("randval=%lf\n",rand_val);
        //  rand_val=0.15;
        if(rand_val>=0&&rand_val<0.45)//发送交易请求
        {
            Transcate_sendmsg(tran_pools);
            printf("Send Transcate success\n");
        }
        else if(rand_val>=0.45&&rand_val<0.9)//发送查询请求
        {
            Query_sendmsg(tran_pools);
            printf("send query success\n");
        }
        else printf("sleep\n");
        sleep(0.5);
    }   
    return 0; 
}

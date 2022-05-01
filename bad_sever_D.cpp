#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
typedef struct BlockNode
{
    char transactions[1000];//????
    char hash[200];
    char prevhash[200];
    int height;
    struct BlockNode *lchild,*rchild;
    int transactions_num;
}BlockNode,*BlockNodePtr;

typedef struct BlockLinkTree
{
    struct BlockNode* head;
    struct BlockNode* tail;
}BlockLinkTree,*BlockLinkTreePtr;
//队列的操作：

char* get_queue_top(char* msg_queue)//找到队首元素的内容
{
    // printf("in get queue top\n");
    // printf("in get queue top : %s\n", "msg_queue");
    // printf("msg_queue=%s\n",msg_queue);
    char head_filename[200];//头结点的路径
    strcpy(head_filename,msg_queue);
    strcat(head_filename,"0.txt");
    // printf("head_filename=%s\n",head_filename);
    FILE *fp;
    // printf("1?????%s\n", head_filename);

    fp=fopen(head_filename,"r");
    // printf("2?????\n");
   
    // char buffer[1200];   
    char *head_data=(char*)malloc(200);   
    // char head_data[200];
    // memset(head_data, 200, 0);
    // head_data[0] = '\0';
    // printf("3?????\n");

    // head_data;   
    // printf("4?????\n");

    if(fgets(head_data,200,fp) == NULL){
        printf("cannot read!\n");
    }
    // printf("5?????\n");
    // printf("headdata=%s\n",head_data);
    if(strlen(head_data)<=8)
    {
        // printf("queuetop is empty\n");
        return NULL;
    } 
    fclose(fp);
    char *headnext_filename;//首元素的文件名
    headnext_filename=(char*)malloc(sizeof(char)*100);
    printf("head_data+7=%s\n",head_data+7);
            // printf("debugdebug111");

    strcpy(headnext_filename,head_data+8);

    char headnext_filepath[200];//首元素的文件路径
    strcpy(headnext_filepath,msg_queue);
    strcat(headnext_filepath,headnext_filename);
    FILE *fp2;
    fp2=fopen(headnext_filepath,"r");
    // printf("headnext_filepath=%s\n",headnext_filepath);  
    // char buffer[100];
    char *headnext_data;
    headnext_data=(char*)malloc(sizeof(char)*2000);
    fgets(headnext_data,2000,fp2);//首元素的文件内容
    fclose(fp2);

    // printf("headnext data=%s\n",headnext_data);
    free(headnext_filename);
    free(head_data);
    return headnext_data;
}

char *Get_Next_Num(char *tail_name)//获取下一个文件名
{
    char *name_num;
    name_num=(char*)malloc(sizeof(char)*200);
    for(int i=0;i<strlen(tail_name);i++)//把当前最后一个文件的名拿出来
    {
        if(tail_name[i]!='.')
        {
            name_num[i]=tail_name[i];
        }
        else break;
    }
    // printf("name_num=%s",name_num);
    char* new_name;
    new_name=(char*)malloc(sizeof(char)*200);
    // printf("atoi(name_num)=%d ",atoi(name_num));
    itoa(atoi(name_num)+1,new_name,10);//把最后一个文件的名+1就是新文件名
    strcat(new_name,".txt");
    // printf("[GetNextNum]newName=%s\n",new_name);
    free(name_num);
    return new_name;

}


int enqueue(char *msg_queue,BlockNodePtr new_block)//入队
{
    // printf("in enqueue\n");
    char tail_filename[100];//tail.txt的路径
    strcpy(tail_filename,msg_queue);
    strcat(tail_filename,"tail.txt");
    char *tail_name;
    tail_name=(char*)malloc(sizeof(char)*200);
    FILE *fp;
    fp=fopen(tail_filename,"r");
    // char buffer[200];
    fgets(tail_name,200,fp);//获得尾元素的文件名
    // printf("tailname=%s\n",tail_name);
    fclose(fp);
    //新建下一个文件名 
    char *new_file_name=Get_Next_Num(tail_name);
    // printf("newfilename=%s\n",new_file_name);    
    char new_dirpath[100];
    strcpy(new_dirpath,msg_queue);
    strcat(new_dirpath,new_file_name);//获得下一个文件名的路径
    // printf("new_dirpath=%s\n",new_dirpath);
    //写新结点
    FILE *fp5;
    fp5=fopen(new_dirpath,"w");
    
    char msg[1000];
    strcpy(msg,"height:");
    char h[10];
    itoa(new_block->height,h,10);
    strcat(msg,h);
    // printf("msg1=%s\n",msg);
    strcat(msg,",hash:");
    strcat(msg,new_block->hash);
    strcat(msg,",prevhash:");
    strcat(msg,new_block->prevhash);
    strcat(msg,",transactions:");
    strcat(msg,new_block->transactions);
    // printf("[1]msg=%s\n",msg);
    fputs(msg,fp5);
    fclose(fp5);
    //向原本的尾结点写入next
    FILE *fp4;
    char last_dirpath[100];
    strcpy(last_dirpath,msg_queue); 
    strcat(last_dirpath,tail_name);//获得原来尾文件的路径
    char *next;
    next=(char*)malloc(sizeof(char)*200);
    if(strcmp(tail_name,"0.txt")==0)
    {
        fp4=fopen(last_dirpath,"w");
        strcpy(next,"{},next:");
    }else{
        fp4=fopen(last_dirpath,"a");
        strcpy(next,",next:");
    }
    // printf("last_dirpath=%s\n",last_dirpath);
    
    
    strcat(next,new_file_name);
    // printf("new_filename2=%s\n",new_file_name);
    // printf("last_dirpath:%s/next=%s\n",last_dirpath, next);
    fputs(next,fp4);//写入下一个文件名 
    fclose(fp4);
    //更新tail.txt
    FILE *fp3;
    fp3=fopen(tail_filename,"w");
    printf("<<<<<<......new tailname=%s\n",new_file_name);
    fputs(new_file_name,fp3);
    fclose(fp3);
    free(tail_name);
    free(next);
    return 1;
}

char* GetNextContent(char* content)//获取文件内容中的next
{
    // printf("in get next content\n");
    // printf("content=%s\n",content);
    char *next_content=NULL;
    next_content=(char*)malloc(sizeof(char)*2000);
    if(!strstr(content, "next")){
        return NULL;
    }
    for(int i=strlen(content)-6;i>=0;i--)
    {
        if(content[i]=='n'&&content[i+1]=='e'&&content[i+2]=='x'
        &&content[i+3]=='t') 
        {
            
            strcpy(next_content,content+i+5);
            // printf("nextcontent=%s\n",next_content);
            break;
        }
    }
    // printf("next_content=%s\n",next_content);
    return next_content;
}

char *dequeue(char* msg_queue)//出队
{
    // printf("in dequeue\n");
    //获取队首的内容
    char *queue_top_data = get_queue_top(msg_queue);
    
    if(!queue_top_data)
    {
        // printf("empty queue\n");
        return 0;
    }
    // printf("queue_top_data=%s\n",queue_top_data);
    //获取队首元素的next
    char *topNode_next;
    // printf("123 queue_top_data=%s\n",queue_top_data);
    topNode_next=GetNextContent(queue_top_data); 
    if(topNode_next){
        // printf("topnode_next=%s\n",topNode_next);

    }
    // printf("hihihihihi!!!!!!!!!!!!!!!!\n");
    //重写head指针       
    char head_filename[1200];//头结点的路径
    
    strcpy(head_filename,msg_queue);
    strcat(head_filename,"0.txt");
    // printf("head_filename=%s\n",head_filename);
    FILE *fp;
        // printf("hihihihihi2!!!!!!!!!!!!!!!!\n");

    // printf("1the last <<<queue_top_data>>>:#%s#%s\n",topNode_next, queue_top_data);

    // printf("2the last <<<queue_top_data>>>:#%s#%s#\n",topNode_next, queue_top_data);

    char topnext[300]="{},next:";
    // printf("3the last <<<queue_top_data>>>:#%s#\n",queue_top_data);
    fp=fopen(head_filename,"w");
    // queue_top_data = get_queue_top(msg_queue);
    // printf("hihihihihi3!!!!!!!!!!!!!!!!\n");

    // printf("4the last <<<queue_top_data>>>:#%s#\n",queue_top_data);
    if(topNode_next){
        strcat(topnext,topNode_next);
    }
    // printf("hihihihihi4!!!!!!!!!!!!!!!!\n");

    if(!topNode_next){
        char* tail_file_path = (char*)malloc(sizeof(char)*200) ;
        strcpy(tail_file_path,msg_queue);
        strcat(tail_file_path,"tail.txt");
        FILE *fp_tail;
        fp_tail = fopen(tail_file_path,"w");
        fputs("0.txt",fp_tail);
        fclose(fp_tail);
    }
        // printf("hihihihihi5!!!!!!!!!!!!!!!!\n");

    // printf("topnext=%s\n",topnext); 
    fputs(topnext,fp);
    fclose(fp);
    printf("queue top data=%s\n",queue_top_data);
    return queue_top_data;
}
 
int is_empty(char* msg_queue)
{
    // printf("in isempty %s\n", msg_queue);
    char *queue_top=get_queue_top(msg_queue);
    if(!queue_top) 
    {
        // printf("empty\n");
        return 1;
    }
    else 
    {
        // printf("no empty\n");
        return  0;
    }
}

//交易池的操作：

typedef struct TranList
{
    char* data;
    struct TranList *next;
    struct TranList *tail;//指向链表的尾部
    int length;
}TranList,*TranListptr;


TranListptr Init()
{
    TranListptr T=(TranListptr)malloc(sizeof(TranList));
    T->next=NULL;
    T->tail=T;
    T->length=0;
    return T;
}

void Add(TranListptr T,char* elem)//把交易的内容加到交易池尾部
{
    printf("in Add\n");
    TranListptr p;
    p=(TranListptr)malloc(sizeof(TranList));
    p->data=(char*)malloc(sizeof(char)*200);
    strcpy(p->data,elem); 
    T->tail->next=p;
    T->tail=p;
    p->next=NULL;
    T->length++;//长度加一
    printf("p->data[Add]=%s\n",p->data);
    // printf("[Add]T->length=%d\n",T->length);
}
char* Get(char* msg,char* target)//从msg中找到target对应的值，类似字典
{
    if(!msg){
        return "";
    }
    if(strlen(msg) == 0){
        return "";
    }
    if(!strstr(msg, target)){
        return "";
    }
    // printf("in Get, msg: %s # target: %s\n", msg, target);
    char *s=strstr(msg,target);
    // printf("s=%s\n",s);
    // printf("targrt=%s\n",target);
    char *res;
    res=(char*)malloc(sizeof(char)*200);
    // printf("targrtl=%d  slen=%d\n",strlen(target),strlen(s));
    int i =strlen(target);
    for(;i<strlen(s);i++)
    {
        // printf("i=%d\n",i);
        if(s[i]!=',')
        {  
            res[i-strlen(target)]=s[i];
            // printf("#%c",res[i-strlen(target)]);
        }
        else break;
    }
    res[i-strlen(target)] = '\0';
    // printf("\n");

    // printf("Get end\n");
    return res;
}
char* Pop(TranListptr T)//从“交易池”中取出1个交易
{
    // printf("in pop\n");
    if(T->length==0)//如果交易池空的
    {
        printf("trans pool is empyt\n");
        return NULL;
    }
    int idx=rand()%(T->length);//取第idx个
    TranListptr p=T;
    // printf("0000\n");
    for(int i=0;i<idx;i++)
    {
        p=p->next; 
    }
    // printf("1111\n");
    TranListptr r=p->next;
    if(p->next==T->tail)
    {
        T->tail=p; 
    }
    // printf("%d\n",r);
    p->next=p->next->next;
    // printf("2222\n"); 
    T->length--;//交易数减一
    printf("[pop]r data=%s\n",r->data);
    
    return Get(r->data,"txid:");
}

char* PopN(TranListptr T,int n)//从“交易池”中取出n(>=1) 个交易，组成一个区块newBLK.
{
    printf("in popN\n");
    if(n>T->length) return NULL;
    char* datas;
    datas=(char*)malloc(sizeof(char)*2000);
    // strcpy(datas,"transactions:");

    strcpy(datas,Pop(T));
    for(int i=1;i<n;i++)
    {
        strcat(datas,";");
        char* pop_t = Pop(T);
        // printf("pop_t:%s\n", pop_t);
        strcat(datas,pop_t);
        
    }
    printf("pop datas:%s\n",datas);
    return datas;
}

// int TransNum_inBlock(char** transactions)//求块包含的交易数
// {
//     int rs=0;
//     while(transactions[rs])
//     {
//         rs++;
//     }
//     return rs;
// }


//根据交易的内容从交易池里删除交易
TranListptr del_by_ids(TranListptr T,char* data)
{
    // printf("in del by ids\n");
    TranListptr p=T;
    TranListptr r;
    while(p!=NULL)
    {
        // printf("in while\n");
        if(strcmp(p->next->data,data)==0)//如果交易的内容相等
        {
            // printf("in cmp\n");
            if(p->next==T->tail)//如果要删除的是尾结点  
            {
                T->tail=p;
            }
            r=p->next;//r指向这个被删除的交易
            p->next=p->next->next;
            T->length--;
        }
        
        p=p->next;
    }
    // printf("r=%s\n",r->data);
    return r;
}
void del_transactions(TranListptr T,BlockNodePtr block)//从“交易池”中删除该区块中包含的交易
{
   
    char *res;
    printf("trancs in block which need del=%s\n",block->transactions);
    res=strtok(block->transactions,";");
    for(int i=0;i<block->transactions_num;i++)
    {
        // printf("res=%s\n",res);
        del_by_ids(T,res);
        res=strtok(NULL,";");
    }
    //  printf("out del_transac\n");
}
int not_have_tid(TranListptr T,char* data)//判断交易池是否有这个交易
{
    // printf("[have_tid]data=%s\n",data);
    TranListptr p=T->next;
    // printf("T->length=%d\n",T->length);
    for(int i=0;i<T->length;i++)
    {
        // printf("<<<p->data>>>>=%s\n",p->data);
        if(strstr(data,p->data)!=NULL)//如果有返回1
        {
            return 0;
        }
        p=p->next;
    }
    // printf("no have id\n");
    return 1;//没有 返回1
}

//区块链表的操作：


void Init_BlockList(BlockLinkTreePtr B)
{
    // printf("Init 1\n");
    B->head->lchild=NULL;//head与第一个结点的链接？？？
    B->head->rchild=NULL;
    // printf("Init 2\n");
    B->head->height=0;
    B->tail=B->head;
    // printf("Init 3\n");

}
typedef struct SqStack
{
	BlockNode *base;//栈底指针
	BlockNode *top;//栈顶指针
	int	stacksize;//栈的大小
}SqStack;//顺序栈

//构造一个空栈
int init_stack(SqStack &S)
{
	S.base = (BlockNode *)malloc(200 * sizeof(BlockNode));//此处默认栈的大小为200
	if(!S.base)
		return 0;
	S.top = S.base;
	S.stacksize = 200;
	return 1;
}
//判断栈是否为空，若为空则返回1，否则返回0
int StackEmpty(SqStack S)
{
	if(S.base == S.top)
		return 1;
	else
		return 0;
}
//获取栈顶元素
int get_stack_top(SqStack S,BlockNode &e)
{
	if(S.base == S.top)
	{
		return 0;
	}
	e = *(S.top - 1);
	return 1;
}
//进栈操作
int Push_stack(SqStack &S,BlockNode e)
{
	if(S.top - S.base == S.stacksize)
		return 0;
	*S.top = e;
	S.top ++;
	return 1;
}
//出栈操作
int Pop_stack(SqStack &S,BlockNode &e)
{
	if(S.base == S.top)
		return 0;
	S.top --;
	e = *S.top;
	return 1;
}
//先序遍历二叉树,找到preblk
BlockNodePtr find_preblk(BlockLinkTreePtr T,char* prevhash)
{
	if(!T)//T头结点就是空的哇？？？？
		return 0;
	BlockNode p;
	SqStack S;
	init_stack(S);
	Push_stack(S,*(T->head));
	while(!StackEmpty(S))
	{
		Pop_stack(S,p);
        if(strcmp(p.hash,prevhash)==0)//找到preblk
        {
            return &p;
        }
		// printf("%c  ",p.data);
		if(p.rchild)
			Push_stack(S,*p.rchild);
		if(p.lchild)
			Push_stack(S,*p.lchild);
	}
	printf("\n");
	return T->head;//找不到 preblk就是头结点
}
//随机选择一个度为1或度为0的节点,lastblk
BlockNodePtr random_choose(BlockLinkTreePtr B,int num)
{
    if(num==0) return B->head;//如果树是空的

    
    BlockNode p;
	SqStack S;
	init_stack(S);
	Push_stack(S,*(B->head));
    while(1)
    {
        int random_num=rand()%num;
        
        for(int i=0;i<random_num;i++)
        {
            Pop_stack(S,p);
            
            // printf("%c  ",p.data);
            if(p.rchild){
               
                Push_stack(S,*p.rchild);
            }
                
            if(p.lchild){
               
                Push_stack(S,*p.lchild);
            }                
        }   
        if(!p.rchild)//如果没有右孩子
        {
            return &p;
        }
        if(StackEmpty(S))//如果栈空
            return NULL;
    }
    
}

BlockNode* get_tail(BlockLinkTreePtr B)
{

    return B->tail;
}



void get_random_str(char *str,int len)//生成一个随机字符串
{
    char allChar[63] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned int cnt, randNo;
	for (cnt = 0; cnt<len; cnt++)
	{
		randNo = rand() % 62;
		*str = allChar[randNo];
		str++;
	}
 
	*str = '\0';

}

// int get_block_list_length(BlockLinkTreePtr block_list){
//     BlockNodePtr p= block_list->head;
//     int i = 0;
//     while(p){
//         p = p->next;
//         i ++;
//     }
//     return i;
// }
void Query_txid(BlockLinkTreePtr T,char* txid)
{
    printf("in query...\n");
	//根据交易id 查询一个交易 非递归前序遍历
	BlockNode p;
	SqStack S;
	init_stack(S);
    if(T->head->lchild)
    {
        Push_stack(S,*(T->head->lchild));
    }
	
     printf("in query...3\n");
    printf("head---%s\n",T->head->hash);
	while(!StackEmpty(S))
	{
		Pop_stack(S,p);
        printf("###p.transcations: %s### txid: %s\n", p.transactions, txid);
        if(strstr(p.transactions,txid)!=NULL)//找到txid
        {
            printf("[find result!!!!!!!] blockheight=%dblockhash=%s\n",p.height,p.hash);
            printf("%s\n",p.transactions);
            return;
        }
		// printf("%c  ",p.data);
		if(p.rchild)
			Push_stack(S,*p.rchild);
		if(p.lchild)
			Push_stack(S,*p.lchild);
	}
	
	printf("not find this id...\n"); 
}


int main()
{

 
    srand((unsigned int)time(NULL));
    char* block_msg_queue_B="D:\\codes\\vscode_c\\shujujiegou\\MiniBlockListSystem\\data\\b\\msg_blk\\";
    char* client_msg_queue="D:\\codes\\vscode_c\\shujujiegou\\MiniBlockListSystem\\data\\d\\msg_clt\\";
    char* block_msg_queue="D:\\codes\\vscode_c\\shujujiegou\\MiniBlockListSystem\\data\\d\\msg_blk\\";
    char* block_msg_queue_C = "D:\\codes\\vscode_c\\shujujiegou\\MiniBlockListSystem\\data\\c\\msg_blk\\";
    TranListptr transaction_pool;
    transaction_pool = Init();
    
    // printf("[0.5]transcation poopl length: %d\n", transaction_pool->length); 

    BlockLinkTreePtr block_linklist;
    block_linklist=(BlockLinkTreePtr)malloc(sizeof(BlockLinkTree));
    Init_BlockList(block_linklist);
    // printf("hi!2222\n");
    double prob_award=0.5;
    int n=1;
    // printf("hi!");
    // printf("client_msg_queue:%s\n",client_msg_queue);

    // printf("[test]queue top: %s\n", get_queue_top(client_msg_queue));
    int iii = 0;
    int Tree_blk_num=0;//区块二叉树里的区块数
    while(1)
    {
        printf("_---------------------##%d--------------------------------------------\n",iii );
        double rand_val=((double)rand())/RAND_MAX;
        // rand_val=0.06;
        // printf("[0]transcation poopl length: %d\n", transaction_pool->length);

        if(rand_val<prob_award)//如果中奖
        {
            printf("award!\n");
            char *pool_datas;
            pool_datas=(char*)malloc(sizeof(char)*1000);
            pool_datas=PopN(transaction_pool,n);//从“交易池”中取出n(>=1) 个交易id
            // printf("pool_data=%s\n",pool_datas);
            if(pool_datas)
            {
                printf("pool datas not null\n");
                BlockNodePtr last_blk=random_choose(block_linklist,Tree_blk_num);
                printf("lastblk.hash=%s\n",last_blk->hash);
                int height=0;
                char prevhash[100];
                char hash[100];
                get_random_str(hash,70);//生成一个随机字符串
                if(last_blk->transactions)
                {
                    strcpy(prevhash,last_blk->hash);
                    height=last_blk->height+1;
                }
                BlockNodePtr new_blk; 
                new_blk=(BlockNodePtr)malloc(sizeof(BlockNode));
                new_blk->height=height;
                strcpy(new_blk->prevhash,prevhash);
                strcpy(new_blk->hash,hash);
                strcpy(new_blk->transactions,pool_datas);
                new_blk->transactions_num=n;
                // printf("before get_block_list_length : %d", get_block_list_length(block_linklist));
                if(last_blk->lchild==NULL)//如果度为0，作为左孩子
                {
                    last_blk->lchild=new_blk;
                }
                else//否则作为右孩子
                {
                    last_blk->rchild=new_blk;
                }
                Tree_blk_num++;//区块数加1

                // Insert(block_linklist, new_blk);//将newBLK插入本节点的区块链表末尾
                // printf("last get_block_list_length : %d", get_block_list_length(block_linklist));

                enqueue(block_msg_queue_B,new_blk);//将newBLK以字符串形式“发送”给另一个区块链节点的“区块消息队列”。
                enqueue(block_msg_queue_C,new_blk);
            }
            free(pool_datas);
        }
        else//没中奖
        {
            printf("no ward \n");
            int isempty_block = is_empty(block_msg_queue_B);
            // printf("[@@@@@@@@@@]get queue top of client:%s\n", get_queue_top(client_msg_queue));
            int isempty_client = is_empty(client_msg_queue);
            // isempty_block = is_empty(block_msg_queue);

            // printf("hi hi empties: %d %d\n", isempty_block, isempty_client); 
            // printf("hihihihih  client_msg_queue:%s\n", client_msg_queue);
            if(!isempty_block)//如果区块消息队列不空
            {
                printf("msg_block operation\n");
                char *msg=dequeue(block_msg_queue);//从“区块消息队列”头部取出一个消息（内容为区块）
                // printf("[2]msg=%s\n",msg);
                if(msg)
                {
                    BlockNodePtr preblk= find_preblk(block_linklist,Get(msg,"prevhash:"));//找到preblk
                    //如果preblk不满两个孩子
                    if(preblk->rchild==NULL)
                    {
                        printf("no conflict\n");
                        BlockNodePtr new_blk;
                        new_blk=(BlockNodePtr)malloc(sizeof(BlockNode));
                                                // printf("no conflict1\n");

                        strcpy(new_blk->hash,Get(msg,"hash:")); 
                                                // printf("no conflict2\n");

                        strcpy(new_blk->prevhash,Get(msg,"prevhash:")); 
                                                // printf("no conflict3\n");

                        new_blk->height=atoi(Get(msg,"height:")); 
                                                // printf("no conflict4\n");
                        printf("msg: %s\n", msg);
                         char* tmp=Get(msg,"transactions:");
                                                //  printf("no conflict5 tmp=%s\n", tmp);

                        strcpy(new_blk->transactions,tmp);
                        // printf("before get_block_list_length : %d", get_block_list_length(block_linklist));
 
                        if(preblk->lchild==NULL)//如果没有孩子
                        {
                            preblk->lchild=new_blk;//新区块作为preBlk的左孩子
                        }
                        else//如果有左孩子
                        {
                            preblk->rchild=new_blk;//新区块作为preBlk的右孩子
                        }
                        del_transactions(transaction_pool,new_blk);//从“交易池”中删除该区块中包含的交易
                    }
                
                    
                }
            }
            else if (!isempty_client)//客户消息队列不空
            {
                // printf("[1]transcation poopl length: %d\n", transaction_pool->length);
                printf("msg_client operation:\n");
                // printf("client_msg_queue:123213\n");

                // printf("client_msg_queue:%s", client_msg_queue);
                //从“客户消息队列”头部取出一个消息MSG
                char* msg;
                msg=dequeue(client_msg_queue);
                printf("dequeue_msg=%s\n",msg); 
                // printf("[2]transcation poopl length: %d\n", transaction_pool->length);

                if(msg)
                { 
                    char *gey_type=Get(msg,"type:");
                    printf("[main]type=%s\n",gey_type);
                    int jugge_equal=strcmp(gey_type,"transaction");
                    printf("jugge_equal=%d\n",jugge_equal);
                    // printf("[3]transcation poopl length: %d\n", transaction_pool->length);

                    if(jugge_equal==0)//如果是个交易
                    {
                        // printf("in strcmp____\n");
                        printf("do transcation\n");
                       
                        if(not_have_tid(transaction_pool,Get(msg,"txid:")))
                        {
                            // printf("in if length before: %d\n", transaction_pool->length);
                            Add(transaction_pool,msg);//将该交易加入到“交易池”
                            //看看加完了交易池有什么：
                            printf("after add length: %d\n", transaction_pool->length);
                            TranListptr hhh=transaction_pool->next;
                            while(hhh!=NULL)
                            {
                                // printf("in while\n");
                                printf("pool have--->>>%s\n",hhh->data);
                                hhh=hhh->next;
                            }
                            // printf("transaction pool add msg\n");
                        }
                        
                    }
                    else //如果是查询
                    {

                        // printf("when query last get_block_list_length : %d", get_block_list_length(block_linklist));
                        printf("msg==%s\n",msg);
                        char* txid_need_query=Get(msg,"txid:");
                        printf("the txid need query: %s\n", txid_need_query);

                        // // printf("need_query=%s\n",txid_need_query);
                        // // 在本节点维护的区块链表中执行 查询
                        Query_txid(block_linklist, txid_need_query);
                         

                    }
                }
            }
            else
            {
                printf("all empty,sleep\n");
            }
        }
        iii += 1;
        Sleep(1000);
    } 

    return 0;
}

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define N 50
#define M 2*N-1

typedef struct node
{
	char lettle;
	int num_count;
	struct node *next;
}words;

typedef struct 
{
	char data;
	int weight;
	int parent,lchild,rchild;
}HTnode;
typedef struct
{
	char cd[N];
	int start;
}HCode;

words *InitList(words *L)
{
	words *head=NULL;
	head=(words *)malloc(sizeof(words));
	if(!head)
	{
		return 0;
	}
	head->next=NULL;
	L=head;
	return L;
}

words *CreateList(words *L, char n)
{
    words *p = NULL;
	p=(words *)malloc(sizeof(words));
	p->lettle=n;p->num_count=1;
	p->next=L->next;
	L->next=p;
	return L;
}

words *InsertList(words *L, char m)
{
	words *s=NULL, *p=NULL;
	p=L;
	s=(words *)malloc(sizeof(words));
	s->lettle=m;s->num_count=1;
	s->next=p->next;
	p->next=s;
	return L;
}

int LenList(words *L)
{
	words *p=NULL;
	int count=0;
	p=L;
	while(p->next)
	{
		count++;
		p=p->next;
	}
	return count;
}

//创建哈夫曼树 
void CreateHT(HTnode ht[],int n)
{
	int  i,k,lnode,rnode,j;
	int min1,min2;
	for(i=0;i<2*n-1;i++)        //所有节点的相关域全部赋初值为-1 
	{
		ht[i].parent=ht[i].lchild=ht[i].rchild=-1;
	}
	for(i=n;i<2*n-1;i++)       //构造哈夫曼树 
	{
		min1=min2=8000000;
		lnode=rnode=-1;       //lnode,rnode为最小权重的两个节点位置 
		for(k=0;k<=i-1;k++)
		{
			if(ht[k].parent==-1)   //将最小权重的两颗树过滤，不扫描 
			{
				if(ht[k].weight<min1)
				{
					min2=min1;
					rnode=lnode;
					min1=ht[k].weight;
					lnode=k;
				}
				else if(ht[k].weight<min2)
				{
					min2=ht[k].weight;
					rnode=k;
				}
			}
		}
		ht[lnode].parent=i;
		ht[rnode].parent=i;
		ht[i].weight=ht[lnode].weight+ht[rnode].weight;
		ht[i].lchild=lnode;
		ht[i].rchild=rnode;
	}
}
//创建叶子编码 
void CreateHCode(HTnode ht[],HCode hcd[],int n)
{
	int i,f,c;
	HCode hc;
	for(i=0;i<n;i++)           //根据哈夫曼树求哈夫曼编码 
	{
		hc.start=n;
		c=i;
		f=ht[i].parent;
		while(f!=-1)
		{
			if(ht[f].lchild==c)
				hc.cd[hc.start--]='0';      //左孩子为0 
			else
				hc.cd[hc.start--]='1';      //右孩子为 1
			c=f;
			f=ht[f].parent;
		}
		hc.start++;          //start指向哈夫曼编码最开始字符 
		hcd[i]=hc;
	}
}
//输出哈夫曼编码 
void DispHCode(HTnode ht[],HCode hcd[],int n,words *L)
{
	int i,k;
	int sum=0,m=0,j;
	words *s=NULL;
	s=L;
	s=s->next;
	printf("输出哈夫曼编码：\n");
	for(i=0;i<n;i++)
	{
		printf("%c:",s->lettle);s=s->next;
		for(k=hcd[i].start;k<=n;k++)
		{
			printf("%c",hcd[i].cd[k]);
		}
		printf("\n");
	}
}
//用Huffman编码编译文件存于encode.txt中
void encoding(HTnode ht[],HCode hcd[],int n)
{
	int i,j,k;
	int m=0;
	int z=0;
	char str[1200],g[8000];
	FILE *fp,*fp1;
	fp=fopen("data.txt","r");
	fp1=fopen("encode.txt","w");
	fgets(str,1200,fp);
	str[1200]=strlwr(str);
	printf("文件编码:\n");
	for(i=0;str[i]!='\0';i++)
	{
		for(j=0;j<n;j++)
		{
			if(str[i]==ht[j].data)
			{
				for(k=hcd[j].start;k<=n;k++)
				{
					g[m]=hcd[j].cd[k];
					printf("%c",g[m]);
					m++;
				}
			}
		}
	}
	printf("\n\n****编码文件存于encode.txt中****\n");
	g[m]='\0';
	fputs(g,fp1);
	fclose(fp);
	fclose(fp1);
}
//解码
void decoding(HTnode ht[],int n)
{
	int len,k=2*n-2,i;
	int m=0;
	int z=0;
	char str[6000],g[1200];
	FILE *fp,*fp1;
	fp=fopen("encode.txt","r");
	fp1=fopen("decode.txt","w");
	fgets(str,6000,fp);
	str[6000]=strlwr(str);
	len=strlen(str);
	printf("\n\n\n");
	printf("译码为：\n");
	for(i=0;i<len;i++)
	{
		if(str[i]=='0')
		{
			k=ht[k].lchild;
			if(ht[k].lchild==-1)
			{
				printf("%c",ht[k].data);
				g[m]=ht[k].data;m++;
				k=2*n-2;
			}
		}
		else if(str[i]=='1')
		{
			k=ht[k].rchild;
			if(ht[k].rchild==-1)
			{
				printf("%c",ht[k].data);
				g[m]=ht[k].data;m++;
				k=2*n-2;
			}
		}
	}
	printf("\n\n****译码文件存于decode.txt中****\n");
	g[m]='\0';
	fputs(g,fp1);
	fclose(fp);
	fclose(fp1);
}

float ncount()
{
	float num;
	char str[1200];
	FILE *fp;
	fp=fopen("data.txt","r");
	fgets(str,1200,fp);
	num=strlen(str);
	return num;
}

void Txt_info(words *L,float num)
{
	int n=LenList(L),i=0;
	words *p=NULL;
	p=L;
	p=p->next;
	while(p)
	{	
		printf("%c:%d    频率:%f\n",p->lettle,p->num_count,p->num_count/num);
		p=p->next;
	}
	printf("\n");
}
void print_txt()
{
	char str[1200];
	FILE *fp;
	fp=fopen("data.txt","r");
	fgets(str,1200,fp);
	printf("读取文件:%s\n",str);
	fclose(fp);
}

words *Read()
{
	int num,i;
	char str[1200],*w=str;
	words *L=NULL,*p=NULL;
	FILE *fp;
	fp=fopen("data.txt","r");
	fgets(str,1200,fp);
	str[1200]=strlwr(str);
	num=strlen(str);
	L=InitList(L);
	L=CreateList(L,*w);*w++;
	p=L;p=p->next;
	for(i=1;i<num;i++)
	{
		while(*w)
		{
			if(p->lettle==*w)
			{
				p->num_count++;
				*w++;p=L;
			}
			else
				p=p->next;
			while(!p)
			{
				L=InsertList(L,*w);
				*w++;p=L;
			}
		}
	}
	return L;
	fclose(fp);
}

void huffman(words *L,float num)
{
	HTnode ht[M];
	HCode hcd[N];
	int i=0,n=23,j;
	char  *a[27];int fnum[27];
	words *p=NULL;
	p=L;
	p=p->next;
	while(p)
	{	
		a[i]=p->lettle;fnum[i]=(p->num_count/num)*1000000;
		i++;
		p=p->next;
	}
	for(j=0;j<n;j++)
	{   
	    ht[j].data=a[j];
		ht[j].weight=fnum[j];
	}
	CreateHT(ht,n);
	CreateHCode(ht,hcd,n);
	DispHCode(ht,hcd,n,L);
	encoding(ht,hcd,n);
	decoding(ht,n);
}
void comp()
{
	int i=0;
	char str[6000],g[6000];
	FILE *fp,*fp1;
	fp=fopen("data.txt","r");
	fp1=fopen("decode.txt","r");
	fgets(str,6000,fp);
	fgets(g,1200,fp1);
	str[6000]=strlwr(str);
	while(str[i]!='\0'&& g[i]!='\0')
	{
		if(str[i]!=g[i])
		{
			printf("不相等!");
			break;
		}
		else i++;
	}
	printf("相同，编码正确!");
}

void menu()
{
	printf("*****************************\n");
	printf("*      请输入你的选择       *\n");
	printf("*        1:读取文件         *\n");
	printf("*        2:文件字符统计     *\n");
	printf("*        3:哈夫曼编码及解码 *\n");
	printf("*        4:比较             *\n");
	printf("*        0:退出             *\n");
	printf("*****************************\n");
}

void main()
{   HTnode ht[M];
	int i,m;float num;
	words *L=NULL;
	num=ncount();
	menu();
	L=Read();
	do
	{
		scanf("%d",&m);
		switch(m)
		{
		case 1:print_txt();break;
		case 2:Txt_info(L,num);break;
		case 3:huffman(L,num);break;
		case 4:comp();break;
		default:printf("wrong!\n");
		}
	}while(m!=0);
}

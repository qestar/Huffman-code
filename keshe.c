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

//������������ 
void CreateHT(HTnode ht[],int n)
{
	int  i,k,lnode,rnode,j;
	int min1,min2;
	for(i=0;i<2*n-1;i++)        //���нڵ�������ȫ������ֵΪ-1 
	{
		ht[i].parent=ht[i].lchild=ht[i].rchild=-1;
	}
	for(i=n;i<2*n-1;i++)       //����������� 
	{
		min1=min2=8000000;
		lnode=rnode=-1;       //lnode,rnodeΪ��СȨ�ص������ڵ�λ�� 
		for(k=0;k<=i-1;k++)
		{
			if(ht[k].parent==-1)   //����СȨ�ص����������ˣ���ɨ�� 
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
//����Ҷ�ӱ��� 
void CreateHCode(HTnode ht[],HCode hcd[],int n)
{
	int i,f,c;
	HCode hc;
	for(i=0;i<n;i++)           //���ݹ������������������ 
	{
		hc.start=n;
		c=i;
		f=ht[i].parent;
		while(f!=-1)
		{
			if(ht[f].lchild==c)
				hc.cd[hc.start--]='0';      //����Ϊ0 
			else
				hc.cd[hc.start--]='1';      //�Һ���Ϊ 1
			c=f;
			f=ht[f].parent;
		}
		hc.start++;          //startָ������������ʼ�ַ� 
		hcd[i]=hc;
	}
}
//������������� 
void DispHCode(HTnode ht[],HCode hcd[],int n,words *L)
{
	int i,k;
	int sum=0,m=0,j;
	words *s=NULL;
	s=L;
	s=s->next;
	printf("������������룺\n");
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
//��Huffman��������ļ�����encode.txt��
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
	printf("�ļ�����:\n");
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
	printf("\n\n****�����ļ�����encode.txt��****\n");
	g[m]='\0';
	fputs(g,fp1);
	fclose(fp);
	fclose(fp1);
}
//����
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
	printf("����Ϊ��\n");
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
	printf("\n\n****�����ļ�����decode.txt��****\n");
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
		printf("%c:%d    Ƶ��:%f\n",p->lettle,p->num_count,p->num_count/num);
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
	printf("��ȡ�ļ�:%s\n",str);
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
			printf("�����!");
			break;
		}
		else i++;
	}
	printf("��ͬ��������ȷ!");
}

void menu()
{
	printf("*****************************\n");
	printf("*      ���������ѡ��       *\n");
	printf("*        1:��ȡ�ļ�         *\n");
	printf("*        2:�ļ��ַ�ͳ��     *\n");
	printf("*        3:���������뼰���� *\n");
	printf("*        4:�Ƚ�             *\n");
	printf("*        0:�˳�             *\n");
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

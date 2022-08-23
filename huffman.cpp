#include <iostream>
#include <unordered_map>
#include <fstream>
#include <ctype.h>
#include <math.h>
#include <string>
#include <algorithm>
using namespace std;
struct MinHeapNode
{
    char character;
    unsigned frequency;
    struct MinHeapNode *left;
    struct MinHeapNode *right;
};
struct MinHeap
{
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};
struct MinHeapNode* newNode(char character,unsigned frequency)
{
    struct MinHeapNode *newNode=(struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    newNode->character=character;
    newNode->frequency=frequency;
    newNode->left=NULL;
    newNode->right=NULL;
    return newNode;
};
struct MinHeap* createMinHeap(unsigned capacity)
{
    struct MinHeap *minHeap=(struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size=0;
    minHeap->capacity=capacity;
    minHeap->array=(struct MinHeapNode**)malloc(capacity*(sizeof(struct MinHeapNode*)));
    return minHeap;
};
void swapMinHeapNode(struct MinHeapNode **a,struct MinHeapNode **b)
{
    struct MinHeapNode *t=*a;
    *a=*b;
    *b=t;
}
void minHeapify(struct MinHeap *minHeap,int index)
{
    int smallest=index;
    int left=2*index+1;
    int right=2*index+2;
    if((left<minHeap->size)&&(minHeap->array[left]->frequency<minHeap->array[smallest]->frequency))
        smallest=left;
    if((right<minHeap->size)&&(minHeap->array[right]->frequency<minHeap->array[smallest]->frequency))
        smallest=right;
    if(smallest!=index)
    {
        swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[index]);
        minHeapify(minHeap,smallest);
    }
    return;
}
bool isSizeOne(struct MinHeap *minHeap)
{
    return (minHeap->size==1);
}
struct MinHeapNode* extractMin(struct MinHeap *minHeap)
{
    struct MinHeapNode *temp=minHeap->array[0];
    minHeap->array[0]=minHeap->array[minHeap->size-1];
    minHeap->size-=1;
    minHeapify(minHeap,0);
    return temp;
}
void insertMinHeap(struct MinHeap *minHeap,struct MinHeapNode *minHeapNode)
{
    minHeap->size+=1;
    int i=minHeap->size-1;
    while(i&&(minHeapNode->frequency<minHeap->array[(i-1)/2]->frequency))
    {
         minHeap->array[i]=minHeap->array[(i-1)/2];
         i=(i-1)/2;
    }
    minHeap->array[i]=minHeapNode;
    return;
}
void buildMinHeap(struct MinHeap *minHeap)
{
    int n=minHeap->size-1;
    for(int i=(n-1)/2;i>=0;i--)
    {
         minHeapify(minHeap,i);
    }
    return;
}
struct MinHeap* createAndBuildMinHeap(unordered_map<char,int> &frequency_count)
{
    int size=frequency_count.size();
    struct MinHeap *minHeap=createMinHeap(size);
    auto it=frequency_count.begin();
    for(int i=0;i<size;i++,it++)
    {
        minHeap->array[i]=newNode(it->first,it->second);
    }
    minHeap->size=size;
    buildMinHeap(minHeap);
    return minHeap;
}
struct MinHeapNode* buildHuffmanTree(unordered_map<char,int> &frequency_count)
{
    struct MinHeapNode *left;
    struct MinHeapNode *right;
    struct MinHeapNode *top;
    struct MinHeap *minHeap=createAndBuildMinHeap(frequency_count);
    while(!isSizeOne(minHeap))
    {
        left=extractMin(minHeap);
        right=extractMin(minHeap);
        top=newNode('$',left->frequency+right->frequency);
        top->left=left;
        top->right=right;
        insertMinHeap(minHeap,top);
    }
    return extractMin(minHeap);
}
struct MinHeapNode* HuffmanCodes(unordered_map<char,int> &frequency_count)
{
    struct MinHeapNode *root=buildHuffmanTree(frequency_count);
    return root;
}
ifstream::pos_type filesize(const char *filename)
{
    ifstream in(filename,ifstream::ate|ifstream::binary);
    return in.tellg();
}
void HuffmanTraverse(struct MinHeapNode *root,string code,unordered_map<char,string> &bit_codes)
{
    if(root!=NULL)
    {
        HuffmanTraverse(root->left,code+"0",bit_codes);
        if(root->left==NULL&&root->right==NULL)
        {
            bit_codes[root->character]=code;
        }
        HuffmanTraverse(root->right,code+"1",bit_codes);
    }
    return;
}
string binaryconvert(int ascii_value,int flag)
{
    string ans="";
    do
    {
        ans+=((ascii_value)%2)+48;
        ascii_value/=2;
    }while(ascii_value>0);
    int t=ans.size();
    for(int i=0;i<(flag-t);i++)
        ans+="0";
    reverse(ans.begin(),ans.end());
    return ans;
}
int main()
{
    unordered_map<char,int> frequency_count;
    fstream fs{"input.txt"};
    char file_character;
    while(fs>>noskipws>>file_character)
    {
        frequency_count[file_character]++;
    }
    struct MinHeapNode *root=HuffmanCodes(frequency_count);
    unordered_map<char,string> bit_codes;
    HuffmanTraverse(root,"",bit_codes);
    ifstream fin;
    ofstream fout;
    fin.open("input.txt",ios::in);
    fout.open("output.txt",ios::out);
    char outfile_character;
    char temp_character;
    string temp="";
    int count=0;
    int last_num;
    while(fin)
    {
        int i=0;
        if(fin.get(outfile_character))
        {
            auto it=bit_codes.find(outfile_character);
            string x=it->second;
            int temp_length=temp.size();
            int required_length=8-temp_length;
            if(x.size()>=required_length)
            {
                temp+=x.substr(i,required_length);
                i+=required_length;
                int ascii_value=0;
                //cout<<temp;
                for(int j=7;j>=0;j--)
                    ascii_value+=(temp[j]-48)*pow(2,(7-j));
                //cout<<" "<<ascii_value<<"\n";
                temp_character=(char)ascii_value;
                fout.put(temp_character);
                count++;
                temp.clear();
                while(x.size()-i>=8)
                {
                    temp+=x.substr(i,8);
                    i+=8;
                    //cout<<temp;
                    for(int j=7;j>=0;j--)
                        ascii_value+=(temp[j]-48)*pow(2,(7-j));
                    //cout<<" "<<ascii_value<<"\n";
                    temp_character=(char)ascii_value;
                    fout.put(temp_character);
                    count++;
                    temp.clear();
                }
            }   
            temp+=x.substr(i,x.size()-i);
        }
    }
    int ascii_value=0;
    //cout<<temp;
    for(int i=temp.size()-1;i>=0;i--)
        ascii_value+=(temp[i]-48)*pow(2,(temp.size()-1-i));
    last_num=temp.size();
    //cout<<" "<<ascii_value<<"\n";
    temp_character=(char)ascii_value;
    fout.put(temp_character);
    count++;
    fin.close();
    fout.close();
    cout<<"Input File Size : "<<filesize("input.txt")<<" bytes"<<endl;
    cout<<"Compressed File Size : "<<filesize("output.txt")<<" bytes"<<endl;
    cout<<"Compression Ratio : "<<(1.0*filesize("output.txt")/filesize("input.txt"))<<endl;
    fin.open("output.txt",ios::in);
    fout.open("output1.txt",ios::out);
    struct MinHeapNode *ptr=root;
    int scount=0;
    while(fin)
    {
        if(fin.get(outfile_character))
        {
             int ascii_value=(int)outfile_character;
             if(ascii_value<0)
                 ascii_value+=256;
             //cout<<ascii_value<<" ";
             string x;
             if(scount+1==count)
                 x=binaryconvert(ascii_value,last_num);
             else
                 x=binaryconvert(ascii_value,8);
             //cout<<x<<"\n";
             for(int i=0;i<x.size();i++)
             {
                  if(ptr->left==NULL&&ptr->right==NULL)
                  {
                      fout.put(ptr->character);
                      ptr=root;
                  }
                  if(x[i]=='0')
                  {
                      ptr=ptr->left;
                  }
                  else
                  {
                      ptr=ptr->right;
                  }
             }
             if(ptr->left==NULL&&ptr->right==NULL)
             {
                 fout.put(ptr->character);
                 ptr=root;
             }
             scount++;
        }
    }
    fin.close();
    fout.close();
    cout<<"Final Output File Size : "<<filesize("output1.txt")<<" bytes"<<endl;
    return 0;
}

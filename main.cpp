#include<iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;

class Node
{
public:
    int StartingIndex;
    int Length;
    int stIndex;
    int insetindx=0;
    //pointer array to other node via suffix link
    Node * children [26];

//initialize variables and children of the node
    Node(int startingIndex, int length)
    {
        StartingIndex =startingIndex;
        Length = length;
        for(int i=0; i<26; i++)
        {
            children[i]=NULL;
        }

    }
//function to insert new child O(1)
    void insert(Node *child)
    {
        this->children[insetindx]=child;
        insetindx++;
    }

};

class SuffixTree
{
public:
    char*  Arr;
    int Size;
    Node *root= new Node(-1,-1) ;
    //initialize the string and construct the tree
    SuffixTree(char arr[])
    {
        int si=0;
        for (int i = 0; ; ++i)
        {
            si++;
            if(arr[i]=='$')break;
        }

        Size=si;
        root->stIndex=-1;
        Arr = new char [Size];

        for(int i=0; i<Size; i++)
        {
            Arr[i] = *(arr+i);
        }
        construct();
    }


    //function to construct the tree
    void construct()
    {
        //loop on the string
        for(int i=0;i<Size;i++)
        {
            traverse(root,i);
        }
    }

    // function to get the root
    Node *getRoot() const
    {
        return root;
    }

    // function to set the root
    void setRoot(Node *root)
    {
        SuffixTree::root = root;
    }
    //function to get the string
    char *getArr() const
    {
        return Arr;
    }
    //function to set the string
    void setArr(char *arr)
    {
        Arr = arr;
    }

    void subMatching(Node *parent, Node *node, int indx, int count, int childIndx)//function to handle sub matching case
    {
        if(node->insetindx==0)//check if the node is leaf
        {
            Node *node1= new Node(count+node->StartingIndex, node->Length-count);
            Node *node2= new Node(indx+count,Size-(indx+count));
            node1->stIndex=node1->StartingIndex;
            node2->stIndex=node2->StartingIndex;
            node->Length= (count);
            node->stIndex=-1;
            node->insert(node1);
            node->insert(node2);

        }
        else//if the node is not a leaf the parent node will have new level
        {
            Node *temp=new Node(node->StartingIndex,count);
            node->StartingIndex= count+node->StartingIndex,node->Length=node->Length-count;
            temp->insert(node);
            temp->stIndex=-1;
            Node *node2= new Node(indx+count,Size-(indx+count));
            node2->stIndex=node2->StartingIndex;
            temp->insert(node2);
            parent->children[childIndx]=temp;


        }
    }
    void fullMatching(Node *node, int indx, int count)//function to handle full matching case
    {
        if(node->insetindx==0)//check if the node is leaf
        {
            Node *newNode=new Node(count+indx, Size-(count+indx));
            node->stIndex=-1;
            newNode->stIndex=newNode->StartingIndex;
            node->insert(newNode);
        }
        else
        {
            traverse(node,indx+count);// if the node is not leaf we will travers the next level
        }
    }

    int checkMatching(Node *node, int indx)//function to find matching characters and return its length
    {
        int count=0;
        for(int i =node->StartingIndex; i<= (node->StartingIndex+node->Length)-1; i++ )
        {
            if(Arr[indx]!=Arr[i])
                break;
            count++;
            indx++;
        }
        return count;
    }
    void traverse(Node *node, int indx)
    {
        //traverse on the level to find matching node
        for(int i=0; i<26 ; i++)
        {
            if(node->children[i]==NULL)
            {
                Node *temp= new Node (indx,Size-indx);
                temp->stIndex=temp->StartingIndex;
                node->insert(temp);
                node->stIndex=-1;
                return;
            }
            int count = checkMatching(node->children[i],indx);

            //check if the is matching
            if(count !=0)
            {
                if(count==node->children[i]->Length)
                {
                    fullMatching(node->children[i],indx,count);
                }
                else
                {
                    subMatching(node,node->children[i],indx,count,i);
                }


                return;
            }
        }
    }
    void Search(char s[]) //ana
    {

        int  sizes = strlen(s);

        bool flag = false;

        for(int i=0;i<26;i++)
        {
            int j = 0;

            //traverse as long as root's children aren't null
            if(root->children[i]!=NULL) //
            {
                //if starting index character at child == s[j] character
                if(Arr[root->children[i]->StartingIndex]==s[j])
                {
                    flag = true;
                    checkEquality(s,j,root->children[i]->StartingIndex,root->children[i],sizes);
                }
            }
        }

        if (!flag)
        {
            cout << "No match found" << " ";
        }
    }

    //function to check the number of matches between substring and node string
    void checkEquality(char s[],int j,int tempK,Node* node,int sizes)
    {
        bool flag = false;

        int sumLength = 0;
        int count=0;

        //loop to find count of matches
        while(Arr[tempK] == s[j] && count < node->Length)
        {
            tempK++;
            j++;
            count++;
        }

        //if node string fully matched with a portion of the substring
        if(count == node->Length && sizes > count)
        {

            //add node length to substring sum length
            flag = true;

            sumLength += node->Length;
            //we go one level deeper in the tree
            goDeeper(s, j, tempK, node, sumLength, sizes);
        }

        //if string is fully matched with the all the node's string
        else if(count<=node->Length && sizes == count)
        {
            flag=true;
            //check if node has no children
            if(node->insetindx==0)
            {
                cout<<node->StartingIndex - sumLength;
            }
            else
            {
                sumLength+=node->Length;

                for(int i=0;i<26;i++)
                {
                    if(node->children[i]!=NULL)
                    {
                        // we traverse through the tree until we get to the leaves
                        toleafs(node->children[i], sumLength);
                    }
                }
            }
        }

        if (!flag)
        {
            cout << "No match found" << " ";
        }

    }

    // function to go one level down in the tree each time it's called, and traverse the node's children
    void goDeeper(string s,int j,int tempK,Node *node,int sumLength,int sizes)
    {

        //if node we resided on has no children
        if(node->insetindx == 0)
        {
            cout << Size - sumLength << " ";
        }

        else
        {
            bool flag = false;
            //looping through the children of this node
            for(int i=0;i<26;i++)
            {
                if(node->children[i]!=NULL)
                {
                    //if match is found with the children of the node
                    if(Arr[node->children[i]->StartingIndex] == s[j])
                    {
                        flag=true;
                        //the current node
                        Node *tempNode = node->children[i];

                        //starting index of the node
                        tempK = tempNode->StartingIndex;

                        //count for the number of matches found
                        int count=0;

                        //getting the matching count
                        while(Arr[tempK] == s[j] && count < tempNode->Length)
                        {
                            tempK++;
                            j++;
                            count++;
                        }

                        //if substring is fully matched with each character of the node string
                        if(count == tempNode->Length && j == sizes)
                        {
                            sumLength += tempNode->Length;

                            //if node is a leaf node
                            if (tempNode->insetindx == 0)
                            {
                                cout << Size - sumLength << " ";

                            }
                            else {

                                //we traverse to the leaf of the tree
                                for(int i=0;i<26;i++)
                                {
                                    if(tempNode->children[i]!=NULL)
                                    {
                                        toleafs(tempNode->children[i], sumLength);
                                    }
                                }
                            }
                        }
                            //if substring is fully matched but with characters of the node remaining
                        else if(j == sizes && count != tempNode->Length)
                        {
                            sumLength += tempNode->Length;

                            //if node is a leaf node
                            if (tempNode->insetindx == 0)
                            {
                                cout << Size - sumLength << " ";

                            }

                            else {
                                //we traverse to the leaf of the tree
                                for(int i=0;i<26;i++)
                                {
                                    if(tempNode->children[i]!=NULL)
                                    {
                                        toleafs(tempNode->children[i], sumLength);
                                    }
                                }
                            }
                        }

                        //if there's still matching to do --> we go one level deeper
                        else if(count == tempNode->Length && j < sizes)
                        {
                            sumLength += tempNode->Length;
                            goDeeper(s,j,tempK,tempNode,sumLength,sizes);
                            return;
                        }
                    }
                }
            }
            if(!flag)
            {
                cout<<"No match found";
                return;

            }
        }
    }

    // a function to traverse through each level of the tree until it gets to the leaf
    void toleafs(Node * node,int sumLength)
    {
        //check if the current function is a leaf
        if(node->insetindx == 0)
        {
            cout << node->StartingIndex - sumLength << " ";
        }
        else
        {
            sumLength += node->Length;

            for(int i=0;i<26;i++)
            {
                //go one level deeper and fet to the children of the current node
                if(node->children[i]!=NULL)
                {
                    toleafs(node->children[i],sumLength);
                }
            }

        }
    }
    friend class Node;
};


int main()
{
    char sr[] = {"minimize$"};

    SuffixTree tree(sr);
    tree.Search("m");
    cout<<endl;
    tree.Search("i");
    cout<<endl;
    tree.Search("n");
    cout<<endl;
    tree.Search("z");
    cout<<endl;
    tree.Search("e");
    cout<<endl;
    tree.Search("mi");
    cout<<endl;
    tree.Search("min");
    cout<<endl;
    tree.Search("mini");
    cout<<endl;
    tree.Search("minim");
    cout<<endl;
    tree.Search("minimi");
    cout<<endl;
    tree.Search("minimiz");
    cout<<endl;
    tree.Search("minimize");
    cout<<endl;
    tree.Search("ni");
    cout<<endl;
    tree.Search("nim");
    cout<<endl;
    tree.Search("nimi");
    cout<<endl;
    tree.Search("nimiz");
    cout<<endl;
    tree.Search("im");
    cout<<endl;
    tree.Search("z");
    cout<<endl;
    tree.Search("ze");
    cout<<endl;
    tree.Search("ze$");
    cout<<endl;

    return 0;
}

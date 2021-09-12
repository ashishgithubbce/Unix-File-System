#include <iostream>
using namespace std;
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
struct inode
{
    char name[8];
    int size;
    int blockPointers[8];
    int used;
};
struct blockIndex
{
    char block[1024];
};
void createFile(char* fileName, int size, char *freeBlockList, inode *inodeptr, blockIndex *dataBlock){
    for(int i=0;i<16;i++){
          if( (inodeptr+i)->used ==0){
                  (inodeptr + i)->size = size;//updating status of inode
                  (inodeptr + i)->used = 1;
                  int j;
                  for ( j = 0; *(fileName + j); j++)
                           (inodeptr + i)->name[j] = fileName[j];//assigning name to empty inode
                  (inodeptr + i)->name[j] = '\0';
                  //allocating Block space
                  for (int j = 0; j < size; j++){
                           for (int k = 1; k < 128; k++){
                                 if (*(freeBlockList + k) == '0'){
                                            (inodeptr + i)->blockPointers[j] = k;
                                            *(freeBlockList + k) = '1';
                                             break;
                                 }
                           }
                }
       break;//break once a empty inode found
     }
    }
return;
}
void deleteFile(char * fileName, char *freeBlockList, inode *inodePtr){
     for(int i=0;i<16;i++){
          int flag = strcmp((inodePtr+i)->name, fileName);
          cout<<"flag:"<<flag<<endl;
            if(flag==0){
                    int size = (inodePtr+i)->size;
                    for(int j=0;j<size;j++){
                          int index = (inodePtr +i)->blockPointers[j];
                          *(freeBlockList + index) = '0';
                          (inodePtr +i)->blockPointers[j]=0;
                   }
                   (inodePtr +i)->used = 0;
                    break;
          }
    }
}
void readFile(char* fileName, int blockNum, char *buffer, inode *inodePtr, blockIndex *dataBlock){
          int index,flag;
          for(int i=0;i<16;i++){
                     flag = strcmp((inodePtr +i)->name,fileName);
                     if(flag ==0){
                           index = i;
                           break;
                      }
           }
           if(flag!=0)
               cout<<fileName<<" file Does not Exist"<<endl;
           blockIndex *blockPtr  = (dataBlock + ((inodePtr+ index)->blockPointers[blockNum-1] -1));
           int j;
           for( j=0;*((char*)blockPtr +j);j++)//reading from File
                      buffer[j]= *((char *)blockPtr+j);
           buffer[j]= '\0';
return;
}
void  writeIntoFile(char* fileName, int block, char *buffer, inode *inodePtr, char *freeBlockList, blockIndex *dataBlock){
          int index,flag;
          for(int i=0;i<16;i++){
                        flag = strcmp((inodePtr +i)->name,fileName);
                        if(flag ==0){
                             index = i;
                        break;
                       }
          }
          if(flag!=0)
                 cout<<fileName<<" file Does not Exist"<<endl;
          if ((inodePtr + index)->size < block){
                      cout << "Size of file is " << (inodePtr + index)->size << "you can't write to the file choose block number less than or equal to" << (inodePtr + index)->size <<endl;
          }
          blockIndex *ptr = dataBlock + (((inodePtr + index)->blockPointers[block - 1]) - 1);
          int j;
          for(j=0;buffer[j];j++){//Writing into file
                     *((char *)ptr +j) = buffer[j];
          }
 //cout<<"buffer:"<<buffer<<endl;
 return;
}
void  fileSystemStatus(inode *inodePtr){
    for (int i = 0; i < 16; i++)
    {
        if ((inodePtr + i)->used == 1)
        {
              cout << "File name: " << (inodePtr + i)->name << std::endl;
              cout << "File size: " << (inodePtr + i)->size << std::endl;
        }
    }
    return;
}
int main()
{
        //void *diskSpace = malloc(sizeof(char)*128*1024);
        void *diskSpace = new char[128 * 1024];
        if (diskSpace==NULL){
                  cout << "Memory allocation failed" << endl;
        }
        char *freeBlockList = (char *)diskSpace;
        inode *inodePtr = (inode *)((char *)diskSpace + 128);
        blockIndex *dataBlock = (blockIndex *)((char *)diskSpace + 1024);//to get the starting index of data block
        *(freeBlockList) = '1';
        for(int i=0;i<16;i++){//initializing of Inode
             (inodePtr +i)->used = 0;
             for(int j=0;j<8;j++)
                         (inodePtr+i)->blockPointers[j] = 0;//initilizing each inode's block pointer to 0;
        }
        for(int i=1;i<128;i++)//initializing of block pointer
                  *(freeBlockList +i) = '0';
        int option;
        char decision;
        char buffer[1024];
        do{
        cout<<"1.Create File"<<endl;
        cout<<"2.Delete File"<<endl;
        cout<<"3.Read File"<<endl;
        cout<<"4.Write File"<<endl;
        cout<<"5.File System status"<<endl;
        cout<<"Choose Option:";
        cin>>option;
        switch(option){

        case 1:
        {
            char fileName[8];
            int size;
            cout <<"Enter file name:";
            cin >> fileName;
            cout << "Enter size of file: ";
            cin >> size;
            createFile(fileName, size, freeBlockList, inodePtr, dataBlock);
            break;
        }
        case 2:
        {
            char fileName[8];
            cout << "Enter name of file: ";
            cin >> fileName;
            deleteFile(fileName, freeBlockList, inodePtr);
            cout<<"File Deleted"<<endl;
            break;
        }

        case 3:
        {
            char fileName[8];
            int blockNum;
            char buffer[1024];

            cout << "Enter file name: ";
            cin >> fileName;

            cout << "Enter block number that you want to read: ";
            cin >> blockNum;

            readFile(fileName, blockNum, buffer, inodePtr, dataBlock);

            char c;
            int i = 0;
            while ((c = buffer[i++]) != '\0')
            {
                putchar(c);
            }
            memset(buffer,0,sizeof(buffer));
            break;
        }

        case 4:
        {
            char fileName[8];
            int block;
            cout << "Enter file name: ";
            cin >> fileName;

            cout << "Enter block number: ";
            cin >> block;

            cout << "Enter Data:";

            int i = 0;
            char c = getchar();
            while ((c = getchar()) != '\n')
            {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            writeIntoFile(fileName, block, buffer, inodePtr, freeBlockList, dataBlock);
            break;
        }

        case 5:
        {
            fileSystemStatus(inodePtr);
            break;
        }
        default:
            cout<<"Invalid Choise"<<endl;
        }
        cout<<endl<<"Do you want to continue(y/n)"<<endl;
        cin>>decision;
    }while(decision== 'y');

}

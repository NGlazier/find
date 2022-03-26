#include<iostream>
#include "find.h"
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>





using namespace std;
int debug = 0;
int name = 0;
int type = 0;
int size = 0;
string filename;
string filetype;
string sizespec;
bool minussize;
bool setequal = false;
string directory2;
struct stat statbuf;
bool c = false;


int main(int argc, char *argv[]){
    if(argc < 2){
        cout << "ERROR: program is called as: " << endl;
        cout << "./find directory [filters]"<<endl;
        exit(0);
    }
    const char* directory = argv[1];
    for(int i = 0; i < argc; i++){
        if(debug){
        cout << "argument " << i << ": " << argv[i] << endl;}
        if(strcmp(argv[i], "-d") == 0){
            debug = 1;
        }
        if(strcmp(argv[i], "-name") == 0){
            filename = argv[i+1];
            name = 1;
        }
        if(strcmp(argv[i], "-type") == 0){
            filetype = argv[i+1];
            type = 1;
        }
        if(strcmp(argv[i], "-size") == 0){
            sizespec = argv[i+1];
            size = 1;
            if(sizespec[0] == '-'){
                minussize = true;
                sizespec.erase(0, 1);
            }else{
                if(sizespec[0] == '+'){
                    minussize = false;
                    sizespec.erase(0, 1);
                }else{
                    setequal = true;
                }
            }
            if(sizespec[sizespec.size() - 1] == 'c'){
            sizespec.erase(sizespec.size() - 1);
            c = true;
           
        }
        }
        
    }
    if(passfilter(directory, directory)){
    printf("%s\n", directory);}
    dofind(directory, directory);
    if(debug){
        cout << directory << endl;
    }
    return 0;
}


  //recursively view all files & subdirectories within a directory
void dofind(const char* directory, const char* fullpath){
    char path[1000];
    char fullpathname[1000];
    struct dirent *dp;
    DIR *dir = opendir(directory);
    
    // Unable to open directory
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            
            strcpy(fullpathname, fullpath);
            strcat(fullpathname, "/");
            strcat(fullpathname, dp->d_name);
            if(passfilter(fullpathname, dp->d_name)){
            printf("%s\n", fullpathname);
            }
            strcpy(path, directory);
            strcat(path, "/");
            strcat(path, dp->d_name);

            dofind(path, fullpathname);
        }
    }

    closedir(dir);
}

bool passfilter(const char* fullpathname, const char* pathname){
    bool pass = true;
    
    if(type == 1 && filetype == "l"){
        lstat(fullpathname, &statbuf);
    }else{
        stat(fullpathname, &statbuf);
    }
    int ftype;
    ftype = statbuf.st_mode & S_IFMT;
    if(type == 1){
        if(filetype == "f" && ftype != S_IFREG){
            pass = false;
        }if(filetype == "d" && ftype != S_IFDIR){
            pass = false;
        }if(filetype == "l" && ftype != S_IFLNK){
            pass = false;
        }if(filetype == "b" && ftype != S_IFBLK){
            pass = false;
        }if(filetype == "c" && ftype != S_IFCHR){
            pass = false;
        }
    }
    if(name == 1){
        if(filename != pathname){
            pass = false;
        }
    }
    if(size == 1){
        if(minussize){
            if(c){
                
            
                if(statbuf.st_size >= stoi(sizespec)){
                    pass = false;
                }
            }else{
                int blocks = statbuf.st_size / 512;
                if(statbuf.st_size % 512 != 0){
                    blocks++;
                }
                if(blocks >= stoi(sizespec)){
                    pass = false;
                }
            }
        }else{
            if(minussize == false){
                if(c){
                
                
                if(statbuf.st_size <= stoi(sizespec)){
                    pass = false;
                }
            }else{
                 int blocks = statbuf.st_size / 512;
                if(statbuf.st_size % 512 != 0){
                    blocks++;
                }
                if(blocks <= stoi(sizespec)){
                    pass = false;
                }
            }
            }else{
                if(c){
                    
                    if(statbuf.st_size != stoi(sizespec)){
                        pass = false;
                    }
                }else{
                     int blocks = statbuf.st_size / 512;
                if(statbuf.st_size % 512 != 0){
                    blocks++;
                }
                if(blocks != stoi(sizespec)){
                    pass = false;
                }
                }
            }
        }
    }
    
    return pass;
}


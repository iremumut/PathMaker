#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

bool dir_path_exists(char *line,int i,char *dircopy,char *directory,DIR *dr){
    int a=0;
    bool err = false;
    bool folderfound = false;
    char *name = (char*)calloc(150,sizeof(char));
    struct dirent *cd;

    if(line[i] == '*'){
            char *dirbefore = directory;
            getcwd(dircopy,150);
            chdir("..");
            getcwd(dircopy,150);
            char *dirafter = dircopy;
            if(!strcasecmp(dirafter,dirbefore)){
                return false;
            }else{
                chdir(directory);
                getcwd(dircopy,150);
                return true;
            }

    }else{
        while(isalnum(line[i]) || isdigit(line[i]) || line[i] == '_'){
            name[a] = line[i];
            i++;
            a++;
        }

        while(isspace(line[i])){
            i++;
        }

        if(isalnum(line[i])){
                err = true;
            }

        if(err){
            printf("ERROR! Blank characters are not allowed in Directory names.\n");
            while(!isspace(line[i])){
            i++;
            }
            return false;
        }


        dr = opendir(".");
        while ((cd = readdir(dr)) != NULL){
                if(!strcasecmp(name,cd->d_name)){
                       folderfound = true;
                }
            }
        return folderfound;
    }

}




bool control(char *line,DIR *dr,char *directory,char *dircopy,bool calistir){
    dr = opendir(".");
    struct dirent *currentdir;
    int uzn = strlen(line);
    int i=0;
    bool teksatir = false;
    int err = true;

    while(isspace(line[i])){
          i++;
          }
    if(line[i] == '{' && !calistir){
        i = uzn+1;
    }

    for(i;i<uzn;i++){

    if(line[i] == '}' && !calistir){
            calistir = true;
    }

    if(line[i] == 'm' && line[i+1] == 'a' && line[i+2] == 'k' && line[i+3] == 'e' && calistir ){
        int c = i+4;
        i = make(line,c,uzn,dr,directory,dircopy)-1;
    }

    if(line[i] == 'g' && line[i+1] == 'o' && calistir){
        i+=2;
        i = go(line,i,uzn,dr,directory)-1;

    }

    if(line[i] == 'i' && line[i+1] == 'f' && calistir && line[i+2] != 'n'){
        i+=2;
        bool err = true;
        while(line[i]!= '>'){

            if(line[i] == '<'){
                braceterr(line,i,uzn);
                err = false;
            }
            if(line[i] == '/'){
                ayrik(line,i);
            }

            if (isalpha(line[i]) || line[i] == '*'){
                calistir = dir_path_exists(line,i,dircopy,directory,dr);

                while(isalnum(line[i]) || line[i] == '*'){
                    i++;
                }
                i--;
            }


            i++;
        }


        if(err){
            printf("ERROR! if command should always start with '<' operator.\n");
        }

        i++;
        while(isspace(line[i])){
            i++;
        }


        if(line[i] != '{' && !calistir && i<uzn){
            calistir = true;
            i = uzn;
        }else i--;


    }


    if(line[i] == 'i' && line[i+1] == 'f' && line[i+2] == 'n'&& line[i+3] == 'o' && line[i+4] == 't' && calistir){
        i+=5;
        bool err = true;
        while(line[i]!= '>'){

            if(line[i] == '<'){
                braceterr(line,i,uzn);
                err = false;
            }
            if(line[i] == '/'){
                ayrik(line,i);
            }

            if (isalpha(line[i]) || line[i] == '*'){
                calistir = !dir_path_exists(line,i,dircopy,directory,dr);

                while(isalnum(line[i]) || line[i] == '*'){
                    i++;
                }
                i--;
            }


            i++;
        }


        if(err){
            printf("ERROR! ifnot command should always start with '<' operator.\n");
        }

        i++;
        while(isspace(line[i])){
            i++;
        }


        if(line[i] != '{' && !calistir && i<uzn){
            calistir = true;
            i = uzn;
        }else i--;

    }



}


    //printf("%s\n",directory);
    return calistir;
}











int go (char *line,int i,int uzn,DIR *dr,char *directory){
    char *dircheck = (char*)calloc(150,sizeof(char));

    int a;
    bool err,err1 = true;


    for(i;i<uzn;i++){
        if(line[i] == '<'){
            err = false;
            braceterr(line,i,uzn);
        }

        if(line[i] == '*'){
            yildiz(line,directory,i,uzn);

        }

        if(line[i] == '/'){
            ayrik(line,i);
        }

        if(isalpha(line[i])){
            i = klasorsecme(line,i,dr,directory)-1;
        }

        if(line[i] == ';'){
            err1 = false;
            a = i;
        }



    }

    if(err){
        printf("ERROR! go command should always start with '<' operator.\n");
        return a;
    }

    if(err1){
        printf("ERROR! 'make' command require an end of line character ';' .\n");
        return i;
    }

     return i;

}








int make(char *line,int i,int uzn,DIR *dr,char *directory,char *dircopy){

    int b=0;
    int c=0;

    bool err1 = true;
    bool err2 = true;

    for(i;i<uzn;i++){

        if(line[i]== '<'){
            err2 = false;
            braceterr(line,i,uzn);
        }

        if(line[i] == '/'){
            ayrik(line,i);
        }

        if(line[i] == '*'){
            yildiz(line,dircopy,i,uzn);
            //c++;
        }

        if(isalpha(line[i])){
            b++;
            i = makedir(line,i,dircopy,dr)-1;

        }

        if( line[i] == ';'){
            err1 = false;
            break;
        }
    }

    if(err2){
        printf("ERROR! make command should always start with '<' operator.\n");
    }


    getcwd(dircopy,150);
    chdir(directory);
    getcwd(dircopy,150);


    if(!err1){
        return i;
    }else{
        printf("ERROR! 'make' command require an end of line character ';' .\n");
        return i;
    }


}


int makedir(char *line,int i,char *dircopy,DIR *dr){
    char *name = (char*)calloc(150,sizeof(char));
    int a=0;
    bool err = false;
    struct dirent *cd;

    while(isalnum(line[i])){
        name[a] = line[i];
        a++;
        i++;
    }

     while(isspace(line[i])){
        i++;
    }

    if(isalnum(line[i])){
            err = true;
        }

    if(err){
        printf("ERROR! Blank characters are not allowed in Directory names.\n");
        while(!isspace(line[i])){
        i++;
    }
        return i;
    }


    getcwd(dircopy,150);

    dr = opendir(".");
    while ((cd = readdir(dr)) != NULL){

        if(!strcasecmp(name,cd->d_name)){
            printf("A folder named '%s' already exists in this directory.\n",name);
        }
    }

    mkdir(name);
    chdir(name);
    getcwd(dircopy,150);

    return i;

}


void braceterr(char *line,int i,int uzn){
    bool err = true;

    for(i;i<uzn;i++){
        if(line[i] == '>'){
            err = false;
        }
    }

    if(err){
        printf("ERROR! '<' operator should always be followed with '>' operator.\n");
    }
}


void ayrik(char *line,int i){
    bool err = true;
    bool err1 = true;
    int a = i-1;
    int b = i+1;
    while(line[a] != '<'){
        if(!isspace(line[a])){
            err = false;
        }
        a--;
    }

     while(line[b] != '>'){
        if(!isspace(line[b])){
            err1 = false;
        }
        b++;
    }

    if(err || err1){
        printf("ERROR! Operator / cannot be used at the beginning or the end of any path.\n");
    }

}


int klasorsecme(char *line,int i,DIR *dr,char *directory){
    int a=0;
    bool err = false;
    bool folderfound = false;
    char *name = (char*)calloc(150,sizeof(char));
    struct dirent *cd;

    while(isalnum(line[i]) || isdigit(line[i]) || line[i] == '_'){
        name[a] = line[i];
        i++;
        a++;
    }

    while(isspace(line[i])){
        i++;
    }

    if(isalnum(line[i])){
            err = true;
        }

    if(err){
        printf("ERROR! Blank characters are not allowed in Directory names.\n");
        while(!isspace(line[i])){
        i++;
    }
        return i;
    }

    dr = opendir(".");
    while ((cd = readdir(dr)) != NULL){
            //printf("%s\n",cd->d_name);
            if(!strcasecmp(name,cd->d_name)){
                   folderfound = true;
                   chdir(name);
                   getcwd(directory,150);
                   dr = opendir(".");
            }
        }

    return i;
}



void yildiz(char *line,char *directory,int i,int uzn){

    bool err = false;
    while(line[i] != '<'){
        if(isalnum(line[i])){
            err = true;
        }
        i--;
    }

    if(!err){
        chdir("..");
        getcwd(directory,150);
    }else{
        printf("ERROR! Operator * can only be used at the beginning of path expressions.\n");
    }
}


int main()
{
    char fname[25];
    char fname1[9] = ".pmk.txt";
    printf("Please enter the source file name.\n");
    scanf("%s",&fname);
    strcat(fname,fname1);

    FILE *fptr = fopen(fname,"r");
     if (fptr == NULL){
        printf("File not found");
        exit(1);
    }else{
        DIR *dr = opendir(".");
        struct dirent *currentdir;
         if (currentdir == NULL)  {
        printf("Could not open current directory" );
        return 0;
    }
        bool calistir = true;
        char *directory = (char*)calloc(150,sizeof(char));
        char *directorycopy = (char*)calloc(150,sizeof(char));
        getcwd(directory,150);
        getcwd(directorycopy,150);

         while(!feof(fptr)){
            char *line = (char*)calloc(150,sizeof(char));
            fgets(line,150,fptr);
            calistir = control(line,dr,directory,directorycopy,calistir);
            free(line);

        }

        fclose(fptr);
        closedir(dr);
    }


    return 0;
}

#include "functions.h"
#include "headers.h"

int roundNo(float num) 
{ 
    return num < 0 ? num - 0.5 : num + 0.5; 
} 


int lexi_sort(const struct dirent **a ,const struct dirent **b)
{   int p= strcasecmp( (*a)->d_name,(*b)->d_name);
    if(p>0)
     return 1;
    else if(p<0)
     return -1;
    else 
     return 0;
}

char * time_as_req(time_t tmp)
{  char *buff = malloc(100);
   struct tm * tm_tmp = localtime(&tmp);
 
strftime(buff,100,"%b %d %H:%M",tm_tmp);
return buff;
}


int check_fordot(const struct dirent *a)
{     if(strncmp(a->d_name,".",1)==0)
        return 0;
      else
       return 1;

}

char * path(char * parent_path,char *file_name)
{    char * absol_path = malloc(100*sizeof(char));
     if(file_name[0]=='/')
     return file_name;
     else
    { strcpy(absol_path,parent_path);
     strcat(absol_path,"/");
     strcat(absol_path,file_name);
     return absol_path;
    }
}

void display_short(struct dirent **dir_list,int cnt,char *parent_path)
{    for (int i=0;i<cnt;i++)
     { printf("%s\n",dir_list[i]->d_name);} 
   
}


char *detecting_parent(char *cwd_name)
{   int tmp;
    //printf("%s\n",cwd_name);
    for(int i=strlen(cwd_name)-1;i>=0;i--)
     {  if(cwd_name[i]=='/')
         {tmp = i;break;}

     }
   char * parent = malloc(tmp*sizeof(char));
     strncpy(parent,cwd_name,tmp);
  return parent;
}






void display_long(struct dirent **dir_list,int cnt,char *parent_path)
{   struct stat tmp_stat;
    stat(parent_path,&tmp_stat);
    //printf("total %ld\n",tmp_stat.st_blocks/2);
    char *lines[cnt];
    float total = 0;
    
    for(int i=0;i<cnt;i++)    
    {struct dirent *cur_dir = dir_list[i];
    struct stat cur_stat;
    stat(path(parent_path,cur_dir->d_name),&cur_stat);
    char type;
    if(S_ISDIR(cur_stat.st_mode))
    type='d';
    else if(S_ISLNK(cur_stat.st_mode))
    type ='l';
    else
    type = '-';


    total+=cur_stat.st_blocks; 
    char permissions[9];
  
    permissions[0]=(cur_stat.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[1]=(cur_stat.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[2]=(cur_stat.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[3]=(cur_stat.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[4]= (cur_stat.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[5]= (cur_stat.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[6]=(cur_stat.st_mode & S_IROTH) ? 'r' : '-';
    permissions[7]= (cur_stat.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[8]= (cur_stat.st_mode & S_IXOTH) ? 'x' : '-';


    
    struct passwd *p;
    struct group  *g;
    p = getpwuid(cur_stat.st_uid);
    g=getgrgid(cur_stat.st_gid);
    char *username = p->pw_name;
    char *groupname= g->gr_name;

    long long int file_sz= cur_stat.st_size;
    long long int no_of_links= cur_stat.st_nlink;



    char *tm_as_req = time_as_req(cur_stat.st_mtime);
 
    char *tmp =malloc(200);  
    sprintf(tmp,"%c%s %lld %s %s %lld %s %s\n",type,permissions,no_of_links,username,groupname,file_sz,tm_as_req,cur_dir->d_name);
    lines[i]= tmp;
    }
    printf("total %d\n",roundNo(((float)total/(float)2)));
    for(int p=0;p<cnt;p++)
     printf("%s",lines[p]);

    



}

int is_a_filter(const struct dirent *p)
{   if((p->d_name)[0] == '.')
    return 0;
    else 
    return 1;
}



void ls_implt(int cnt_pieces,char*pieces[])
{    
     char *cur_path;
     bool is_a=false,is_l=false;
     
     char *dirs[25];int no_of_dirs =0;
   
    for(int i=1;i<cnt_pieces;i++)
    {    if(pieces[i][0]=='-')
         {  for(int j=1;j<strlen(pieces[i]);j++)
            {  if(pieces[i][j]=='l')
                is_l=true;
               if(pieces[i][j]=='a')
                is_a=true;

            }
         }
       
         else
         { if(strlen(pieces[i])==1 && pieces[i][0]=='.')
           dirs[no_of_dirs]=cwd_name;
           else if(strlen(pieces[i])==2 && (strcmp(pieces[i],"..")==0))
            dirs[no_of_dirs]=detecting_parent(cwd_name);
             
           else
           dirs[no_of_dirs]=pieces[i];
           no_of_dirs++;  
         }
         
    }
    if(no_of_dirs ==0)
    {  dirs[no_of_dirs] = cwd_name;
       no_of_dirs++;

    }
   

     for (int i=0;i<no_of_dirs;i++)
    {   struct dirent **dir_list;
        int no_of_files;
        if(is_a == true && is_l ==true)
        {no_of_files=scandir(path(cwd_name,dirs[i]),&dir_list,NULL,&lexi_sort);
         display_long(dir_list,no_of_files,path(cwd_name,dirs[i]));}
        
        else if(is_a == true && is_l == false)
        {no_of_files=scandir(path(cwd_name,dirs[i]),&dir_list,NULL,&lexi_sort);
         display_short(dir_list,no_of_files,path(cwd_name,dirs[i]));}
        
        else if(is_a==false && is_l == true)
        {no_of_files=scandir(path(cwd_name,dirs[i]),&dir_list,&is_a_filter,&lexi_sort);
         display_long(dir_list,no_of_files,path(cwd_name,dirs[i]));}
        
        else if(is_a==false,is_l==false)
        {no_of_files=scandir(path(cwd_name,dirs[i]),&dir_list,&is_a_filter,&lexi_sort);
         display_short(dir_list,no_of_files,path(cwd_name,dirs[i]));}
      
       if(no_of_files == -1)
       {char * tmp = malloc(120* sizeof(char));
        strcpy(tmp,"Error Reg ");
        strcat(tmp,dirs[i]);
        perror(tmp);
       }
    }



}
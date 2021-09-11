#include "functions.h"
#include "headers.h"


int convert_to_int(char * str)
{    int t=0,i=0,s=1;
     if(str[i]=='-')
     { s=-1;i++;}

     for(;i < strlen(str);i++)
   {   t = t*10 + (str[i]-'0');
   }
return t*s;

}

int is_builtin_cmd(int cnt_pieces, char *pieces[])
{  if(strcmp(pieces[0],"cd")==0 || strcmp(pieces[0],"echo")==0 || strcmp(pieces[0],"ls")==0 || strcmp(pieces[0],"pinfo")==0 
    ||strcmp(pieces[0],"history")==0 || strcmp(pieces[0],"setenv")==0 || strcmp(pieces[0],"unsetenv")==0 || strcmp(pieces[0],"getenv")==0
    || strcmp(pieces[0],"jobs")==0 || strcmp(pieces[0],"kjob")==0 || strcmp(pieces[0],"fg")==0 || strcmp(pieces[0],"bg")==0 || strcmp(pieces[0],"overkill")==0)
    return 1;
    else
    return 0;
}




void intialize()
{    
    gethostname(hstnam,50);
    fas= getpwuid(getuid());
    getcwd(base_dir,sizeof(base_dir));
    cnt_of_bg = 0;
    index_of_hist =0;
    foregrndpid =0 ;

}

void sub_intialize()
{   getcwd(cwd_name,sizeof(cwd_name));
    if(strncmp(cwd_name,base_dir,strlen(base_dir))==0)
     cwd_asreq = &cwd_name[strlen(base_dir)];
    else
     cwd_asreq = cwd_name;
   
}

void prompt() 
{    
  printf("<%s@%s:~%s>",fas->pw_name,hstnam,cwd_asreq);

}



void separating_buff(char * buff, char *parts[],int *num)
{   
   int c=0;
   char * token = strtok(buff,";\n");

   while( token!=NULL )
   {
       parts[c] = strdup(token);
       c++;
       token = strtok(NULL,";\n");
   
   }
   parts[c]=NULL;   
   *num = c;
}

void separating_based_on_pipes(char * buff, char *pieces_pipes[],int *num)
{   
   int c=0;
   char * token = strtok(buff,"|");

   while( token!=NULL )
   {
       pieces_pipes[c] = strdup(token);
       c++;
       token = strtok(NULL,"|");
   
   }
   pieces_pipes[c]=NULL;   
   *num = c;
}

void separating_str(char * buff, char *pieces[],int *num )
{  int c =0;
   char * token = strtok(buff," \t\n"); 

   while( token!=NULL )
   {
       pieces[c] = strdup(token);
       c++;
       token = strtok(NULL," \t\n");
   
   }
    pieces[c]=NULL;
   *num = c;
   
}


void execute_theline_other(int cnt_pieces, char *pieces[])
{       int k =0; 
        k=execvp(pieces[0],pieces);
        if(k==-1)
        printf("%s: command not found\n",pieces[0]);
       // perror("error:");
        exit(0); 
       

}



void execute_theline_buitin(int cnt_pieces, char *pieces[])
{     if(strcmp(pieces[0],"cd")==0)
        cd_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"echo")==0)
           echo_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"ls")==0)
          ls_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"pinfo")==0)
          pinfo_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"history")==0)
          hist_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"setenv")==0)
           setenv_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"unsetenv")==0)
          unsetenv_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"getenv")==0)
          getenv_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"jobs")==0)
          jobs_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"kjob")==0)
          kjob_implt(cnt_pieces,pieces);
       else if(strcmp(pieces[0],"fg")==0)
          fg_implt(cnt_pieces,pieces);
        else if(strcmp(pieces[0],"bg")==0)
          bg_implt(cnt_pieces,pieces);   
      else if(strcmp(pieces[0],"overkill")==0)
          overkill_implt(cnt_pieces,pieces);   

}


void echo_implt(int cnt_pieces, char *pieces[])
{   for(int i=1;i<cnt_pieces;i++)
    printf("%s ",pieces[i]);
    printf("\n");
    
}

void pwd_implt(int cnt_pieces, char *pieces[])
{   printf("%s",cwd_name);

}

void cd_implt(int cnt_pieces, char *pieces[])
{    if(cnt_pieces>1)
    {  if(strcmp(pieces[1],"~") == 0) 
        chdir(base_dir);
        
       else if(strcmp(pieces[1],".")==0)
        {}
       else if(strcmp(pieces[1],"-")==0)
       {  printf("%s\n",lwd);
          chdir(lwd);
       }
       else
       { if(chdir(pieces[1])==-1)
           perror("Error");
           
       }
    }

    else
      {  
          chdir(base_dir); 
       
      }
      
  
}



void pinfo_implt(int cnt_pieces, char *pieces[])
{   int pid;
    

     if(cnt_pieces > 1)
      {   pid = convert_to_int(pieces[1]);
          
      }
          
     else
      { pid=getpid();}
   
   char filename_stat[200];
  sprintf(filename_stat,"/proc/%d/stat",pid);

  //  printf("%s",filename_stat);
   FILE *f = fopen(filename_stat,"r");
    if( f == NULL )
    perror("Error:");
   
   char state[2];
   long unsigned int vm_sz;
   int t = fscanf(f,"%*d%*s%s%*d%*d%*d%*d%*d%*u%*u%*u%*u%*u%*u%*u%*d%*d%*d%*d%*d%*d%*u%lu",state,&vm_sz);


   fclose(f);

   char filename_exe[200];
   sprintf(filename_exe,"/proc/%d/exe",pid);
   char *exe_path= malloc(500);
   int k= readlink(filename_exe,exe_path,500);
    if(k ==-1)
   perror("Error:");
   
   exe_path[k]='\0';
  
   printf("pid -- %d\nProcess Status -- %s\nmemory -- %lu\nExecutable Path -- %s\n",pid,state,vm_sz,exe_path);



}

void setenv_implt(int cnt_pieces, char *pieces[])
{  if(cnt_pieces==3) 
   { int tmp = setenv(pieces[1],pieces[2],1);
     if(tmp < 0)
     perror("Error reg setenv");
   }
   else if(cnt_pieces==2)
   { int tmp = setenv(pieces[1],pieces[2],1);
     if(tmp < 0)
     perror("Error reg setenv");
 
   }
   else
   {
      printf("No of arguments needs to be 2 for setenv\n");
   }
   
}
void unsetenv_implt(int cnt_pieces, char *pieces[])
{ if(cnt_pieces==2)
  {  int temp =unsetenv(pieces[1]);
     if(temp < 0)
     perror("Error reg unsetenv");
     }
  else
  {
     printf("No of arguments needs to be 1 for unsetenv");
  }
  
}
void getenv_implt(int cnt_pieces, char *pieces[])
{  char * t = getenv(pieces[1]);
   if(t == NULL)
   printf("environment variable not found\n");
   else 
   printf("%s\n",t);
}


void jobs_implt(int cnt_pieces, char *pieces[])
{  int j; 
   for(int i=0,j=0;i<cnt_of_bg;i++)
     {  if(bgarr[i].is_active == 1)
       {  j++;
          char filename_stat[200];
       //  printf("The pid is %d\n",bgarr[i].pid);
          sprintf(filename_stat,"/proc/%d/stat",bgarr[i].pid);
          
         FILE *f = fopen(filename_stat,"r");
    
         if( f == NULL )
        { perror("Error:");
         return;
         }
          char state[2];
         int t = fscanf(f,"%*d%*s%s",state);
         
        fclose(f);
       
       if(t<=0)
        { printf("error in status checking\n");
          return ;
        }
       
       
       strcpy(bgarr[i].status,"Running");
       if(state[0]=='S')
       strcpy(bgarr[i].status,"Running");
       else if(state[0]=='T')
       strcpy(bgarr[i].status,"Stopped");       
        
       
       printf("[%d] %s %s [%d]\n",j,bgarr[i].status,bgarr[i].name,bgarr[i].pid);
      
       }

     }



}
void kjob_implt(int cnt_pieces, char *pieces[])
{   int j_num = 0; int sig=0;
   
   for(int i=0;i<strlen(pieces[1]);i++)
    j_num = j_num*10+(pieces[1][i]-'0');
   
   for(int i=0;i<strlen(pieces[2]);i++)
    sig = sig*10+(pieces[2][i]-'0');
   
   int j;
   for(int i=0,j=0;i<cnt_of_bg;i++)
   { if(bgarr[i].is_active == 1 )
     { j++;
       if(j == j_num)
      { int a = kill(bgarr[i].pid,sig);
        bgarr[i].is_active=0;
       if(a == -1)
        perror("Reg kjob");
        break;
      }
     }
   
   }
  // printf("%d %d\n",j_num,sig);


}

void fg_implt(int cnt_pieces, char*pieces[])
{   int j_num =0;
  
  for(int i=0;i<strlen(pieces[1]);i++)
    j_num = j_num*10+(pieces[1][i]-'0');

   int j=0, pid_of_req=-5;
  int k;
  for(k=0;k<cnt_of_bg;k++)
  {  if(bgarr[k].is_active == 1)
     { j++;
       if(j == j_num)
       {
         pid_of_req = bgarr[k].pid;
         break;
       }

     }
   }
   if(pid_of_req == -5)
   {  printf("No job found with the given job num\n");
      return;
   }
   signal(SIGCHLD, SIG_IGN);
    
   int temp = kill(pid_of_req,SIGCONT);
   if(temp<0)
   {  perror("Error reg kill");
   }
   foregrndpid = pid_of_req;
   bgarr[k].is_active=0;

   int ret_status;
   waitpid(-1,&ret_status,WUNTRACED);
   
   if(WIFSTOPPED(ret_status))
   {   bgarr[k].is_active=1;

   }
   signal(SIGCHLD, handler);

}

void bg_implt(int cnt_pieces,char *pieces[])
{  int j_num =0;
  
  for(int i=0;i<strlen(pieces[1]);i++)
    j_num = j_num*10+(pieces[1][i]-'0');

   int j=0, pid_of_req=-5;
  int k;
  for(k=0;k<cnt_of_bg;k++)
  {  if(bgarr[k].is_active == 1)
     { j++;
       if(j == j_num)
       {
         pid_of_req = bgarr[k].pid;
         break;
       }


     }
   }
   if(pid_of_req == -5)
   {  printf("No job found with the given job num\n");
      return;
   }
   int temp= kill(pid_of_req,SIGCONT);
    if(temp<0)
    perror("Error reg kill:");
}

void overkill_implt(int cnt_pieces, char *pieces[])
{  
   for(int i=0;i<cnt_of_bg;i++)
   {   if(bgarr[i].is_active == 1)
     {  if( kill(bgarr[i].pid,SIGKILL) <0)
         perror("Error in overkill");
        bgarr[i].is_active=0;
     }
   }


}
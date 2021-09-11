#include "functions.h"
#include "headers.h"
int shell_pid;
int bgpid = 0;
bool first = true;

int cnt_pieces;
char *pieces[30];

void handler()
{    int corpse;
     int status;
     char *pr_name; 
   
     corpse = waitpid((pid_t)-1, &status, WNOHANG | WUNTRACED);
     
     
    if(corpse > 0)
    {  int i;
        for(i=0;i<cnt_of_bg;i++)
        if(bgarr[i].pid == corpse)
          break;
      if (WIFEXITED(status)) 
         {  int a = WEXITSTATUS(status); 
            if(a == 0)
              printf("\r%s with pid %d exited \n", bgarr[i].name ,corpse);
             else
               fprintf(stderr,"\r%s with pid %d exited with status %d \n", bgarr[i].name ,corpse,a);
               bgarr[i].is_active = 0;
           
            prompt();
             fflush(stdout);
         }
     
      else if (WIFSIGNALED(status)) 
        { //fprintf(stderr,"\n%s with pid %d killed by signal %d\n",bgarr[i].name ,corpse, WTERMSIG(status));
           //bgarr[i].is_active = 0;
           } 
     
      else if (WIFSTOPPED(status)) 
        fprintf(stderr,"\n%s with pid %d stopped by signal %d\n", bgarr[i].name ,corpse,WSTOPSIG(status));
     
      else if (WIFCONTINUED(status)) 
        fprintf(stderr,"\n%s with pid %d continued\n", bgarr[i].name ,corpse);
        
    }

}

void ctrlz_handler()
{   if(getpid()!=shell_pid)
    return;

  if(foregrndpid!=0)
  {  if(kill(foregrndpid,SIGTTIN)<0)
      {  perror("Error in kill");
          return;
      }
    if(kill(foregrndpid,SIGTSTP)<0)  
       { perror("Error in kill");  
         return ;
       }
        bgarr[cnt_of_bg].pid = foregrndpid;
        strcpy(bgarr[cnt_of_bg].name,pieces[0]);
        bgarr[cnt_of_bg].is_active =1; 
        cnt_of_bg++;
   }
  signal(SIGTSTP,ctrlz_handler);
}
void ctrlc_handler()
{  if(getpid()!=shell_pid)
  return; 
  if(foregrndpid!=0)
 {  if(kill(foregrndpid,SIGINT)<0)
        perror("Error in kill");
    foregrndpid=0;    
  fprintf(stderr,"\r");
 }

signal(SIGINT, ctrlc_handler);

}

void check_redir(int * cnt_pieces,char * pieces[])
{    int index_of_in = -1;
    int index_of_out = -1;
    int index_of_append =-1;
    int c = *cnt_pieces;
    for(int i=0;i<c;i++)
    { if(strcmp(pieces[i],"<")==0) 
        index_of_in=i;
      if(strcmp(pieces[i],">")==0)
      index_of_out=i;
      else if(strcmp(pieces[i],">>")==0)
      index_of_append =i;

    }
   if(index_of_out >=0)
   {  int open_fd = open(pieces[index_of_out+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if(open_fd < 0)
        {
            perror("Error reg file opening:");
            return;
        } 
        pieces[index_of_out]=NULL;pieces[index_of_out+1]=NULL;
        dup2(open_fd, 1);
        close(open_fd);
      c-=2; 
  }

    if(index_of_append >=0)
   {  int open_fd = open(pieces[index_of_append+1], O_WRONLY| O_APPEND | O_CREAT, 0644);
        if(open_fd < 0)
        {
            perror("Error reg file opening:");
            return;
        } 
        pieces[index_of_append]=NULL;pieces[index_of_append+1]=NULL;
        dup2(open_fd, 1);
        close(open_fd);
        c-=2;
    }
    

    if(index_of_in >=0)
   {  int open_fd = open(pieces[index_of_in+1], O_RDONLY);
        if(open_fd < 0)
        {
            perror("Error reg file opening:");
            return;
        } 
        pieces[index_of_in]=NULL;pieces[index_of_in+1]=NULL;
        dup2(open_fd, 0);
        close(open_fd);
     c-=2;
  }

 *cnt_pieces = c;

}




int fn(char *p)
{   
  //int actual_in = dup(0);
  int actual_out = dup(1);
 
  separating_str(p,pieces,&cnt_pieces);

  check_redir(&cnt_pieces,pieces);
 

  
  if(strcmp(pieces[0],"quit")==0)
  return 0; 
  if(strcmp(pieces[cnt_pieces-1],"&")==0)
    { pieces[cnt_pieces-1] = NULL;
         cnt_pieces--;
         background =1;
    }
  else
      background = 0;
     
      
  if(background && is_builtin_cmd(cnt_pieces,pieces))
       background = 0;
       
  if(background)
  { pid_t pid=fork();
    if(pid==0)
    { setpgid(0,0);
      execute_theline_other(cnt_pieces,pieces);
    }
    
      printf("started %s with pid %d\n",pieces[0],pid);
      bgarr[cnt_of_bg].pid = pid;
      strcpy(bgarr[cnt_of_bg].name,pieces[0]);
      bgarr[cnt_of_bg].is_active =1;
      
     cnt_of_bg++;  
      
  }

  else if (!background)
      {  if(is_builtin_cmd(cnt_pieces,pieces))
          execute_theline_buitin(cnt_pieces,pieces);
         else 
        {  int k =fork();
          if(k<0)
          { perror("Error in forking:");
            return 0;
          }
      
          if(k==0)
          { 
            execute_theline_other(cnt_pieces,pieces);
          
          }
          if(k > 0)
          { foregrndpid = k;
            
            
           
            waitpid(-1,NULL,WUNTRACED);

           /*if(WIFSTOPPED(ret_status))
           {  int ret_status;
           }*/
            foregrndpid =0;
           
          } 
         }
      }
  
  
  
   dup2(actual_out,1);
//   dup2(actual_in,0);   
return 1;
}



int main()
{  
    size_t buff_sz = 1000;
    buffer= (char *)malloc(buff_sz*sizeof(char));
    buffer_dup=(char *)malloc(buff_sz*sizeof(char));
    intialize();
    
    shell_pid = getpid();
    while (1)
    {     free(buffer);
          free(buffer_dup);

         signal(SIGCHLD,handler);
          signal(SIGTSTP,ctrlz_handler);
          signal(SIGINT,ctrlc_handler);
          //signal(EOF,ctrld_handler);
        sub_intialize();
       // printf("pid is %d |",getpid());
       buffer= (char *)malloc(buff_sz*sizeof(char));
    buffer_dup=(char *)malloc(buff_sz*sizeof(char));
        prompt();
       int b = getline(&buffer,&buff_sz,stdin);
       if(b<=0)
       { printf("\nShell is been exited\n");
         return 0;
       }
        store_history();
        strcpy(buffer_dup,buffer);
      
      char *parts[80],*pieces_pipes[20];
      int cnt_no_commands,cnt_based_on_pipes;

     

      // based on ;
      separating_buff(buffer_dup,parts,&cnt_no_commands);
      int a=1;
      for(int i = 0;i<cnt_no_commands;i++)
      {  
        separating_based_on_pipes(parts[i],pieces_pipes,&cnt_based_on_pipes);

        int no_of_pipes = cnt_based_on_pipes - 1;
        int actual_in= dup(0);
        int actual_out = dup(1);

        int j=0;
        for(j=0;j< no_of_pipes;j++)
        { 
          int fd_pipes[2];
          pipe(fd_pipes);
          
          int p =fork();
          if(p == 0)
          { dup2(fd_pipes[1],1);
            close(fd_pipes[0]);
            close(fd_pipes[1]);
           
            fn(pieces_pipes[j]);
             abort();           
          } 
          else if (p >0)
          { wait(NULL);
            dup2(fd_pipes[0],0);
            close(fd_pipes[0]);
            close(fd_pipes[1]);
          }
     

        }
        if(j == no_of_pipes)
        {  dup2(actual_out,1);
           a =fn(pieces_pipes[j]);
          if(!a)
          break;
        }
       

        
       dup2(actual_in,0);
       dup2(actual_out,1); 
      
   
     strcpy(lwd,cwd_name);
     }
    if(a==0)
     {  printf("Shell is been exited\n");
       break;}
    }
return 0;
}

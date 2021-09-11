#include "functions.h"
#include "headers.h"



void store_history()
{  
    FILE * frmfptr;
    FILE * tofptr;

   int present_line_no,tobereplaced_no;
   char path[200];
   strcpy(path,base_dir);
   strcat(path,"/history.txt");
      
   int buf_sz =1000;

   char tmp_buf[buf_sz];
   char inpt_line[buf_sz];
   
   strcpy(inpt_line,buffer);

   frmfptr = fopen(path,"r");
   if(frmfptr == NULL)
   {frmfptr = fopen("history.txt","w+");
    char *tmp = "0\n";
    fputs(tmp,frmfptr);
    fseek(frmfptr,0,SEEK_SET);
   }
   tofptr = fopen("replace.tmp","w");
   //fseek(frmfptr,0,SEEK_SET);
  
  char *num_buff = malloc(5);
  index_of_hist = 0; 
  present_line_no = 0 ;

  
  fgets(tmp_buf,buf_sz,frmfptr);

 
  //printf("index as read from file %s,%ld\n",tmp_buf,strlen(tmp_buf));

  for(int i=0;i<strlen(tmp_buf)-1;i++)
  { index_of_hist = index_of_hist*10+(tmp_buf[i]-'0');
  }


  if(index_of_hist == 20)
   index_of_hist = 0;
  index_of_hist++;
  
  //printf("index of history %da\n",index_of_hist);
  sprintf(num_buff,"%d\n",index_of_hist);
  fputs(num_buff,tofptr);
   
 present_line_no =0;
 
  while(fgets(tmp_buf,buf_sz,frmfptr)!=NULL)
  {  present_line_no++;
     
     if(present_line_no == index_of_hist)
     fputs(inpt_line,tofptr);
     else
      fputs(tmp_buf,tofptr);
  }
  if(present_line_no !=20)
  {
    fputs(inpt_line,tofptr);
    
  } 
  //if(index_of_hist==20)
  //is_historyfull=1;
  fclose(frmfptr);
  fclose(tofptr);

   remove(path);
   rename("replace.tmp",path);
   

}


   

void hist_implt(int cnt_pieces, char *pieces[])
{ 
    FILE * frmfptr;
   char path[200];
   strcpy(path,base_dir);
   strcat(path,"/history.txt");
   frmfptr = fopen(path,"r");

    int c=0 ,index =0 ;
    if(cnt_pieces == 1)
    c =10;
    else 
    {   for(int j=0;j<strlen(pieces[1]);j++)
          c = c*10 + (pieces[1][j]-'0');
    }
    char *l_of_history[21];

   int k;
   for(k=0;k<21;k++)
   {  l_of_history[k]=malloc(1000);
      
      if(fgets(l_of_history[k],1000,frmfptr)==NULL)
          break;
   
   }
   if(k>=20)
   is_historyfull = 1;


  for(int i=0;i<strlen(l_of_history[0])-1;i++)
  { index = index*10+(l_of_history[0][i]-'0');
  }
  
  int sp = index - c;
  //printf("%d %d\n",sp,is_historyfull);
  if(sp < 0 )
  {  if(is_historyfull)
     {  sp = 20 +sp;
      
      for(int k =sp+1;k<=20;k++)
      { printf("%s",l_of_history[k]);
        
        if(k == 20)
        {  k=1; break;}
      }
      for(k =1;k<=index;k++)
      { printf("%s",l_of_history[k]);
        
        
      }
     
     }
     else
     {   sp=0;
       for(int k =sp+1;k<=index;k++)
       printf("%s",l_of_history[k]);
        
     }
  }
  else
  {  for(int k =sp+1;k<=index;k++)
       printf("%s",l_of_history[k]);
      
  }
  
fclose(frmfptr);


}

     






   












#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H
void handler();
void intialize();
void sub_intialize();
void prompt();
void separating_buff(char * buff, char *parts[],int * num);
void separating_based_on_pipes(char * buff, char *pieces_pipes[],int *num);
void separating_str(char * buff, char *pieces[],int * num);

void execute_theline_buitin(int cnt_pieces, char *pieces[]);
void execute_theline_other(int cnt_pieces, char *pieces[]);
void cd_implt(int cnt_pieces, char *pieces[]);
void echo_implt(int cnt_pieces, char *pieces[]);
void ls_implt(int cnt_pieces, char *pieces[]);
void pinfo_implt(int cnt_pieces, char *pieces[]);
int is_builtin_cmd(int cnt_pieces, char *pieces[]);
void store_history();
void hist_implt(int cnt_pieces, char *pieces[]);
void setenv_implt(int cnt_pieces, char *pieces[]);
void unsetenv_implt(int cnt_pieces, char *pieces[]);
void getenv_implt(int cnt_pieces, char *pieces[]);
void jobs_implt(int cnt_pieces, char *pieces[]);
void kjob_implt(int cnt_pieces, char *pieces[]);
void fg_implt(int cnt_pieces,char *pieces[]);
void bg_implt(int cnt_pieces,char *pieces[]);
void overkill_implt(int cnt_pieces, char *pieces[]);
int fn(char *p);


int background;
int bgprpidarr[1024][1];
char *bgprpidnamearr[1024];
int cnt_of_bg;
int index_of_hist;
int is_historyfull;
int foregrndpid;

struct passwd *fas;
char base_dir[200];
char cwd_name[200];
char *cwd_asreq;
char hstnam[50];
char *buffer;
char *buffer_dup;
char lwd[200];

typedef struct bgp
{  int pid;
   int is_active;
   char name[100];
   char status[100];
}bgp;

bgp bgarr[1024];


#endif
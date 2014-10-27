#include <stdio.h>
#include <stdlib.h>
#include<malloc.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
/* ABOVE ARE THE VARIOUS LIBRARIES */
#define MAX_LEN 1024
#define DELIMS " \t\r\n"
#define del " <>"
#define dele "<>"


int aj;
char txt[255],hstnm[1024];
char pwd2[1000];

typedef struct allhis
{
	char nm[100];
	int pid;
}allhis;
/* STRUCTURE allhis STORES ALL PROCESSES AND THEIR PIDS*/

int k1=0;int pdp[1000],flag[1000]={0},cd[1000];
char pdpnm[1000][100],curr[1000][100];
int fl1=0,fl2=0,fl3=0,k2=0;
void sig_handler(int signum){

	signal(SIGINT, sig_handler);
	signal(SIGTSTP, sig_handler);
//	signal(SIGCHLD, child_handler);
	signal(SIGQUIT,sig_handler);


	return;
}
void child_handler(int signum)
{

	int status,x;
	pid_t pid;
	pid=waitpid(WAIT_ANY,&status,WNOHANG);

	for(x=0;x<k2;x++)
	{
		if(cd[x]==pid)
		{
			printf("\n");
			printf("%s %d exited normally\n",curr[x],pid);
			printf("<%s@%s:%s>",txt,hstnm,pwd2);

			
				flag[x]=0;
				break;
			}
		
	}
	fflush(stdout);
	signal(SIGCHLD,child_handler);
}

/*child_handler mainly used to handle BACKGROUND PROCESSES*/

int key=0,k=0; 
int his_flag=0;
char h[1000][1000];/*h stores history*/
//---------------------------------------------------------------------------------------------------//
void histr()
{	
	his_flag=0;
	int i;
	for(i=0;i<k-1;i++)printf("%d. %s\n",(i+1),h[i]);
}
//--------------------------------------------------------------------------------------------------//
void ppid1();//ppid1->pid
void ppid2();//ppid2->pid all
void ppid3();//ppid3->pid current
//lashis basically performs work of histn if it comes in a piped process-----------------------------//
void lashis(char *cmd)
{
	his_flag=0;                             //-------------------------------------------//
	int ln=0,cv;                            // histn command: here n is sored in ln      //
	//printf("%s\n",cmd);			//e.g. hist15 : ln stores 15                 //
	for(cv=4;cv<strlen(cmd);cv++)           //the last loop to print last n commands     //
	{     					//-------------------------------------------//
		ln=(ln*10)+(int)(cmd[cv]-'0');	
	}
		//printf("%d %d\n",k,ln);
	int sp=1;
	for(cv=k-ln+-2;cv<k-2;cv++)
	{
			printf("%d. %s\n",sp,h[cv]);
			sp++;
	}
}


/*-----------------------------------------------------------------------------------------------------*/
/* ------------------MAIN FUNCTION-----------------*/
allhis **hs1;
//hs1=(allhis**)malloc(sizeof(allhis*)*1000);
int hsin=0;
int main(int argc, char *argv[])
{
//	allhis **hs1;
	hs1=(allhis**)malloc(sizeof(allhis*)*1000);
//	int hsin=0;
	for(hsin=0;hsin<1000;hsin++)hs1[hsin]=(allhis*)malloc(sizeof(allhis));
	hsin=0;
	
signal(SIGINT,  sig_handler);
signal(SIGTSTP, sig_handler);
signal(SIGCHLD, child_handler);
signal(SIGQUIT,sig_handler);


long pd[1000];
int x,y;
//k=0;
for(x=0;x<100;x++)
{
	
	for(y=0;y<100;y++)
	{
		h[x][y]='\0';
		curr[x][y]='\0';
	}
}
char *pwd1;/*pwd1 gives the directory from where we are invoking the function*/
if((pwd1=getcwd(NULL,64))==NULL)
{
	perror("pwd1");
	exit(0);
}
aj=1;

char p1[1000],p3[1000];
strcpy(p1,pwd1);
strcpy(p3,pwd1);


free(pwd1);
int cle=strlen(pwd1);
while(1)
{
	char p2[1000];
	FILE *usrnm=popen("whoami","r");/*usrnm stores the username*/
	fgets(txt,sizeof(txt),usrnm);
	if(txt[strlen(txt)-1]=='\n')txt[strlen(txt)-1]='\0';
	hstnm[1023]='\0';
	gethostname(hstnm,1023);/*hstnm stores the name of hostmachine*/

	char *pwd;
	if((pwd=getcwd(NULL,64))==NULL)
	{
		perror("pwd");
		exit(0);
	}
	strcpy(p2,pwd);
	free(pwd);
	pwd2[0]='~';				
						/*---------------------------------*/
	int x1,x2=1;				/*       a bit of manipulation     */ 
	for(x1=cle;x1<strlen(pwd);x1++)		/*       to get the current        */
	{					/*       with respect to the       */
		pwd2[x2]=pwd[x1];		/*       from where function       */
		x2++;				/*       was invoked. pwd2         */
	}					/*       stores this path          */
	pwd2[x2]='\0';				/*---------------------------------*/
	
	printf("<%s@%s:%s>",txt,hstnm,pwd2);	/*printing command prompt*/
	fflush(stdout);
	char st1[1000];
	char hiso[1000];
	
	char ln[MAX_LEN];                      /*ln takes command name from user e.g. ls*/
	char *cmd;
	if(his_flag==0)			      /*condition to check if ln is entered or !histn was entered*/
	{
		if(!fgets(ln,MAX_LEN,stdin))break;
		ln[strlen(ln)-1]='\0';
	}
	else if(his_flag==1)		   /*if previous cmmnd was !histn the nth value from history array is copied in ln*/
	{
		strcpy(ln,hiso);
		printf("%s\n",ln);
	}
	char ln2[1000];
	strcpy(ln2,ln);

	strcpy(h[k],ln);
	
	k++;
	strcpy(st1,"command name:");
	strcat(st1,ln);
	strcat(st1," process id: ");

	cmd=strtok(ln,DELIMS);

	if(cmd)
	{
	if((strchr(ln2,'<') || strchr(ln2,'>'))&&(!strchr(ln2,'|')))/*condition for case 1 of assignment 2(file redirection only)*/
	{
		his_flag=0;
		char dup[1000],sign[1000];
		strcpy(dup,ln2);
		int cnt=0;
		int ia,ja,ksmall=0,kbig=0,asmall=-1,abig=-1,aapp=-1,kapp=0;
		for(ia=0;ia<strlen(ln2);ia++)
		{
			if(ln2[ia]=='>')
			{	 if(ia<strlen(ln2) && ln2[ia+1]!='>'){
				sign[cnt]='>';	/*ksmall stores the number of < in command */
				abig=cnt;	/*kbig stores the number of > in command   */
				cnt++;	        /*cnt stores the total < > in command      */
				kbig++;		/*sign array stores < > in order of command*/
				               /*------------------------------------------*/
			}
			else
			{
				sign[cnt]='p';
				aapp=cnt;
				cnt++;
				kapp++;
				ia++;
			}}
		
		
		
			else if(ln2[ia]=='<')
			{
				sign[cnt]='<';
				asmall=cnt;
				cnt++;
				ksmall++;
			}
		}
		char fname[1000][1000]; /*fname array stores the filenames given in command*/
		for(ia=0;ia<1000;ia++)
		{
			for(ja=0;ja<1000;ja++)fname[ia][ja]='\0';
		}
		ia=0;

		char *tkn=strtok(ln2,dele);
		
		char cmdname[100];
		strcpy(cmdname,tkn);    /*cmdname stores the command name(process)*/
		while(tkn!=NULL)
		{	
			
			tkn=strtok(NULL,del);
			if(tkn==NULL)break;
			strcpy(fname[ia],tkn);
			ia++;

		}
		int fdo=1,fdw=1;
	//	for(ja=0;ja<ia;ja++)printf("%c %s\n",sign[ja],fname[ja]);
		for(ja=0;ja<cnt;ja++)
		{
			if(sign[ja]=='<')
			{
				fdo=open(fname[ja],O_RDONLY);
				if(fdo==-1)break;
			}
			else if(sign[ja]=='>')
			{
				int fdd=open(fname[ja],O_WRONLY);
				if(fdd!=-1)
				{
					pid_t pidu;
					int status;
					char *cm[]={"rm",fname[ja],NULL};
					pidu=fork();
					if(pidu==0)
					{
						execvp(*cm,cm);
						_exit(0);
					}
					else
					{
						while(wait(&status)!=pidu)
							;

					}
				}

				
				fdw=open(fname[ja], O_WRONLY | O_CREAT, S_IRWXU);
		}
		}
		if(fdo==-1)printf("file %s doesnot exist\n",fname[ja]);
		else
			{
				char a[100][100];	/*--------------------------------------------*/
				int i,j;		/* array a will stored process name in the    */
				for(i=0;i<100;i++)	/* parsed form e.g ls -a -l > 1               */
				{			/* a[0]->ls                                   */
				for(j=0;j<100;j++)	/*a[1]->-a                                    */
				{			/*a[2]->-l                                    */
				a[i][j]='\0';		/*--------------------------------------------*/
				}
				}
		char *cmd1=strtok(cmdname,DELIMS);
		strcpy(a[0],cmd1);
		char *s;
		for(i=1;i<100;i++)
		{
			
			s=strtok(NULL,DELIMS);
			if(!s)break;
			strcpy(a[i],s);
		}
		char *cm[100];

		for(j=0;j<i;j++)
		{
			cm[j]=a[j];

		}
		cm[i]=NULL;
		if(ksmall==0 && kapp==0)    /*if condition for eg like ls > 1 */
		{
			//printf("%d\n",k);
		//	printf("%s\n",cm[1]);
			pid_t pid;
			int status;
			int fd=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			pid=fork();
			if(pid==0)
			{
			dup2(fd,1);
			close(fd);
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
				ppid2();
				}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
			
			execvp(*cm,cm);
			_exit(0);
			
			}
			else
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}
		}
			else if(ksmall==0 && kbig==0)    /*if condition for eg like ls > 1 */
		{
			pid_t pid;
			int status;
			int fd=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			pid=fork();
			if(pid==0)
			{
			dup2(fd,1);
			close(fd);
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();
				}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
			
			execvp(*cm,cm);
			_exit(0);
			

			}
			else
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}
		}

		else if(kbig==0 && kapp==0) /* if condition for eg like sort < 1 */
		{
			pid_t pid;
			int status;
			int fd=open(fname[asmall],O_RDONLY);
			pid=fork();
			if(pid==0)
			{
				dup2(fd,0);
				close(fd);
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
			
			execvp(*cm,cm);
			_exit(0);
			

			}
			else  
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}
		}
		else if(ksmall!=0 && kbig!=0 && kapp==0)       /* if condition for eg like sort < 1 > 2 */
		{
			pid_t pid;
			int status;
			int fd=open(fname[asmall],O_RDONLY);
			int fd1=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			pid=fork();
			if(pid==0)
			{
				dup2(fd,0);
				close(fd);
				dup2(fd1,1);
				close(fd);
				if(strcmp(cm[0],"hist")==0)histr();
				else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
				else if(strcmp(cm[0],"pid")==0)
				{
					if(!cm[1])ppid1();
					else if(strcmp(cm[1],"all")==0)
					{
						ppid2();}
					else if(strcmp(cm[1],"current")==0)ppid3();
				}
			
			
			
				else
				
				execvp(*cm,cm);
				_exit(0);
				

				
			}
			else
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}
		}

		else if(ksmall!=0 && kapp!=0 && kbig==0)       /* if condition for eg like sort < 1 > 2 */
		{
			pid_t pid;
			int status;
			int fd=open(fname[asmall],O_RDONLY);
			int fd1=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			pid=fork();
			if(pid==0)
			{
				dup2(fd,0);
				close(fd);
				dup2(fd1,1);
				close(fd);
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
			
			execvp(*cm,cm);
			_exit(0);
			

				
			}
			else
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}
		}
		else if(ksmall==0 && kbig!=0 && kapp!=0)    /*if condition for eg like ls > 1 */
		{
			pid_t pid;
			int status,fd;
			if(abig>aapp)
				fd=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			else if(aapp>abig)
			fd=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU); 
			pid=fork();
			if(pid==0)
			{
			dup2(fd,1);
			close(fd);
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
			
			execvp(*cm,cm);
			_exit(0);
			

			
			}
			else
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}
		}

		else if(ksmall!=0 && kbig!=0 && kapp!=0)       /* if condition for eg like sort < 1 > 2 */
		{
			pid_t pid;
			int status,fd1;
			int fd=open(fname[asmall],O_RDONLY);
			if(abig>aapp)
			fd1=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			else if(aapp>abig)
			fd1=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT , S_IRWXU);
			pid=fork();
			if(pid==0)
			{
				dup2(fd,0);
				close(fd);
				dup2(fd1,1);
				close(fd);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);
			

			
			}
			else
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}
		}




		

	}}

	else if((strchr(ln2,'<') || strchr(ln2,'>'))&&(strchr(ln2,'|')))/*if condition for commands like in case 3 of assignment 2 */
	{								/*      e.g cat < in.txt | wc > out.txt                    */
		his_flag=0;
		int res;
		dup2(res,0);
		char dup[1000];
		strcpy(dup,ln2);
		int cnt=0;
		char *tkn=strtok(ln2,"|");
		while(tkn!=NULL)
		{	
			cnt++;                         /*cnt stores no of processes separated by | */
			tkn=strtok(NULL,"|");

		}
		//printf("| count%d\n",cnt);
		//printf("input %s\n",dup);
		char pa1[100];
		int x,y;


	
		pid_t pid;
		int kp=1,val=0,in=0,out,fd[2];
		for(kp=0;kp<cnt;kp++)
		{				      /*------------------------------------------------------*/
			for(x=0;x<100;x++)pa1[x]='\0';/*the loop will go on cnt no of times and pa1 will take */
			int i,j=0;		      /*processes separated by | in each iteration            */
			for(i=val;i<strlen(dup);i++)  /*------------------------------------------------------*/
			{
				if(dup[i]=='|')break;
				pa1[j]=dup[i];
				j++;
			}			     /*-----------------------------------------------------------------------------*/
			val=i+1;                     /*val is just used to store index of | so that pa1 can store command next to | */
		//	printf("%spa1\n",pa1);	     /* in next iteration                                                           */	
			char dupp[1000],sign[1000];  /*-----------------------------------------------------------------------------*/	
			strcpy(dupp,pa1);
		//	printf("dupp%s\n",dupp);
		int count=0;
		int ia,ja,ksmall=0,kbig=0,asmall=-1,abig=-1,kapp=0,aapp=-1;
		for(ia=0;ia<strlen(pa1);ia++)
		{
			if(pa1[ia]=='>')
				{
					if(ia<strlen(pa1) &&pa1[ia+1]!='>')
					{
				sign[count]='>';
				abig=count;
				count++;
				kbig++;               /*kbig,ksmall,sign performs the same function as described in previous if   */
				}
					else
					{
						sign[count]='p';
						aapp=count;
						ia++;
						count++;
						kapp++;
					}}
			else if(pa1[ia]=='<')
			{
				sign[count]='<';
				asmall=count;
				count++;
				ksmall++;
			}
		}
	//	printf("kbig %d ksmall %d\n",kbig,ksmall);
		char fname[1000][1000];
		for(ia=0;ia<1000;ia++)                      /*fname stores filename of each pipe separated process */
		{
			for(ja=0;ja<1000;ja++)fname[ia][ja]='\0';
		}
		ia=0;

		char *tkn1=strtok(dupp,dele);
		
		char cmdname[100];                       /*cmdname stores command name of each pipe separated process */
		strcpy(cmdname,tkn1);
	//	printf("counts%d command%s\n",count,cmdname);
		while(tkn1!=NULL)
		{	
			
			tkn1=strtok(NULL,del);
			if(tkn1==NULL)break;
			strcpy(fname[ia],tkn1);
			ia++;

		}

		int fdo=1,fdw=1;
	//	for(ja=0;ja<ia;ja++)printf("sign%c \tfile %s\n",sign[ja],fname[ja]);
		for(ja=0;ja<count;ja++)
		{
			if(sign[ja]=='<')
			{
				fdo=open(fname[ja],O_RDONLY);
				if(fdo==-1)break;
			}
			else if(sign[ja]=='>')
			{
				int fdd=open(fname[ja],O_WRONLY);
				if(fdd!=-1)
				{
					pid_t pidu;
					int status;				  /*----------------------*/
					char *cm[]={"rm",fname[ja],NULL};         /* this whole procedure */
					pidu=fork();                              /* is to create a blank */
					if(pidu==0)				  /* which appears after  */
					{					  /*      the sign >      */
						execvp(*cm,cm);                   /*e.g ls | grep a > 1.in*/
						_exit(0);			  /*initially a blank file*/
					}					  /* 1.in is created      */
					else					  /*----------------------*/
					{
						while(wait(&status)!=pidu)
							;
					}
				}

				
				fdw=open(fname[ja], O_WRONLY | O_CREAT, S_IRWXU);
		}
		}
		if(fdo==-1)
		{
			printf("file %s doesnot exist\n",fname[ja]); /*if input file is not there error is thrown */
			break;
		}
		else
			{
				char a[100][100];
				int i,j;
				for(i=0;i<100;i++)
				{
				for(j=0;j<100;j++)
				{
				a[i][j]='\0';
				}
				}
		char *cmd1=strtok(cmdname,DELIMS);
		strcpy(a[0],cmd1);
		char *s;
		for(i=1;i<100;i++)
		{
			
			s=strtok(NULL,DELIMS);
			if(!s)break;
			strcpy(a[i],s);
		}
		char *cm[100];

		for(j=0;j<i;j++)
		{
			cm[j]=a[j];
		}
		cm[i]=NULL;
		pipe(fd);
		if(kp<cnt-1)
		{
			int status;
		out=fd[1];                      /*-------------------------------------------------------*/
		if((pid=fork())==0)   		/* In this step, first the command is executed depending */
		{		      		/* on input and output                                   */
//printf("in %d\t out %d\n",in,out);  		/* Then the output is written to the pipe fd[1]          */
//printf("dforking\n");               		/*from there it is read by fd[0]                         */
			if(in!=0)     		/*next command is executed with input that is read from  */
			{             		/*pipe again written to fd[1] of next pipe.This continues*/            
				dup2(in,0);     /*until all the pipes are covered.                       */
				close(in);      /*-------------------------------------------------------*/
			}
			if(out!=1)
			{
				dup2(out,1);
				close(out);
			}
	//		printf("ksmall %d\t kbig%d\n",ksmall,kbig);
			if(ksmall==0 && kbig==0 && kapp==0)
		{
			pid_t pid1;
			int status1;
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);


				
	
		}
		else if(ksmall==0 && kapp==0 && kbig!=0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			dup2(fad,1);
			close(fad);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

			

		}
		else if(kbig==0 && kapp==0 && ksmall!=0)
		{
			pid_t pid1;
			int status1;
	//		printf("filenam%s\n",fname[asmall]);
			int fad=open(fname[asmall],O_RDONLY);
				dup2(fad,0);
				close(fad);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

				
		}
		else if(ksmall==0 && kapp!=0 && kbig==0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			dup2(fad,1);
			close(fad);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

		

		}
		else if(ksmall!=0 && kbig!=0 && kapp==0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[asmall],O_RDONLY);
			int fad1=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
				dup2(fad,0);
				close(fad);
				dup2(fad1,1);
				close(fad1);
					if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

		}
		else if(ksmall!=0 && kbig==0 && kapp!=0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[asmall],O_RDONLY);
			int fad1=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
				dup2(fad,0);
				close(fad);
				dup2(fad1,1);
				close(fad1);
					if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

				
		}
		else if(ksmall==0 && kapp!=0 && kbig!=0)
		{
			pid_t pid1;
			int status1,fad;
			if(aapp>abig)
			fad=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			else if(abig>aapp)
			fad=open(fname[abig],O_WRONLY | O_CREAT , S_IRWXU);
			dup2(fad,1);
			close(fad);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

			

		}
		else if(ksmall!=0 && kbig!=0 && kapp!=0)
		{
			pid_t pid1;
			int status1,fad1;
			int fad=open(fname[asmall],O_RDONLY);
			if(abig>aapp)
			fad1=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			else if(aapp>abig)
			fad1=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT , S_IRWXU);
				dup2(fad,0);
				close(fad);
				dup2(fad1,1);
				close(fad1);
					if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

				
		}
		}
		else
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;


				}

	//	}
		close(fd[1]);
		in=fd[0];    //in and out are basically input and output descriptors for the pipe
	//	printf("fd %d  in %d\n",fd[0],in);
		}
		else
		{
	//		printf("in%d\t out %d\n",in,out);
			if(in!=0)dup2(in,0);
			out=1;
			dup2(out,1);
			int status;
			if((pid=fork())==0)
			{
		if(ksmall==0 && kbig==0 && kapp==0)
		{
			pid_t pid1;
			int status1;
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);


				
	
		}
		else if(ksmall==0 && kapp==0 && kbig!=0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			dup2(fad,1);
			close(fad);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);


		}
		else if(kbig==0 && kapp==0 && ksmall!=0)
		{
			pid_t pid1;
			int status1;
	//		printf("filenam%s\n",fname[asmall]);
			int fad=open(fname[asmall],O_RDONLY);
				dup2(fad,0);
				close(fad);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

	
		}
		else if(ksmall==0 && kapp!=0 && kbig==0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			dup2(fad,1);
			close(fad);
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

	

		}
		else if(ksmall!=0 && kbig!=0 && kapp==0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[asmall],O_RDONLY);
			int fad1=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
				dup2(fad,0);
				close(fad);
				dup2(fad1,1);
				close(fad1);
				execvp(*cm,cm);
				_exit(0);
		}
		else if(ksmall!=0 && kbig==0 && kapp!=0)
		{
			pid_t pid1;
			int status1;
			int fad=open(fname[asmall],O_RDONLY);
			int fad1=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
				dup2(fad,0);
				close(fad);
				dup2(fad1,1);
				close(fad1);
				execvp(*cm,cm);
				_exit(0);
		}
		else if(ksmall==0 && kapp!=0 && kbig!=0)
		{
			pid_t pid1;
			int status1,fad;
			if(aapp>abig)
			fad=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			else if(abig>aapp)
			fad=open(fname[abig],O_WRONLY | O_CREAT , S_IRWXU);
			dup2(fad,1);
			close(fad);
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

	

		}
		else if(ksmall!=0 && kbig!=0 && kapp!=0)
		{
			pid_t pid1;
			int status1,fad1;
			int fad=open(fname[asmall],O_RDONLY);
			if(abig>aapp)
			fad1=open(fname[abig],O_WRONLY | O_CREAT, S_IRWXU);
			else if(aapp>abig)
			fad1=open(fname[aapp],O_WRONLY | O_APPEND | O_CREAT , S_IRWXU);
				dup2(fad,0);
				close(fad);
				dup2(fad1,1);
				close(fad1);
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

			
		}

			
			}
			else 
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}

		//	close(fd[0]);
		close(fd[1]);
	
		}
		
	}
	//	printf("------------\n");
	}
	
		dup2(1,res);
	}

//------------------------------------------------------------------------------------------------------------------//
	else if(strchr(ln2,'|'))/* if condition for case 2 of assignment 2. All the variable are more or less       */
	{                       /* the same operation as in previous if . Basic functionality remains same          */
		his_flag=0;     /*----------------------------------------------------------------------------------*/
		int res;
		dup2(res,0);
		char dup[1000];
		strcpy(dup,ln2);
		int cnt=0;
		char *tkn=strtok(ln2,"|");
		while(tkn!=NULL)
		{	
			cnt++;
			tkn=strtok(NULL,"|");

		}
		char pa1[100];
		int x,y;
		pid_t pid;
		int kp=1,val=0,in=0,out,fd[2];
		for(kp=0;kp<cnt;kp++)
		{
			for(x=0;x<100;x++)pa1[x]='\0';
			int i,j=0;
			for(i=val;i<strlen(dup);i++)
			{
				if(dup[i]=='|')break;
				pa1[j]=dup[i];
				j++;
			}
			val=i+1;
	
		char a[100][100];
		for(i=0;i<100;i++)
		{
			for(j=0;j<100;j++)
			{
				a[i][j]='\0';
			}
		}
		char *pa2;
		pa2=strtok(pa1," ");
		strcpy(a[0],pa2);
		for(i=1;i<100;i++)
		{
			pa2=strtok(NULL," ");
			if(!pa2)break;
			strcpy(a[i],pa2);
		}
		char *cm[100];

		for(j=0;j<i;j++)
		{
			cm[j]=a[j];
		}
		cm[i]=NULL;
		pipe(fd);
		if(kp<cnt-1)
		{
			int status;
	//	pipe(fd);
		out=fd[1];
		if((pid=fork())==0)
		{
			if(in!=0)
			{
				dup2(in,0);
				close(in);
			}
			if(out!=1)
			{
				dup2(out,1);
				close(out);
			}
			if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

		
		//	if(execvp(*cm,cm)<0)printf("command not found\n");
		//	_exit(0);
		}
		else 
		{
		//	printf("%d\n",pid);
			strcpy(hs1[hsin]->nm,cm[0]);
			hs1[hsin]->pid=pid;
			hsin++;
			while(wait(&status)!=pid)
				;
		}
		close(fd[1]);
		in=fd[0];
		}
		else
		{
//	wait(NULL);
			if(in!=0)dup2(in,0);
			int status;
			if((pid=fork())==0)
			{
				if(strcmp(cm[0],"hist")==0)histr();
			else if(cm[0][0]=='h' && cm[0][1]=='i' && cm[0][2]=='s' && cm[0][3]=='t' && strlen(cm[0])>4 && cm[0][4]!='o')	lashis(cm[0]);
			else if(strcmp(cm[0],"pid")==0)
			{
				if(!cm[1])ppid1();
				else if(strcmp(cm[1],"all")==0)
				{
					ppid2();}
				else if(strcmp(cm[1],"current")==0)ppid3();
			}
			
			
			
			else
				execvp(*cm,cm);
			_exit(0);

			//execvp(*cm,cm);
			//_exit(0);
			}
			else 
			{
				strcpy(hs1[hsin]->nm,cm[0]);
				hs1[hsin]->pid=pid;
				hsin++;
				while(wait(&status)!=pid)
					;
			}

		//	close(fd[0]);
		close(fd[1]);
	
		}
//		else return pid;
//		pa3=strtok(NULL,"|");
		//cmd1[kp]=cm;
		
	}
		dup2(1,res);
	
		//if(in!=0)dup2(in,0);
	//	printf("xxxx\n");
		//exe
	
	}
	
		
//---------------------------------------------------------------------------------------------------------//
	else if(strcmp(cmd,"cd")==0)// cd command has to be handled separately using chdir
	{
		his_flag=0;
		char *op=strtok(NULL,DELIMS); //op stores the directory path given after cd
char *h=getenv("PWD");
		char f;
		if(!op)
		{
			char *hm=getenv("PWD");//hm stores path of present working directory retrieved from environment variable
		chdir(hm);
			
		}
		else 
		{
			char f=op[0];
			if(f=='~' || (f=='.' & strcmp(op,"..")!=0) )
			{

	char *hm=getenv("PWD");
				
				chdir(hm);
			}
			else if(strcmp(op,"..")==0 && strcmp(p1,p2)==0 )
			{
				char *hm=getenv("PWD");
				chdir(hm);
				
			
			}
			
			else chdir(op);
	}}
//------------------------------------------------------------------------------------------------------------------
	else if(strcmp(cmd,"quit")==0)/* if user enters quit,loop breaks and program exits */
	{                             /*his_flag=1 only when user enters !histn            */
		his_flag=0;
	
		break;
	}
//------------------------------------------------------------------------------------------------------------------	
	else if(strcmp(cmd,"hist")==0)//hist command will print array h[i] which stores history of commands
	{
		his_flag=0;
		int i;
		for(i=0;i<k-1;i++)printf("%d. %s\n",(i+1),h[i]);
	}
//-------------------------------------------------------------------------------------------------------------------
	else if(cmd[0]=='h' && cmd[1]=='i' && cmd[2]=='s' && cmd[3]=='t' && strlen(cmd)>4 && cmd[4]!='o')
	{
		his_flag=0;                             //-------------------------------------------//
		int ln=0,cv;                            // histn command: here n is sored in ln      //
		//printf("%s\n",cmd);			//e.g. hist15 : ln stores 15                 //
		for(cv=4;cv<strlen(cmd);cv++)           //the last loop to print last n commands     //
		{     					//-------------------------------------------//
			ln=(ln*10)+(int)(cmd[cv]-'0');	
		}
		//printf("%d %d\n",k,ln);
		int sp=1;
		for(cv=k-ln+-2;cv<k-2;cv++)
		{
			printf("%d. %s\n",sp,h[cv]);
			sp++;
		}
	}
//----------------------------------------------------------------------------------------------------------------------
	else if(cmd[0]=='!' && cmd[1]=='h' && cmd[2]=='i' && cmd[3]=='s' && cmd[4]=='t' && strlen(cmd)>5)
	{
		his_flag=1;
		int ln=0,cv;                           //-------------------------------------------//
		for(cv=5;cv<strlen(cmd);cv++)	       //!histn command : ln stores n               //
		{				       //his_flag=1                                 //
			ln=(ln*10)+(int)(cmd[cv]-'0'); //-------------------------------------------//
		}
		strcpy(hiso,h[ln-1]);
		hiso[strlen(hiso)]='\0';
	}
//-----------------------------------------------------------------------------------------------------------------------
		else if(strcmp(cmd,"pid")==0) //pid : inbuilt command
	{
		his_flag=0;
		char *op=strtok(NULL,DELIMS);
		if(!op)
                {
			printf("command name: ./a.out process id:%d\n",(int)getpid());
		}

		else if(strcmp(op,"all")==0)
		{
			printf("List of all processes spawned from this shell:\n");
			fl1++;
			int ii;
			for(ii=0;ii<hsin;ii++)
			{
				printf("command name: %s process id:%d\n",hs1[ii]->nm,hs1[ii]->pid);
			}
		}
		else if(strcmp(op,"current")==0)
		{
			printf("List of currently executing processes spawned from this shell:\n");
			int kl;
			for(kl=0;kl<k2;kl++)
			{
				if(flag[kl]==1)
				printf("command name: %s process id:%d\n",curr[kl],cd[kl]);
			}
		}
	}
//-------------------------------------------------------------------------------------------------------------------	

	else
	{
		his_flag=0;                   //----------------------------------------------------------//
		char a[100][100];             // time to execute inbuilt commands using exec and fork     //
		int i,j;                     // cm array sores the pointer to main command and their flags//
		for(i=0;i<100;i++)           //if process is background, parent doesnot wait for child    //
		{                            //if process is foreground , parent wait until child         //
			for(j=0;j<100;j++)   //has completed the task                                     //
			{                    //-----------------------------------------------------------//
				a[i][j]='\0';
			}
		}
		strcpy(a[0],cmd);
		char *s;
		for(i=1;i<100;i++)
		{
			
			s=strtok(NULL,DELIMS);
			if(!s)break;
			strcpy(a[i],s);
		}
		if(a[i-1][strlen(a[i-1])-1]=='&')
{
		a[i-1][strlen(a[i-1])-1]='\0';
		pid_t pid;
		int status;
		char *cm[100];

		for(j=0;j<i;j++)
		{
			//printf("%s\n",a[j]);
			cm[j]=a[j];
			//strcpy(&cm[j],a[j]);
		}
		cm[i]=NULL;
		//strcpy(&cm[i],NULL);
		pid=fork();
		if(pid==0)
		{
		
			int k=execvp(*cm,cm);
			if(k<0)printf("Command not found\n");
			_exit(0);
		
			
		}
		else 
		{

			strcpy(hs1[hsin]->nm,cmd);
			hs1[hsin]->pid=pid;
			hsin++;

			key++;
			pdp[k1]=pid;
			strcpy(pdpnm[k1],cmd);
			k1++;
			printf("command %s pid %d\n",cmd,pdp[k1-1]);
			strcpy(curr[k2],cmd);
			flag[k2]=1;
			cd[k2]=(int)pid;
			k2++;
			signal(SIGCHLD, child_handler);
		//	sigset_t set;
		//	sigemptyset(&set);
		//	sigaddset(&set, SIGCHLD);
		//	sigprocmask(SIG_BLOCK, &set, NULL);
			//signal(SIGCHLD,SIG_DFL); /* ignore child */

		}

	}

	else
	{	pid_t pid;
		int status;
		char *cm[100];

		for(j=0;j<i;j++)
		{
			//printf("%s\n",a[j]);
			cm[j]=a[j];
			//strcpy(&cm[j],a[j]);
		}
		
		cm[i]=NULL;
		//strcpy(&cm[i],NULL);
		pid=fork();
		if(pid==0)
		{
			
			 

			int k=execvp(*cm,cm);
			if(k<0)printf("Command not found\n");
			_exit(0);
		
			
		}
		else 
		{
			strcpy(hs1[hsin]->nm,cmd);
			hs1[hsin]->pid=pid;
			hsin++;
	//		fl2++;
			key++;
//			printf("parent\n");
			pdp[k1]=pid;
			strcpy(pdpnm[k1],cmd);
			k1++;
			while(wait(&status)!=pid)
				;

		}

	}}
	}
//char *st;
//char ne[1000];
}
return 0;
}
void ppid1() //pid : inbuilt command
	{
		his_flag=0;
		//char *op=strtok(NULL,DELIMS);

                
			printf("command name: ./a.out process id:%d\n",(int)getpid());
		}

void ppid2()		
		{
			printf("List of all processes spawned from this shell:\n");
			fl1++;
			int ii;
			for(ii=0;ii<hsin;ii++)
			{
				printf("command name: %s process id:%d\n",hs1[ii]->nm,hs1[ii]->pid);
			}
		}
void ppid3()
		{
			printf("List of currently executing processes spawned from this shell:\n");
			int kl;
			for(kl=0;kl<k2;kl++)
			{
				if(flag[kl]==1)
				printf("command name: %s process id:%d\n",curr[kl],cd[kl]);
			}
		}
	



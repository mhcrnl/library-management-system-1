#include<stdio.h>
#include<string.h>
#include<termios.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

#define ADMIN 1
#define NORMAL_USER 0
#define FORBIDDEN -1
#define MAXUSERNAME 100
#define MAXPASSWD 100
#define MAXPHONENUM 20
#define MAXBOOKNAME 100
#define MAXNAME 100
#define MAXDATELEN 100
#define ECHOFLAGS (ECHO|ECHOE|ECHOK|ECHONL)

typedef struct user{
  char username[MAXUSERNAME];
  char password[MAXPASSWD];
  char phonenum[MAXPHONENUM];
  char sex[10];
  int id;
  int age;
}User;
typedef struct usernode{
  User user;
  struct usernode *next;
}Usernode,*ulink;

typedef struct book{
  char name[MAXBOOKNAME];
  char author[MAXNAME];
  int totalcount;
  int borrowcount;
  int id;
}Book;
typedef struct booknode{
  Book book;
  struct booknode *next;
}Booknode,*blink;

typedef struct borrow{
  char borrowdate[MAXDATELEN];
  int borrow_id;
  char username[MAXUSERNAME];
  char bookname[MAXBOOKNAME];
}Borrow;

typedef struct borrownode{
  Borrow borrow;
  struct Borrownode *next;
}Borrownode,*borrowlink;

typedef struct loginnode{
  int usertype;
  char username[MAXUSERNAME];
}Loginnode,*loginlink;

loginlink login(void);
int authorize(char [],char []);
int getpassword(char [],int);
int setdisplay_mode(int,int);
void admin_UI(char []);
void normaluser_UI(char []);
ulink searchuserlist(ulink ,char [],char []);
int getusername(char [],int);
void init_userlist(void);

static ulink adminlist;
static ulink normallist;

int main()
{
  loginlink llink=NULL;
  while(1){
  free(llink);
  llink=login();
  if(llink->usertype==ADMIN)
    admin_UI(llink->username);
 else if(llink->usertype==NORMAL_USER)
    normaluser_UI(llink->username);
 else
    printf("Login Fail , Can't find a match in user DB\n");
}
return 0;
}

void init_userlist()
{
  adminlist=NULL;
  normallist=NULL;
  adminlist=(ulink)malloc(sizeof(Usernode));
  strcpy(adminlist->user.username,"admin");
  strcpy(adminlist->user.password,"czj050507");
  adminlist->next=NULL;

}

void admin_UI(char username[])
{
  int c,flag=1;
  printf("############# Menu for user '%s' #############\n",username);
  printf("1. User Management\n2. Book Management\n3. Borrow Management\n");
  printf("Which service do you like?[ 1 ]\n");
  c=getchar();
  while(flag--){
  switch(c){
    case '\n':
    case '1':
      user_manageUI(username);
      break;
    case '2':
      book_manageUI();
      break;
    case '3':
      borrow_manageUI();
      break;
    default:
      printf("Please input an valid option\n");
      flag=1;
      break;
  }
  }
 }

void user_management(char username[])
{ int choice;
  printf("############# User Management Menu for user '%s' #############\n");
  printf("1.Add user[A]\n2.Delete user[D]\n3.Search user[S]\n4.Modify user[M]\n");
  choice=getchar();
  switch(choice)
  {
    case '1':
    case 'a':
    case 'A':
         adduser();
  }

}


void normaluser_UI(char username[])
{
  printf("############# Menu for user '%s' #############\n",username);
}

loginlink login()
{
   char username[MAXUSERNAME];
   char password[MAXPASSWD];
   int len,usertype;
   loginlink llink = NULL;
   printf("******************** Welcome to Library Management System *******************\n");
   getusername(username,MAXUSERNAME);
   getpassword(password,MAXPASSWD);
   printf("username is %s\n",username);
   printf("password is %s\n",password);
   usertype = authorize(username,password);
   llink = (loginlink)malloc(sizeof(Loginnode));
   llink->usertype=usertype;
   strcpy(llink->username,username);
   return llink;
}

int getusername(char username[],int maxusername)
{
  int i=0;
  char c;
  //scanf("%s",username);
  printf("Username:");
  while(((c=getchar())!=EOF)&&(i<maxusername-1) && c!='\n')
  {
     if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))
         username[i++]=c;
     else
         continue;
  }
  username[i]='\0';
  return i;

}

int authorize(char username[],char password[])
{
  static ulink adminptr,normalptr;
  int usertype=FORBIDDEN;
  init_userlist();

  //normallist=(ulink)malloc(size(Usernode));
  //normallist->user.username="oscar";
  //normallist->user.password="oscar050513";
  //adminlist->next=NULL;

  adminptr=searchuserlist(adminlist,username,password);
  if(adminptr)
  {  printf("user %s found in adminlist\n",username);
     usertype=ADMIN;
       }
  else
  {
   normalptr=searchuserlist(normallist,username,password);
   if(normalptr)
   {
      printf("username %s found in normaluserlist\n",username);
      usertype=NORMAL_USER;
   }

  }

   return usertype;
}

ulink searchuserlist(ulink userlist,char name[],char password[])
{
  ulink userptr=userlist;
  while(userptr!=NULL){
     if(strcmp(userptr->user.username,name)==0 && strcmp(userptr->user.password,password)==0)
         break;
     userptr=userptr->next;
}
  return userptr;
}


int getpassword(char passwd[],int size)
{
 int c,i=0;
 printf("Password:");
 setdisplay_mode(STDIN_FILENO,0);
 while(((c=getchar())!=EOF)&&(i<MAXPASSWD-1) && c!='\n')
 {
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))
        passwd[i++]=c;
    else
        continue;
 }
 passwd[i]='\0';
 //scanf("%s",passwd);
 setdisplay_mode(STDIN_FILENO,1);
 putchar('\n');
 return strlen(passwd);

}

int setdisplay_mode(int fd,int flag)
{
   int ret;
   struct termios term;
   if(tcgetattr(fd,&term)==-1)
   {
     perror("can't get the attribute of the terminal");
     return 1;
   }
   if(flag)
     term.c_lflag|=ECHOFLAGS;
   else
     term.c_lflag&=~ECHOFLAGS;
   ret=tcsetattr(fd,TCSAFLUSH,&term);
   if(ret==-1||ret==EINTR)
   {  perror("can't set the attributes of terminal");
      return 1;
    }
    return 0;
}

#include<stdio.h>
#include<string.h>
#include<termios.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

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
  int usertype;
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
  int borrow_counts;
}Borrow;

typedef struct borrownode{
  Borrow borrow;
  struct borrownode *next;
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
void init(void);
void user_insert(ulink,int);
void delete_user(void);
void add_user(void);
void modify_user(int);
int search_user(void);
void user_display(ulink);
ulink search_user_by_name(char []);
ulink search_user_by_id(int);
int user_delete_by_name(char []);
int user_delete_by_id(int);
void user_manageUI(char []);
void book_manageUI(char []);
void borrow_manageUI(char []);
void add_book(void);
void book_insert(blink *,blink);
void delete_book(void);
int book_delete_by_name(char []);
int book_delete_by_id(int);
int search_book(void);
blink search_book_by_name(char []);
blink search_book_by_id(int);
void book_display(blink);
void modify_book(void);
int borrow_book(void);
int return_book(void);
int search_borrow(void);
void borrow_insert(borrowlink *,borrowlink);
char *get_time(void);
borrowlink search_borrow_by_username(char []);
void borrow_display(borrowlink);
int modify_user_by_ulink(ulink);

static ulink adminlist;
static ulink normallist;
static blink booklist;
static borrowlink borrowlist;
static int user_id;

int main()
{ int c;
  loginlink llink=NULL;
  init();
  while(1){
  free(llink);
  llink=login();
  if(llink->usertype==ADMIN)
    admin_UI(llink->username);
  else if(llink->usertype==NORMAL_USER)
    normaluser_UI(llink->username);
  else
    printf("Login Fail , Can't find a match in user DB\n");
  printf("1. Switch user account[S|s]\n");
  printf("2. Exit?[E|e]\n");
  c = getchar();
  if(c=='E'||c=='e'||c=='2')
    break;
  else
    getchar();
}
return 0;
}

void init()
{
  adminlist=NULL;
  normallist=NULL;
  booklist=NULL;
  borrowlist=NULL;
  adminlist=(ulink)malloc(sizeof(Usernode));
  strcpy(adminlist->user.username,"admin");
  strcpy(adminlist->user.password,"czj050507");
  adminlist->user.id=1;
  adminlist->next=NULL;
}


void admin_UI(char username[])
{
  int c,flag=1;

  while(flag){
  printf("############# Menu for user '%s' #############\n",username);
  printf("1. User Management\n2. Book Management\n3. Borrow Management\n4. Exit\n");
  printf("Which service do you like?[ 1 ]\n");
  c=getchar();
  getchar();
  switch(c){
    case '\n':
    case '1':
      user_manageUI(username);
      break;
    case '2':
      book_manageUI(username);
      break;
    case '3':
      borrow_manageUI(username);
      break;
    case '4':
      flag=0;
      break;
    default:
      printf("Please input an valid option\n");
      break;
  }
  }
 }
void modify_book()
{

  char name[MAXBOOKNAME];
  char author[MAXNAME];
  int totalcount,borrowcount;
  int id,option,flag,choice;
  blink booklink=NULL;
  flag=1;
  while(flag--){
  printf("Locate book by name[N] or id[I]:");
  option=getchar();
  getchar();
  switch(option)
  {
     case 'N':
     case 'n':
         printf("Bookname: ");
         scanf("%[^\n]",name);
         getchar();
         booklink=search_book_by_name(name);
         break;
     case 'I':
     case 'i':
         printf("Book ID:\n");
         scanf("%d",&id);
         getchar();
         booklink=search_book_by_id(id);
         break;
    case 'q':
    case 'Q':
         break;
     default:
        printf("Unknown option\n");
        flag=1;
        break;
  }

}
if(booklink==NULL)
  printf("Book not found\n");
else
{
  printf("Change info for Book %s\n",booklink->book.name);
  printf("Book name: %s ,[m/M] to modify ,other to skip\n",booklink->book.name);
  choice=getchar();

  if(choice=='m'||choice=='M')
  {
    getchar();
    printf("new book name is: ");
    scanf("%[^\n]",name);
    strcpy(booklink->book.name,name);
    getchar();
  }

  printf("Author: %s ,[m/M] to modify ,other to skip\n",booklink->book.author);
  choice=getchar();

  if(choice=='m'||choice=='M')
  {
    getchar();
    printf("new author is: ");
    scanf("%[^\n]",author);
    strcpy(booklink->book.author,author);
    getchar();
  }

  printf("Total count: %d ,[m/M] to modify ,other to skip\n",booklink->book.totalcount);
  choice=getchar();

  if(choice=='m'||choice=='M')
  {
    getchar();
    printf("new totalcount is: ");
    scanf("%d",&totalcount);
    booklink->book.totalcount=totalcount;
    getchar();
  }
  printf("Borrow count: %d ,[m/M] to modify ,other to skip\n",booklink->book.borrowcount);
  choice=getchar();

  if(choice=='m'||choice=='M')
  {
    getchar();
    printf("new borrowcount is: ");
    scanf("%d",&borrowcount);
    booklink->book.borrowcount=borrowcount;
    getchar();
  }

  printf("User info modification done\nnew user info is:\n");
  book_display(booklink);

  }
}

void add_book()
{
  char name[MAXBOOKNAME];
  char author[MAXNAME];
  int totalcount;
  blink booklink=NULL;

    printf("Please input book info that you want to add:\n");
    printf("Bookname: ");
    scanf("%[^\n]",name);
    getchar();
    printf("Author: ");
    scanf("%[^\n]",author);
    getchar();
    printf("Total book counts: ");
    scanf("%d",&totalcount);
    getchar();

    booklink=(blink)malloc(sizeof(Booknode));
    strcpy(booklink->book.name,name);
    strcpy(booklink->book.author,author);
    booklink->book.totalcount=totalcount;
    booklink->next=NULL;

    book_insert(&booklist,booklink);
    printf("book '%s' successfully added to book list\n",booklink->book.name);

  }

  void delete_book()
  {
    char name[MAXBOOKNAME];
    blink booklink=NULL;
    int option,id;

    while(1)
    {
      printf("Select book by ID[I] or by bookname[N]?\n");
      option=getchar();
      getchar();
      if(option=='N' || option=='n')
      {  printf("Please input bookname that you want to delete: ");
         scanf("%[^\n]",name);
         getchar();
         book_delete_by_name(name);
          break;
      }
    else if(option=='I'||option=='i')
    {
      printf("Please input book ID that you want to delete:\n");
      scanf("%d",&id);
      getchar();
      book_delete_by_id(id);
      break;

    }
    else
      printf("Unknown option\n");

  }
    }

int book_delete_by_id(int id)
{
  blink booklink,temp;
  booklink=booklist;
  //check if it is the head
  if(booklist==NULL)
    printf("No book in the book list\n");
  if(booklink->book.id==id)
  {
  temp=booklink;
   if(booklink->next==NULL)
     booklist=NULL;
  else
  {   booklist=booklink->next;
      booklink=booklink->next;
    }
  printf("book %s delete successfully\n",temp->book.name);
  free(temp);
  return 0;
  }

  while(booklink->next!=NULL)
  {
    temp=booklink;
    booklink=booklink->next;
    if(booklink->book.id==id)
    {
      temp->next=booklink->next;
      printf("book delete successfully\n");
      free(booklink);
      return 0;
    }

  }
  printf("book not found in booklist\n");
  return -1;
}

int book_delete_by_name(char name[])
{
  blink booklink,temp;
  booklink=booklist;
  //check if it is the head
  if(booklist==NULL)
    printf("No book in the list\n");
  if(strcmp(booklink->book.name,name)==0)
  {
   temp=booklink;
   if(booklink->next==NULL)
     booklist=NULL;
  else
     booklist=booklink->next;
  free(temp);
  printf("Book delete successfully\n");
  return 0;
  }

  while(booklink->next!=NULL)
  {
    temp=booklink;
    booklink=booklink->next;
    if(strcmp(booklink->book.name,name)==0)
    {
      temp->next=booklink->next;
      free(booklink);
      printf("Book delete successfully\n");
      return 0;
    }

  }
  printf("Book not found\n");
  return -1;
}

int search_book()
{
  char name[MAXBOOKNAME];
  int id,option,flag;
  blink book;
  flag=1;
  while(flag--){
  printf("Search book by name[N] or id[I]:");
  option=getchar();
  getchar();
  switch(option)
  {
     case 'N':
     case 'n':
         printf("Please input bookname:");
         scanf("%[^\n]",name);
         getchar();
         book=search_book_by_name(name);
         break;
     case 'I':
     case 'i':
         printf("Please input book ID:\n");
         scanf("%d",&id);
         getchar();
         book=search_book_by_id(id);
         break;
     default:
        printf("Unknown option\n");
        flag=1;
        break;
  }
}
   if(book)
   {
     printf("Book is found\n");
     return 0;
    }
   else
   return -1;
}

blink search_book_by_name(char name[])
{
  blink booklink;
  booklink=booklist;
  //check if it is the head

  while(booklink!=NULL)
  {

    if(strcmp(booklink->book.name,name)==0)
    {
      book_display(booklink);
      return booklink;
    }
    booklink=booklink->next;
  }
  printf("book '%s' not exist!\n",name);
  return NULL;
}

blink search_book_by_id(int id)
{
  blink booklink;
  booklink=booklist;
  //check if it is the head
  while(booklink!=NULL)
  {

    if(booklink->book.id==id)
    {
      book_display(booklink);
      return booklink;
    }
  booklink=booklink->next;
  }
  printf("book id %d not exist!\n",id);
  return NULL;
}

void book_display(blink booklink)
{

  if(booklink==NULL)
   printf("book not exist\n");
  else
  {
      printf("Book ID: %d\n",booklink->book.id);
      printf("Bookname: %s\n",booklink->book.name);
      printf("Author: %s\n",booklink->book.author);
      printf("Total book count: %d\n",booklink->book.totalcount);
      printf("Borrow count: %d\n",booklink->book.borrowcount);
  }
}

void borrow_display(borrowlink borrlink)
{

  if(borrlink==NULL)
   printf("borrow record not exist\n");
  else
  {
      printf("Borrow ID: %d\n",borrlink->borrow.borrow_id);
      printf("Borrow Date: %s\n",borrlink->borrow.borrowdate);
      printf("User: %s\n",borrlink->borrow.username);
      printf("Bookname: %s\n",borrlink->borrow.bookname);
      printf("Borrow count: %d\n",borrlink->borrow.borrow_counts);
  }
}


 void book_manageUI(char username[])
 {
   int choice,flag=1;
   while(flag--){
   printf("############# Book Management Menu for user '%s' #############\n",username);
   printf("1.Add book[A]\n2.Delete book[D]\n3.Search book[S]\n4.Modify book[M]\n5. Go to upper level[E]\n");
   choice=getchar();
   printf("choice is %c\n",choice);
   switch(choice)
   {
     case '1':
     case 'A':
     case 'a':
       getchar();
       add_book();
       flag=1;
       break;
     case '2':
     case 'D':
     case 'd':
       getchar();
       delete_book();
       flag=1;
       break;
     case '3':
     case 'S':
     case 's':
       getchar();
       search_book();
       flag=1;
       break;
     case '4':
     case 'M':
     case 'm':
       getchar();
       modify_book();
       flag=1;
       break;
     case '5':
     case 'E':
     case 'e':
       getchar();
       break;
     default:
       printf("Please input a valid option\n");
       flag=1;
       break;
   }

 }
 }

int borrow_book()
{
  char name[MAXBOOKNAME];
  char username[MAXUSERNAME];
  int counts;
  blink booklink=NULL;
  borrowlink borrlink=(borrowlink)malloc(sizeof(Borrownode));

    printf("Please input book name that you want to borrow:\n");
    printf("Bookname: ");
    scanf("%[^\n]",name);
    getchar();
    booklink = search_book_by_name(name);
    if(booklink)
    {
      printf("Total count %d , %d books already borrowed out for book '%s' \n",booklink->book.totalcount,booklink->book.borrowcount,name);
    if(booklink->book.borrowcount==booklink->book.totalcount)
      printf("Book '%s' all borrowed out\n",name);
    else
    {
      while(1)
      {
      printf("Please input your name: ");
      scanf("%[^\n]",username);
      getchar();
      printf("How many copies you want to borrow: ");
      scanf("%d",&counts);
      getchar();
      if(booklink->book.totalcount-booklink->book.borrowcount<counts)
         printf("There is so many copies left\n");
      else
         break;
       }
      strcpy(borrlink->borrow.username,username);
      strcpy(borrlink->borrow.bookname,name);
      borrlink->borrow.borrow_counts=counts;
      booklink->book.borrowcount+=counts;
      borrlink->next=NULL;

    borrow_insert(&borrowlist,borrlink);
    printf("book '%s' successfully borrowed to '%s' \n",booklink->book.name,username);
  }
  }
   return 1;
}

char *get_time(void)
{
  char *datestr=(char *)malloc(sizeof(char)*MAXDATELEN);
  time_t a;
  time(&a);
  strftime(datestr,MAXDATELEN,"%Y:%m:%d %H:%M:%S",localtime(&a));
  return datestr;
}

void borrow_insert(borrowlink * borrowlistptr,borrowlink borrlink)
{

  borrowlink temp;
  char borrowdate[MAXDATELEN];
  borrowlist=*borrowlistptr;
  char *timeptr;
  timeptr=get_time();
  strcpy(borrlink->borrow.borrowdate,timeptr);
  if(borrowlist==NULL)
  { borrowlist=borrlink;
    borrowlist->borrow.borrow_id=1;
    return;
  }
  for(temp=borrowlist;temp->next!=NULL;temp=temp->next)
     ;
  temp->next=borrlink;
  borrlink->borrow.borrow_id=temp->borrow.borrow_id+1;
}


int return_book(void)
{
  char username[MAXUSERNAME];
  borrowlink borrow;
  int counts;
  printf("Please input your username: ");
  scanf("%[^\n]",username);
  getchar();
  borrow=search_borrow_by_username(username);
  if(borrow)
  {
    printf("How many copies you want to return: ");
    scanf("%d",&counts);
    getchar();
    borrow->borrow.borrow_counts-=counts;
    printf("Book '%s' return successfully\n",borrow->borrow.bookname);
    return 0;
  }
  return -1;
}

int search_borrow(void)
{
  char username[MAXUSERNAME];
  int id,option,flag;
  borrowlink borrow;
  flag=1;
  //while(flag--){
  printf("Please input username: ");
  scanf("%[^\n]",username);
  getchar();
  borrow=search_borrow_by_username(username);
   if(borrow)
   {
     printf("Borrow record is found\n");
     return 0;
    }
   else
     return -1;
  return 0;
}

borrowlink search_borrow_by_username(char username[])
{
  borrowlink borrow;
  borrow=borrowlist;
  //check if it is the head

  while(borrow!=NULL)
  {

    if(strcmp(borrow->borrow.username,username)==0)
    {
      borrow_display(borrow);
      return borrow;
    }
    borrow=borrow->next;
  }
  printf("User '%s' borrow records not found!\n",username);
  return NULL;
}

 void borrow_manageUI(char username[])
 {
   int choice,flag=1;
   while(flag--){
   printf("############# Borrow Management Menu for user '%s' #############\n",username);
   printf("1.Borrow book[B]\n2.Return book[R]\n3.Search borrow[S]\n4. Go to upper level[E]\n");
   choice=getchar();
   printf("choice is %c\n",choice);
   switch(choice)
   {
     case '1':
     case 'B':
     case 'b':
       getchar();
       borrow_book();
       flag=1;
       break;
     case '2':
     case 'R':
     case 'r':
       getchar();
       return_book();
       flag=1;
       break;
     case '3':
     case 'S':
     case 's':
       getchar();
       search_borrow();
       flag=1;
       break;
     case '4':
     case 'E':
     case 'e':
       getchar();
       break;
     default:
       getchar();
       printf("Please input a valid option\n");
       flag=1;
       break;
   }

 }
 }

void user_manageUI(char username[])
{
  int choice,flag=1;
  while(flag--){
  printf("############# User Management Menu for user '%s' #############\n",username);
  printf("1.Add user[A]\n2.Delete user[D]\n3.Search user[S]\n4.Modify user[M]\n5. Go to upper level[E]\n");
  choice=getchar();
  printf("choice is %c\n",choice);
  switch(choice)
  {
    case '1':
    case 'A':
    case 'a':
      getchar();
      add_user();
      flag=1;
      break;
    case '2':
    case 'D':
    case 'd':
      getchar();
      delete_user();
      flag=1;
      break;
    case '3':
    case 'S':
    case 's':
      getchar();
      search_user();
      flag=1;
      break;
    case '4':
    case 'M':
    case 'm':
      getchar();
      modify_user(ADMIN);
      flag=1;
      break;
    case '5':
    case 'E':
    case 'e':
      getchar();
      break;
    default:
      printf("Please input a valid option\n");
      flag=1;
      break;
  }

}
}

void add_user()
{
  char username[MAXUSERNAME];
  char password[MAXPASSWD];
  char phonenum[MAXPHONENUM];
  char sex[10];
  int usernamelen,passwordlen,usertype,age;
  ulink userlink=NULL;
  usertype=FORBIDDEN;

  while(1)
  {
    printf("Please input user info that you want to add:\n");
    usernamelen=getusername(username,MAXUSERNAME);
    passwordlen=getpassword(password,MAXPASSWD);
    printf("USERTYPE:");
    scanf("%d",&usertype);
    printf("AGE:");
    scanf("%d",&age);
    printf("PHONENUMBER:");
    scanf("%s",phonenum);
    printf("SEX:");
    scanf("%s",sex);
    getchar();
    if(strlen(username)<6||passwordlen<6)
      printf("username and password should not be less than 6\n");
    else if(usertype!=ADMIN&&usertype!=NORMAL_USER)
      printf("usertype %d is not correct",usertype);
    else
    {
      userlink=(ulink)malloc(sizeof(Usernode));
      strcpy(userlink->user.username,username);
      strcpy(userlink->user.password,password);
      userlink->user.age=age;
      strcpy(userlink->user.phonenum,phonenum);
      strcpy(userlink->user.sex,sex);
      userlink->user.usertype=usertype;
      userlink->next=NULL;
      if(usertype==ADMIN)
      {  user_insert(userlink,ADMIN);
         printf("user '%s' successfully added to admin user list\n",userlink->user.username);
       }
      else if(usertype==NORMAL_USER)
      { user_insert(userlink,NORMAL_USER);
        printf("user '%s' successfully added to normal user list\n",userlink->user.username);
      }
      break;
    }
  }

}


void delete_user()
{
  char username[MAXUSERNAME];
  int usernamelen,option,id;
  ulink userlink=NULL;

  while(1)
  {
    printf("Select user by ID[I] or by username[U]?\n");
    option=getchar();
    getchar();
    if(option=='U' || option=='u')
    {  printf("Please input username that you want to delete:\n");
       usernamelen=getusername(username,MAXUSERNAME);
    if(usernamelen<6)
      printf("usernamelen should not be less than 6\n");
    else
    {
        user_delete_by_name(username);
        break;
    }
  }
  else if(option=='I'||option=='i')
  {
    printf("Please input user ID that you want to delete:\n");
    scanf("%d",&id);
    getchar();
    user_delete_by_id(id);
    break;

  }
  else
    printf("Unknown option\n");

}

}

int search_user()
{
   char username[MAXUSERNAME];
   int id,option,flag;
   ulink user;
   flag=1;
   while(flag--){
   printf("Search user by name[N] or id[I]:");
   option=getchar();
   getchar();
   switch(option)
   {
      case 'N':
      case 'n':
          getusername(username,MAXUSERNAME);
          user=search_user_by_name(username);
          break;
      case 'I':
      case 'i':
          printf("Please input user ID:\n");
          scanf("%d",&id);
          getchar();
          user=search_user_by_id(id);
          break;
      default:
         printf("Unknown option\n");
         flag=1;
         break;
   }
}
    if(user)
    {
      printf("User is found\n");
      return 0;
     }
    else
    return -1;
}

int modify_user_by_ulink(ulink user)
{
  char username[MAXUSERNAME];
  char password[MAXPASSWD];
  char phonenum[MAXPHONENUM];
  char sex[10];
  int id,option,flag,choice,usertype,age,old_usertype;
  if(!user)
    return -1;
  else
  {
    printf("Change info for user %s\n",user->user.username);
    printf("Usertype: %d ,[m/M] to modify ,other to skip\n",user->user.usertype);
    choice=getchar();
    if(choice=='m'||choice=='M')
    {
      printf("new usertype is: ");
      scanf("%d",&usertype);
      old_usertype=user->user.usertype;
      user->user.usertype=usertype;
      getchar();
    }

    printf("Password: %s ,[m/M] to modify ,other to skip\n",user->user.password);
    choice=getchar();
    if(choice=='m'||choice=='M')
    {
      printf("new password is: ");
      scanf("%s",password);
      strcpy(user->user.password,password);
      getchar();
    }
    printf("Phonenum: %s ,[m/M] to modify ,other to skip\n",user->user.phonenum);
    choice=getchar();
    if(choice=='m'||choice=='M')
    {
      printf("new phonenumber is: ");
      scanf("%s",phonenum);
      strcpy(user->user.phonenum,phonenum);
      getchar();
    }
    printf("Sex: %s ,[m/M] to modify ,other to skip\n",user->user.sex);
    choice=getchar();
    if(choice=='m'||choice=='M')
    {
      printf("new sex value is: ");
      scanf("%s",sex);
      strcpy(user->user.sex,sex);
      getchar();
    }
    printf("Age: %d ,[m/M] to modify ,other to skip\n",user->user.age);
    choice=getchar();
    if(choice=='m'||choice=='M')
    {
      printf("new age is: ");
      scanf("%d",&age);
      user->user.age=age;
      getchar();
    }
    printf("User info modification done\nnew user info is:\n");
    user_display(user);
    if(user->user.usertype!=old_usertype)
    {
      user_delete_by_name(user->user.username);
      if(user->user.usertype==ADMIN)
        user_insert(user,ADMIN);
      else
        user_insert(user,NORMAL_USER);
  }
    return 0;
    }
  }


void modify_user()
{
  char username[MAXUSERNAME];
  char password[MAXPASSWD];
  char phonenum[MAXPHONENUM];
  char sex[10];
  int id,option,flag,choice,usertype,age,old_usertype;
  ulink user=NULL;
  flag=1;

  while(flag--){
  printf("Locate user by name[N] or id[I]: ");
  option=getchar();
  getchar();
  switch(option)
  {
     case 'N':
     case 'n':
         getusername(username,MAXUSERNAME);
         user=search_user_by_name(username);
         break;
     case 'I':
     case 'i':
         printf("Please input user ID:\n");
         scanf("%d",&id);
         getchar();
         user=search_user_by_id(id);
         break;
    case 'q':
    case 'Q':
         break;
     default:
        printf("Unknown option\n");
        flag=1;
        break;
  }

}

if(user==NULL)
  printf("User not found\n");
else
  modify_user_by_ulink(user);
}

void user_display(ulink user)
{
  if(user==NULL)
   printf("user not exist\n");
  else
  {
      printf("User ID: %d\n",user->user.id);
      printf("Usertype: %d\n",user->user.usertype);
      printf("Username: %s\n",user->user.username);
      printf("Password: %s\n",user->user.password);
      printf("phonenum: %s\n",user->user.phonenum);
      printf("sex: %s\n",user->user.sex);
      printf("age: %d\n",user->user.age);
  }
}


ulink search_user_by_name(char username[])
{
  ulink user,temp;
  user=adminlist;
  //check if it is the head

  while(user!=NULL)
  {

    if(strcmp(user->user.username,username)==0)
    {
      user_display(user);
      return user;
    }
    user=user->next;
  }
  printf("user %s not exist in ADMIN list!\n",username);

  user=normallist;
  while(user!=NULL)
  {

    if(strcmp(user->user.username,username)==0)
    {
      user_display(user);
      return user;
    }
    user=user->next;
  }
  printf("user %s not exist in normal user list!\n",username);
  return NULL;
}

ulink search_user_by_id(int id)
{
  ulink user;
  user=adminlist;
  //check if it is the head
  while(user!=NULL)
  {

    if(user->user.id==id)
    {
      user_display(user);
      return user;
    }
  user=user->next;
  }
  printf("user id %d not exist in ADMIN list!\n",id);
  user=normallist;
  while(user!=NULL)
  {

    if(user->user.id==id)
    {
      user_display(user);
      return user;
    }
  user=user->next;
  }
  printf("user id %d not exist in normaluser list!\n",id);
  return NULL;
}

int user_delete_by_name(char username[])
{
  ulink user,temp;
  user=adminlist;
  //check if it is the head
  if(adminlist==NULL)
    printf("No user in the admin list\n");
  else if(strcmp(user->user.username,username)==0)
  {
   temp=user;
   if(user->next==NULL)
     adminlist=NULL;
  else
     adminlist=user->next;
  free(temp);
  printf("user delete successfully\n");
  return 0;
  }

  while(user->next!=NULL)
  {
    temp=user;
    user=user->next;
    if(strcmp(user->user.username,username)==0)
    {
      temp->next=user->next;
      free(user);
      printf("user delete successfully\n");
      return 0;
    }

  }
  printf("user not found in admin list\n");

  user=normallist;
  //check if it is the head
  if(normallist==NULL)
    printf("No user in the normal user list\n");
  else if(strcmp(user->user.username,username)==0)
  {
   if(user->next==NULL)
     normallist=NULL;
  else
     normallist=user->next;
  free(user);
  printf("user delete successfully\n");
  return 0;
  }

  while(user->next!=NULL)
  {

    temp=user;
    user=user->next;
    if(strcmp(user->user.username,username)==0)
    {
      temp->next=user->next;
      free(user);
      printf("user delete successfully\n");
      return 0;
    }

  }

  return -1;
}

int user_delete_by_id(int id)
{
  ulink user,temp;
  user=adminlist;
  //check if it is the head
  if(adminlist==NULL)
    printf("No user in the admin list\n");
  else if(user->user.id==id)
  {
   temp=user;
   if(user->next==NULL)
     adminlist=NULL;
  else
     adminlist=user->next;
  free(temp);
  printf("user delete successfully\n");
  return 0;
  }

  while(user->next!=NULL)
  {

    temp=user;
    user=user->next;
    if(user->user.id==id)
    {
      temp->next=user->next;
      free(user);
      printf("user delete successfully\n");
      return 0;
    }

  }
  printf("user not found in adminlist\n");

  user=normallist;
  //check if it is the head
  if(normallist==NULL)
    printf("No user in the normal list\n");
  else if(user->user.id==id)
  {
   if(user->next==NULL)
     normallist=NULL;
  else
     normallist=user->next;
  free(user);
  printf("user delete successfully\n");
  return 0;
  }

  while(user->next!=NULL)
  {

    temp=user;
    user=user->next;
    if(user->user.id==id)
    {
      temp->next=user->next;
      free(user);
      printf("user delete successfully\n");
      return 0;
    }

  }
  printf("user not found in normallist\n");

  return -1;
}

void book_insert(blink *booklistptr,blink booklink)
{
  blink temp;
  booklist=*booklistptr;
  if(booklist==NULL)
  { booklist=booklink;
    booklist->book.id=1;
    return;
  }
  for(temp=booklist;temp->next!=NULL;temp=temp->next)
     ;
  temp->next=booklink;
  booklink->book.id=temp->book.id+1;
}

void user_insert(ulink userlink,int flag)
{
   ulink temp,userlist;
   if(flag==ADMIN)
     userlist=adminlist;
   else
     userlist=normallist;
   if(userlist==NULL)
   {
     userlist=userlink;
     userlist->user.id=1;
     if(flag==ADMIN)
       adminlist=userlist;
     else
       normallist=userlist;
     return;
   }
   for(temp=userlist;temp->next!=NULL;temp=temp->next)
      ;
   temp->next=userlink;
   userlink->user.id=temp->user.id+1;

}

void normaluser_UI(char username[])
{
  printf("############# Menu for user '%s' #############\n",username);
  int c,flag=1;
  ulink user=NULL;
  borrowlink borrow=NULL;
  user=search_user_by_name(username);
  borrow=search_borrow_by_username(username);
  while(flag){
  printf("############# Menu for user '%s' #############\n",username);
  printf("1. User Info\n2. Modify User Info\n3. My Borrow\n4. Search Book\n5. Exit\n");
  printf("Which service do you like?[ 1 ]\n");
  c=getchar();
  getchar();
  switch(c){
    case '\n':
    case '1':
      user_display(user);
      break;
    case '2':
      modify_user_by_ulink(user);
      break;
    case '3':
      borrow_display(borrow);
      break;
    case '4':
      search_book();
      break;
    case '5':
      flag=0;
      break;
    default:
      printf("Please input an valid option\n");
      break;
  }
  }
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
  ulink adminptr,normalptr;
  int usertype=FORBIDDEN;

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

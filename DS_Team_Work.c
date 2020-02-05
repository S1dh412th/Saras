/*-------------------------------------------------------------
    C program to manage the information about a business

            Author :- DS-B3
            Date   :- 09/12/19

----------------------------------------------------------------*/


/* header files included*/
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

// macros used to create dynamic structure
#define unstr(ty,x) ty *content[x];

#define str(ty,x) typedef struct{\
                    unstr(ty,x)\
                    }general;

//Function prototype
void fnDelete (void);
void fnInsert(void);
int fnSearch(char [], char [], char [], char*);
char* fnGetData(char [], char [], char [], char*);
void fnModify(void);
void fRemove(char*,char*);

/*
    date: 9 Dec 2019
    Author:Snehal R. M.
*/
int main(void)
{
    char type[20],use[20],brand[20],*filename;
    int choice;

    for(;;)
    {
        //Menu interface
        system("cls");
        printf("\n");
        printf("\n");
        printf("\t\t\tSARAS MARKET");
        printf("\n\t______________________________________________________________");
        printf("\n\t|                                                            |");
        printf("\n\t|1.INSERT                                                    |");
        printf("\n\t|2.DELETE                                                    |");
        printf("\n\t|3.MODIFY                                                    |");
        printf("\n\t|4.SEARCH                                                    |");
        printf("\n\t|5.EXIT                                                      |");
        printf("\n\t|____________________________________________________________|");
        printf("\n\n\tEnter your choice : ");
        scanf("%d",&choice);
        fflush(stdin);
        printf("\n");

        switch(choice)
        {
            case 1 : fnInsert();
                     break;

            case 2 : fnDelete();
                     break;

            case 3 : fnModify();
                     break;

            case 4 : filename=fnGetData(type,use,brand,filename);
                     choice=fnSearch(type,use,brand,filename);
                     break;

            case 5 : exit(0);

            default : printf("\tInvalid choice");
       }

       printf("\n\tpress enter to continue.....");
       getch();
       fflush(stdin);
  }
  return 0;
} // end of  main function

/*
    date: 9 Dec 2019
    function return: filename that program uses to manipulate persistent information
    Author: Sidharth
*/
char* fnGetData(char type[20], char use[20], char brand[20], char *filename)
{
    filename=(char *)malloc(sizeof(char)*60);

    printf("\tEnter the type of product : ");
    scanf("%[^\n]s", type);
    fflush(stdin);
    printf("\tUsage for : ");
    scanf("%[^\n]s", use);
    fflush(stdin);
    printf("\tEnter the brand : ");
    scanf("%[^\n]s", brand);
    fflush(stdin);
    strcpy(filename,type);
    strcat(filename,"_");
    strcat(filename,use);
    strcat(filename,"_");
    strcat(filename,brand);
    strcat(filename,".txt");

    return filename;
}// end of fnGetData

/*
    function description : to Insert any new information
    date: 9 Dec 2019
    Author: Sidharth
*/
void fnInsert(void)
{
    char type[20], use[20], brand[20],ch,*filename=NULL;
    FILE *fp;
    int count=0,i,flag=0;  //count is to store no of parameters

    filename=fnGetData(type,use,brand,filename);

    if( (fp=fopen(filename,"r")) != NULL) //to check whether the file exists or not
    {
        fclose(fp);
        if( (fp=fopen(filename,"a+")) == NULL)
        {
            printf("\t%s failed to open.\n", filename);
            return;
        }
        rewind(fp);
        flag=1;
        fscanf(fp,"%d",&count);
    }
    else
    {
        if((fp=fopen(filename,"w")) == NULL)
        {
            printf("\t%s failed to open.\n", filename);
            return;
        }
        printf("\tEnter the number of parameters the %s has : ", type);
        scanf("%d",&count);
        fprintf(fp,"%d\n",count);
    }

    char cntnt[count][20];
    str(char, count);
    general temp;

    if(flag == 0) //if file doesn't exists
    {
        for(i=0 ; i<count ; ++i)
        {
            fflush(stdin);
            printf("\n\tEnter the %d parameter : ",i+1);
            scanf("%[^\n]",cntnt[i]);
            fflush(stdin);
            fputs(cntnt[i],fp);
            fputs(" ",fp);
        }
        fputs("\n",fp);
    }
    else
    {
        i=0;
        ch=fgetc(fp);
        char str1[count*22*sizeof(char)];
        while( (ch=fgetc(fp)) != '\n')    //to read parameter headers
        {
            str1[i]=ch;
            i++;
        }
        str1[i]='\0';


        i=0;
        char *token=strtok(str1," ");
        while (token!=NULL)
        {
            strcpy(cntnt[i],token);
            token = strtok(NULL," ");
            ++i;
        }
        fseek(fp,0,SEEK_END);
    }

    do
    {
        for(i=0 ; i<count ; ++i)   //to read the values of parameters
        {
            fflush(stdin);
            printf("\n\tEnter the %s of %s : ", cntnt[i], type);
            temp.content[i]=(char *)malloc(20*sizeof(char));
            scanf("%[^\n]s",temp.content[i]);
            fflush(stdin);
            fputs(temp.content[i],fp);
            fputc(' ',fp);
        }
        fputs("\n",fp);

        printf("\n\tDo you want to add more product of same type, usage and brand ?(y/n) : ");
        scanf("%c",&ch);
        fflush(stdin);
    }while(ch == 'y' || ch == 'Y');

    fclose(fp);
}//

/*
    file description : to delete a record and to delete the file if it is empty
    date: 9 Dec 2019
    Author: Ramya Ashrit
*/
void fnDelete(void)
{
    char type[20],use[20],brand[20],ch,*filename=NULL;
    FILE *fp,*fp2;

    filename=fnGetData(type,use,brand,filename);

    if( (fp = fopen(filename,"r+")) == NULL)
    {
        printf("\tFail to open");
        return;
    }

    fp2=fopen("temp.txt","w");

    while((ch=getc(fp)) != '\n')
    {
        putc(ch,fp2);
    }
    putc(ch,fp2);

    while((ch=getc(fp)) != '\n')
    {
        putc(ch,fp2);
    }

    printf("\n\t");
    while( (ch=getc(fp)) != '\n' )
    {
        printf("%c",ch);
    }
    printf("is deleted from the record.\n");

    putc(ch,fp2);

    if( ( ch = getc(fp) ) == EOF ) //to delete the current file if empty
    {
        fclose(fp);
        fclose(fp2);
        remove(filename);
        remove("temp.txt");

        return;
    }
    else
    {
        while(ch != EOF)
        {
            putc(ch,fp2);
            ch=getc(fp);
        }
    }

    fclose(fp);
    fclose(fp2);
    remove(filename);
    rename("temp.txt",filename);

    return;
}// end of fnInsert

/*
    date: 9 Dec 2019
    function return value: position of the product record from file (line number)
    Author: Ashwini
*/
int fnSearch(char type[20], char use[20], char brand[20], char *filename)
{
    FILE *fp;
    int wcount=0,i=0,temp=0,lNum=3;
    char ch;

    if( (fp=fopen(filename,"r")) ==NULL)
    {
        printf("\tfail to open");
        return -1;
    }

    fscanf(fp,"%d",&wcount);
    ch=fgetc(fp);

    char cntnt[wcount][20],str1[wcount*22*sizeof(char)],value[wcount][20];

    while( (ch=getc(fp)) != '\n')           //to read parameter headers
    {
        str1[i]=ch;
        i++;
    }
    str1[i]='\0';

    i=0;
    char *token=strtok(str1," ");
    while (token!=NULL)
    {
        strcpy(cntnt[i],token);
        token = strtok(NULL," ");
        ++i;
    }

    for(i=0 ;i<wcount ;++i)
    {
       printf("\tEnter the value of %s : ",cntnt[i]);
       scanf("%[^\n]s",value[i]);
       fflush(stdin);
    }

    while((ch=getc(fp)) != EOF)
    {
        i=0;
        while( ch != '\n')
        {
            str1[i]=ch;
            i++;
            ch=getc(fp);
        }
        str1[i]='\0';

        for(i=0; i<wcount; ++i)
        {
            if( strstr(str1,value[i]) )
            {
                ++temp;
            }
        }
        if(temp == wcount)
        {
            printf("\n\tProduct available.\n");
            fclose(fp);
            return lNum;
        }
        ++lNum;
        temp=0;
    }
    fclose(fp);

    printf("\n\tProduct unavailable.\n");

    return -1;
}// end of fnSearch

/*
    function description : to modify an existing record
    date: 9 Dec 2019
    Author: Adithi N.Prabhu
*/
void fnModify(void)
{
    int lineCount=3,i=0,pos,wcount;
    FILE *fp,*tempf;
    char type[20],use[20],brand[20],ch;
    char* filename=NULL;

    filename = fnGetData(type,use,brand,filename);
    pos = fnSearch(type,use,brand,filename);

    if(pos<=1)
    {
        return;
    }

    if( (fp=fopen(filename,"r")) ==NULL)
    {
        printf("\tfail to open");
        return ;
    }

    if((tempf=fopen("temp.txt","w")) == NULL)
    {
        printf("\tfail to open");
    }

    fscanf(fp,"%d",&wcount);
    fprintf(tempf,"%d",wcount);
    putc('\n',tempf);

    ch=getc(fp);

    char cntnt[wcount][20],str1[wcount*22*sizeof(char)],modValue[wcount][20];

    while( (ch=getc(fp)) != '\n')            //to read parameter headers
    {
        putc(ch,tempf);
        str1[i]=ch;
        i++;
    }
    putc(ch,tempf);
    str1[i]='\0';

    i=0;
    char *token=strtok(str1," ");
    while (token!=NULL)
    {
        strcpy(cntnt[i],token);
        token = strtok(NULL," ");
        ++i;
    }

    for(i=0 ;i<wcount ;++i)
    {
       printf("\tEnter the new value of %s : ",cntnt[i]);
       scanf("%[^\n]s",modValue[i]);
       fflush(stdin);
    }

    while(lineCount < pos)
    {
        if((ch=getc(fp)) == '\n')
        {
            ++lineCount;
        }
        putc(ch,tempf);
    }

    for(i=0 ; i<wcount ; ++i)
    {
        fputs(modValue[i],tempf);
        fputc(' ',tempf);
    }
    fputs("\n",tempf);

    while( (ch=getc(fp)) != '\n' );

    while((ch=getc(fp)) != EOF)
    {
        putc(ch,tempf);
    }

    fclose(fp);
    fclose(tempf);
    remove(filename);
    rename("temp.txt",filename);

    return;
}// end of fnModify

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <sys/stat.h>
#include <errno.h>

void CommandInput();
void Scan(char a[]);
void AddressInput(char a[]);
void Createfile(char add[]);

char root_address[] = "C:/Users/Asus/Desktop" ;

int main()
{
    while(1)
    {
        CommandInput();
    }
}

void Scan(char a[])
{
    char c;
    int i = 1;
    while((c = getchar()) == ' ')
        continue;
    a[0] = c;
    while((c = getchar()) != ' ')
    {
        a[i] = c;
        i++;
    }
    a[i+1] = '\0';
}

void AddressInput(char a[])
{
    for(int i = 0; i < strlen(a); i++)
    {
        a[i] = '\0';
    }
    char c;
    int i;
    while((c = getchar()) == ' ')
        continue;
    if(c == '/')
    {
        a[0] = '/';
        i = 1;
        while((c = getchar()) != ' ')
        {
            a[i] = c;
            i++;
        }
    }
    else if(c == '"')
    {
        i = 0;
        while(1)
        {
            if((c = getchar()) == '"' && a[i-1] != '\\')
                break;
            a[i] = c;
            i++;
        }
    }
}

void Createfile(char add[])
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    int k = strlen(tmp_address);
    int n = strlen(add);
    for(int i = 0; i < n; i++,k++)
    {
        if(add[i] == '\0')
        {
            FILE* file = fopen(tmp_address,"w");
            fclose(file);
        }
        if(i != 0 && add[i] == '/')
        {
            _mkdir(tmp_address);
        }
        tmp_address[k] = add[i];
    }
}

void CommandInput()
{
    char address[1000];
    char command[30];
    char Input_Type[10];
    Scan(command);
    if(!strcmp(command,"createfile"))
    {
        Scan(Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            Createfile(address);
        }
        else
        {
            printf("Wrong Input\n");
        }
    }
    else
    {
        printf("Wrong Command\n");
    }
}

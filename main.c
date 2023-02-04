#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

void CommandInput();
void AddressInput(char a[]);
void Createfile(char add[]);
void CheckAddress(char add[]);
void TextInput(char txt[]);
void InsertText(char add[], char txt[], int lin, int col);
void Cat(char add[]);

char root_address[] = "C:/Users/Asus/Desktop" ;
char val_address[] = "E:/FileVal";
int flag = 1, flag_file = 1;

int main()
{
    while(flag)
    {
        CommandInput();
    }
}

void AddressInput(char a[])
{
    char c;
    int i;
    while((c = getchar()) == ' ')
        continue;
    if(c == '/')
    {
        a[0] = '/';
        i = 1;
        while(1)
        {
            if((c = getchar()) == ' ')
                break;
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
    a[i] = '\0';
}

void CheckAddress(char add[])
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    int k = strlen(tmp_address);
    int n = strlen(add);
    for(int i = 0; i < n+1; i++,k++)
    {
        if(add[i] == '\0')
        {
            tmp_address[k] = add[i];
            FILE* file;
            if((file = fopen(tmp_address,"r")))
            {
                flag_file = 1;
                fclose(file);
            }
            else
            {
                fclose(file);
                flag_file = 0;
                printf("file doesn't exist\n");
            }
            break;
        }

        tmp_address[k] = add[i];
    }
}

void Createfile(char add[])
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    int k = strlen(tmp_address);
    int n = strlen(add);
    for(int i = 0; i < n+1; i++,k++)
    {
        if(add[i] == '\0')
        {
            tmp_address[k] = add[i];
            FILE* file;
            if((file = fopen(tmp_address,"r")))
            {
                printf("file already exists\n");
                fclose(file);
            }
            else
            {
                file = fopen(tmp_address,"w");
                fclose(file);
                printf("file creation successful");
            }
            break;
        }
        if(i != 0 && add[i] == '/')
        {
            _mkdir(tmp_address);
        }
        tmp_address[k] = add[i];
    }
}

void TextInput(char txt[])
{
    char c;
    int i;
    while((c = getchar()) == ' ')
        continue;
    if(c == '/')
    {
        txt[0] = '/';
        i = 1;
        while(1)
        {
            if((c = getchar()) == ' ')
                break;
            txt[i] = c;
            i++;
        }
    }
    else if(c == '"')
    {
        i = 0;
        while(1)
        {
            if((c = getchar()) == '"' && txt[i-1] != '\\')
                break;
            txt[i] = c;
            i++;
        }
    }
    txt[i] = '\0';
}

void InsertText(char add[], char txt[], int lin, int col)
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
        //char tmp_val[500]
        //strcpy(tmp_val,val_address);
        //strcat(tmp_val,"/clone");
    //char number in file
    FILE* file = fopen(tmp_address,"r");
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    //copying file in array
    char* str = malloc(sizeof(char)*length);
    int i = 0;
    int line = 1;
    int column = 0, pos = -1;
    while(i < length)
    {
        column++;
        *(str+i) = fgetc(file);
        if(*(str+i) == '\n')
        {
            line++;
            column = 0;
        }
        if(line == lin && column == col)
        {
            pos = ftell(file);
        }
        i++;
    }
    *(str+i-line+1) = '\0';
    fclose(file);
    if(pos == -1)
        printf("This position does not exist\n");
    else
    {
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        while(*(str+k)!='\0')
        {
            if(k == pos-1)
            {
                for(int j = 0; j < strlen(txt); j++)
                {
                    if(txt[j] == '\\' && txt[j+1] == '"')
                        continue;
                    if(txt[j] == '\\' && txt[j+1] == '\\' && txt[j+2] == 'n')
                    {
                        fprintf(file, "\\");
                        fprintf(file, "n");
                        j += 2;
                        continue;
                    }
                    if(txt[j] == '\\' && txt[j+1] == 'n')
                    {
                        fprintf(file, "\n");
                        j++;
                        continue;
                    }
                    fprintf(file, "%c", txt[j]);
                }
            }
            fprintf(file, "%c", *(str+k));
            k++;
        }
        printf("insertion successful\n");
        fclose(file);
    }
    free(str);
}

void Cat(char add[])
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
    //char number in file
    FILE* file = fopen(tmp_address,"r");
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    for(int i = 0; i < length; i++)
    {
        char c = fgetc(file);
        printf("%c", c);
    }
    printf("\n");
}

void CommandInput()
{
    char address[1000];
    char command[30];
    char Input_Type[10];
    scanf("%s", command);
    //getchar();
    if(!strcmp(command,"createfile"))
    {
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            Createfile(address);
            char c;
            while((c = getchar()) != '\n')
                continue;
        }
    }
    else if(!strcmp(command,"insertstr"))
    {
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            CheckAddress(address);
            scanf("%s", Input_Type);
            if(!(strcmp(Input_Type,"--str")) && flag_file == 1)
            {
                char* text = malloc(sizeof(char) * 1000000);
                TextInput(text);
                scanf("%s", Input_Type);
                if(!(strcmp(Input_Type,"--pos")))
                {
                    int lin,col;
                    char c;
                    scanf("%d", &lin);
                    c = getchar();
                    scanf("%d", &col);
                    InsertText(address, text, lin, col);
                    while((c = getchar()) != '\n')
                        continue;
                }
                free(text);
            }
        }
    }
    else if(!strcmp(command,"cat"))
    {
        AddressInput(address);
        CheckAddress(address);
        if(flag_file == 1)
        {
            Cat(address);
            char c;
            while((c = getchar()) != '\n')
                    continue;
        }
    }
    else if(!strcmp(command,"exit"))
    {
        flag = 0;
    }
    else
    {
        printf("Wrong Command\n");
    }
}

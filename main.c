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
void Remove(char add[], int lin, int col, int size, char mov);
void Copy(char add[], int lin, int col, int size, char mov);
void Cut(char add[], int lin, int col, int size, char mov);
void Paste(char add[], int lin, int col);
void Undo(char add[]);
void Compare(char add[], char add2[]);

char root_address[] = "C:/Users/Asus/Desktop" ;
char val_address[] = "E:/FileVal/rootclone";
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
    //normal file
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
                printf("file creation successful\n");
            }
            break;
        }
        if(i != 0 && add[i] == '/')
        {
            _mkdir(tmp_address);
        }
        tmp_address[k] = add[i];
    }
    //undo file1
    char undo_val[500];
    strcpy(undo_val,val_address);
    k = strlen(undo_val);
    undo_val[k] = '\0';
    n = strlen(add);
    for(int i = 0; i < n+1; i++,k++)
    {
        if(add[i] == '\0')
        {
            strcat(undo_val,"-undo.txt");
            strcat(undo_val,"\0");
            FILE* file;
            if((file = fopen(undo_val,"r")))
            {
                fclose(file);
            }
            else
            {
                file = fopen(undo_val,"w");
                fclose(file);
                strcat(undo_val,"V2.txt");
                strcat(undo_val,"\0");
                file = fopen(undo_val,"w");
                fclose(file);
            }
            break;
        }
        if(i != 0 && add[i] == '/')
        {
             undo_val[k] = '\0';
            _mkdir(undo_val);
        }
        undo_val[k] = add[i];
    }
}

void TextInput(char txt[])
{
    char c;
    int i;
    while((c = getchar()) == ' ')
        continue;
    if(c != '"')
    {
        txt[0] = c;
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
    //undo file
    char undo_val[500];
    strcpy(undo_val,val_address);
    strcat(undo_val,add);
    strcat(undo_val,"-undo.txt\0");
    //clone file to put in
    char tmp_val[500];
    strcpy(tmp_val,val_address);
    strcat(tmp_val,"/clone");
    //copying file in array
    FILE* file = fopen(tmp_address,"r");
    FILE* clone = fopen(tmp_val,"w");
    int i = 0;
    int line = 1;
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int column = 0, pos = -1;
    while(i < length)
    {
        char c = fgetc(file);
        fputc(c, clone);
        if(c == '\n')
        {
            line++;
            column = 0;
        }
        if(line == lin && column == col)
        {
            pos = ftell(file);
        }
        column++;
        i++;
    }
    fclose(file);
    fclose(clone);
    //copying file in undo
    file = fopen(tmp_address,"r");
    FILE* undo = fopen(undo_val,"w");
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int j = 0;
    line = 1;
    while(j < length - line)
    {
        char c = fgetc(file);
        fputc(c, undo);
        j++;
    }
    fclose(file);
    fclose(undo);
    //insertion
    if(pos == -1)
        printf("This position does not exist\n");
    else
    {
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
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
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("insertion successful\n");
        fclose(file);
        fclose(clone);
    }
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
    fclose(file);
    printf("\n");
}

void Remove(char add[], int lin, int col, int size, char mov)
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
    //undo file
    char undo_val[500];
    strcpy(undo_val,val_address);
    strcat(undo_val,add);
    strcat(undo_val,"-undo.txt\0");
    //file to put in
    char tmp_val[500];
    strcpy(tmp_val,val_address);
    strcat(tmp_val,"/clone");
    //copying file in array
    FILE* file = fopen(tmp_address,"r");
    FILE* clone = fopen(tmp_val,"w");
    int i = 0;
    int line = 1;
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int column = 0, pos = -1;
    while(i < length)
    {
        char c = fgetc(file);
        fputc(c, clone);
        if(c == '\n')
        {
            line++;
            column = 0;
        }
        if(line == lin && column == col)
        {
            pos = ftell(file);
        }
        column++;
        i++;
    }
    fclose(file);
    fclose(clone);
    //copying file in undo
    file = fopen(tmp_address,"r");
    FILE* undo = fopen(undo_val,"w");
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int j = 0;
    line = 1;
    while(j < length - line)
    {
        char c = fgetc(file);
        fputc(c, undo);
        j++;
    }
    fclose(file);
    fclose(undo);
    //removal
    if(pos == -1)
        printf("This position does not exist\n");
    else if (pos != -1 && mov == 'b')
    {
        if(pos <= size)
            size = pos - 1;
        if(pos+size-1 >= length)
            size = length - pos + 1;
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
        {
            if(k == pos-size-1)
            {
                k += size;
                while(size > 0)
                {
                    char c = fgetc(clone);
                    size--;
                }
            }
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("backward removal successful\n");
        fclose(file);
    }
    else if (pos != -1 && mov == 'f')
    {
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
        {
            if(k == pos-1)
            {
                k += size;
                while(size > 0)
                {
                    char c = fgetc(clone);
                    size--;
                }
            }
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("forward removal successful\n");
        fclose(file);
        fclose(clone);
    }
}

void Copy(char add[], int lin, int col, int size, char mov)
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
    //file to put in
    char tmp_val[500];
    strcpy(tmp_val,val_address);
    strcat(tmp_val,"/clone");
    //clipboard address
    char clipboard[500];
    strcpy(clipboard,val_address);
    strcat(clipboard,"/clipboard.txt");
    //char number in file
    FILE* file = fopen(tmp_address,"r");
    FILE* clone = fopen(tmp_val,"w");
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    //copying file in array
    int i = 0;
    int line = 1;
    int column = 0, pos = -1;
    while(i < length)
    {
        char c = fgetc(file);
        fputc(c, clone);
        if(c == '\n')
        {
            line++;
            column = 0;
        }
        if(line == lin && column == col)
        {
            pos = ftell(file);
        }
        column++;
        i++;
    }
    fclose(file);
    fclose(clone);
    if(pos == -1)
        printf("This position does not exist\n");
    else if (pos != -1 && mov == 'b')
    {
        if(pos <= size)
            size = pos - 1;
        if(pos+size-1 >= length)
            size = length - pos + 1;
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
        {
            if(k == pos-size-1)
            {
                k += size;
                FILE* board = fopen(clipboard,"w");
                while(size > 0)
                {
                    char c = fgetc(clone);
                    fprintf(board, "%c", c);
                    fprintf(file, "%c", c);
                    size--;
                }
                fclose(board);
            }
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("backward copy successful\n");
        fclose(file);
    }
    else if (pos != -1 && mov == 'f')
    {
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
        {
            if(k == pos-1)
            {
                k += size;
                FILE* board = fopen(clipboard,"w");
                while(size > 0)
                {
                    char c = fgetc(clone);
                    fprintf(file, "%c", c);
                    fprintf(board, "%c", c);
                    size--;
                }
                fclose(board);
            }
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("forward copy successful\n");
        fclose(file);
        fclose(clone);
    }
}

void Cut(char add[], int lin, int col, int size, char mov)
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
    //undo file
    char undo_val[500];
    strcpy(undo_val,val_address);
    strcat(undo_val,add);
    strcat(undo_val,"-undo.txt\0");
    //file to put in
    char tmp_val[500];
    strcpy(tmp_val,val_address);
    strcat(tmp_val,"/clone");
    //clipboard address
    char clipboard[500];
    strcpy(clipboard,val_address);
    strcat(clipboard,"/clipboard.txt");
    //copying file in array
    FILE* file = fopen(tmp_address,"r");
    FILE* clone = fopen(tmp_val,"w");
    int i = 0;
    int line = 1;
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int column = 0, pos = -1;
    while(i < length)
    {
        char c = fgetc(file);
        fputc(c, clone);
        if(c == '\n')
        {
            line++;
            column = 0;
        }
        if(line == lin && column == col)
        {
            pos = ftell(file);
        }
        column++;
        i++;
    }
    fclose(file);
    fclose(clone);
    //copying file in undo
    file = fopen(tmp_address,"r");
    FILE* undo = fopen(undo_val,"w");
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int j = 0;
    line = 1;
    while(j < length - line)
    {
        char c = fgetc(file);
        fputc(c, undo);
        j++;
    }
    fclose(file);
    fclose(undo);
    //cut
    if(pos == -1)
        printf("This position does not exist\n");
    else if (pos != -1 && mov == 'b')
    {
        if(pos <= size)
            size = pos - 1;
        if(pos+size-1 >= length)
            size = length - pos + 1;
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
        {
            if(k == pos-size-1)
            {
                k += size;
                FILE* board = fopen(clipboard,"w");
                while(size > 0)
                {
                    char c = fgetc(clone);
                    fprintf(board, "%c", c);
                    size--;
                }
                fclose(board);
            }
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("backward copy successful\n");
        fclose(file);
    }
    else if (pos != -1 && mov == 'f')
    {
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
        {
            if(k == pos-1)
            {
                k += size;
                FILE* board = fopen(clipboard,"w");
                while(size > 0)
                {
                    char c = fgetc(clone);
                    fprintf(board, "%c", c);
                    size--;
                }
                fclose(board);
            }
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("forward cut successful\n");
        fclose(file);
        fclose(clone);
    }
}

void Paste(char add[], int lin, int col)
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
    //undo file
    char undo_val[500];
    strcpy(undo_val,val_address);
    strcat(undo_val,add);
    strcat(undo_val,"-undo.txt\0");
    //file to put in
    char tmp_val[500];
    strcpy(tmp_val,val_address);
    strcat(tmp_val,"/clone");
    //clipboard address
    char clipboard[500];
    strcpy(clipboard,val_address);
    strcat(clipboard,"/clipboard.txt");
    //copying file in array
    FILE* file = fopen(tmp_address,"r");
    FILE* clone = fopen(tmp_val,"w");
    int i = 0;
    int line = 1;
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int column = 0, pos = -1;
    while(i < length)
    {
        char c = fgetc(file);
        fputc(c, clone);
        if(c == '\n')
        {
            line++;
            column = 0;
        }
        if(line == lin && column == col)
        {
            pos = ftell(file);
        }
        column++;
        i++;
    }
    fclose(file);
    fclose(clone);
    //copying file in undo
    file = fopen(tmp_address,"r");
    FILE* undo = fopen(undo_val,"w");
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int j = 0;
    line = 1;
    while(j < length - line)
    {
        char c = fgetc(file);
        fputc(c, undo);
        j++;
    }
    fclose(file);
    fclose(undo);
    //paste
    if(pos == -1)
        printf("This position does not exist\n");
    else
    {
        int k = 0;
        FILE* file = fopen(tmp_address,"w");
        FILE* clone = fopen(tmp_val,"r");
        while(k < length-line+1)
        {
            if(k == pos-1)
            {
                FILE* board = fopen(clipboard,"r");
                fseek(board, 0, SEEK_END);
                int size = ftell(board);
                fseek(board, 0, SEEK_SET);
                while(size > 0)
                {
                    char c = fgetc(board);
                    fprintf(file, "%c", c);
                    size--;
                }
                fclose(board);
            }
            char c = fgetc(clone);
            fprintf(file, "%c", c);
            k++;
        }
        printf("paste successful\n");
        fclose(file);
        fclose(clone);
    }
}

void Undo(char add[])
{
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
    //undo file
    char undo_val[500];
    strcpy(undo_val,val_address);
    strcat(undo_val,add);
    strcat(undo_val,"-undo.txt\0");
    //undo2 file
    char undo_val2[500];
    strcpy(undo_val2,undo_val);
    strcat(undo_val2,"V2.txt\0");
    //line num file
    FILE* file = fopen(tmp_address,"r");
    FILE* undo2 = fopen(undo_val2,"w");
    int i = 0;
    int line = 1;
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    while(i < length)
    {
        char c = fgetc(file);
        if(c == '\n')
        {
            line++;
        }
        i++;
    }
    //copying file in undo2
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    int j = 0;
    while(j < length - line + 1)
    {
        char c = fgetc(file);
        fputc(c, undo2);
        j++;
    }
    fclose(file);
    fclose(undo2);
    //line num undo
    FILE* undo = fopen(undo_val,"r");
    file = fopen(tmp_address,"w");
    i = 0;
    line = 1;
    fseek(undo, 0, SEEK_END);
    length = ftell(undo);
    fseek(undo, 0, SEEK_SET);
    while(i < length)
    {
        char c = fgetc(undo);
        if(c == '\n')
        {
            line++;
        }
        i++;
    }
    //copying undo in file
    fseek(undo, 0, SEEK_END);
    length = ftell(undo);
    fseek(undo, 0, SEEK_SET);
    j = 0;
    while(j < length - line + 1)
    {
        char c = fgetc(undo);
        fputc(c, file);
        j++;
    }
    fclose(file);
    fclose(undo);
    //line num undo2
    undo2 = fopen(undo_val2,"r");
    undo = fopen(undo_val,"w");
    i = 0;
    line = 1;
    fseek(undo2, 0, SEEK_END);
    length = ftell(undo2);
    fseek(undo2, 0, SEEK_SET);
    while(i < length)
    {
        char c = fgetc(undo2);
        if(c == '\n')
        {
            line++;
        }
        i++;
    }
    //copying undo2 in undo
    fseek(undo2, 0, SEEK_END);
    length = ftell(undo2);
    fseek(undo2, 0, SEEK_SET);
    j = 0;
    while(j < length - line + 1)
    {
        char c = fgetc(undo2);
        fputc(c, undo);
        j++;
    }
    fclose(undo);
    fclose(undo2);
    printf("Undo successful\n");
}

void Compare(char add[], char add2[])
{
    //add1
    char tmp_address[1050];
    strcpy(tmp_address,root_address);
    strcat(tmp_address,add);
    //add2
    char tmp_address2[1050];
    strcpy(tmp_address2,root_address);
    strcat(tmp_address2,add2);
    //clone1 file to put in
    char tmp_val[500];
    strcpy(tmp_val,val_address);
    strcat(tmp_val,"/clone");
    //clone2 file to put in
    char tmp_val2[500];
    strcpy(tmp_val2,val_address);
    strcat(tmp_val2,"/clone2");
    //copying file in clone1
    FILE* file = fopen(tmp_address,"r");
    FILE* clone = fopen(tmp_val,"w");
    int i = 0;
    int line = 1;
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    while(i < length)
    {
        char c = fgetc(file);
        fputc(c, clone);
        if(c == '\n')
        {
            line++;
        }
        i++;
    }
    fclose(file);
    fclose(clone);
    //copying file2 in clone2
    FILE* file2 = fopen(tmp_address2,"r");
    FILE* clone2 = fopen(tmp_val2,"w");
    int i2 = 0;
    int line2 = 1;
    fseek(file2, 0, SEEK_END);
    int length2 = ftell(file2);
    fseek(file2, 0, SEEK_SET);
    while(i2 < length2)
    {
        char c = fgetc(file2);
        fputc(c, clone2);
        if(c == '\n')
        {
            line2++;
        }
        i2++;
    }
    fclose(file2);
    fclose(clone2);
    clone = fopen(tmp_val,"r");
    clone2 = fopen(tmp_val2,"r");
    int l1 = 1,l2 = 1;
    int k1 = 0,k2 = 0;
    if(line2 < line)
    {
        while(l1 <= line2)
        {
            char* lane1 = calloc(1500,sizeof(char));
            char* lane2 = calloc(1500,sizeof(char));
            int j1 = 0,j2 = 0;
            while(k1 < length - line + 1)
            {
                lane1[j1] = fgetc(clone);
                if(lane1[j1] == '\n')
                    break;
                j1++;
                k1++;
            }
            while(k2 < length2 - line2 + 1)
            {
                lane2[j2] = fgetc(clone2);
                if(lane2[j2] == '\n')
                    break;
                j2++;
                k2++;
            }
            if((strcmp(lane1,lane2)))
            {
                printf("================#%d===============\n", l1);
                printf("%s", lane1);
                printf("%s", lane2);
                if(l1 == line2)
                    printf("\n");
            }
            l1++;
            l2++;
            free(lane1);
            free(lane2);
        }
        printf("================#%d-#%d===============\n", l1, line);
        while(l1 > line2 && l1 <= line)
        {
            char* lane1 = calloc(1500,sizeof(char));
            int j1 = 0;
            while(k1 < length - line + 1)
            {
                lane1[j1] = fgetc(clone);
                if(lane1[j1] == '\n')
                    break;
                j1++;
                k1++;
            }
            printf("%s", lane1);
            l1++;
            free(lane1);
        }
    }
    else if(line2 > line)
    {
        while(l2 <= line)
        {
            char* lane1 = calloc(1500,sizeof(char));
            char* lane2 = calloc(1500,sizeof(char));
            int j1 = 0,j2 = 0;
            while(k1 < length - line + 1)
            {
                lane1[j1] = fgetc(clone);
                if(lane1[j1] == '\n')
                    break;
                j1++;
                k1++;
            }
            while(k2 < length2 - line2 + 1)
            {
                lane2[j2] = fgetc(clone2);
                if(lane2[j2] == '\n')
                    break;
                j2++;
                k2++;
            }
            if((strcmp(lane1,lane2)))
            {
                printf("================#%d===============\n", l2);
                printf("%s", lane1);
                if(line == l2)
                    printf("\n");
                printf("%s", lane2);
            }
            l1++;
            l2++;
            free(lane1);
            free(lane2);
        }
        printf("================#%d-#%d===============\n", l2, line2);
        while(l2 > line && l2 <= line2)
        {
            char* lane2 = calloc(1500,sizeof(char));
            int j2 = 0;
            while(k2 < length2 - line2 + 1)
            {
                lane2[j2] = fgetc(clone2);
                if(lane2[j2] == '\n')
                    break;
                j2++;
                k2++;
            }
            printf("%s", lane2);
            l2++;
            free(lane2);
        }
    }
    else if(line2 == line)
    {
        while(l2 <= line)
        {
            char* lane1 = calloc(1500,sizeof(char));
            char* lane2 = calloc(1500,sizeof(char));
            int j1 = 0,j2 = 0;
            while(k1 < length - line + 1)
            {
                lane1[j1] = fgetc(clone);
                if(lane1[j1] == '\n')
                    break;
                j1++;
                k1++;
            }
            while(k2 < length2 - line2 + 1)
            {
                lane2[j2] = fgetc(clone2);
                if(lane2[j2] == '\n')
                    break;
                j2++;
                k2++;
            }
            if((strcmp(lane1,lane2)))
            {
                printf("================#%d===============\n", l2);
                printf("%s", lane1);
                if(l2 == line)
                    printf("\n");
                printf("%s", lane2);
            }
            l1++;
            l2++;
            free(lane1);
            free(lane2);
        }
    }
    fclose(clone);
    fclose(clone2);
    printf("\n--------------------------------\nComparation successful\n");
}

void CommandInput()
{
    char address[1000];
    char address2[1000];
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
    else if(!strcmp(command,"insert"))
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
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
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
    }
    else if(!strcmp(command,"remove"))
    {
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            CheckAddress(address);
            scanf("%s", Input_Type);
            if(!(strcmp(Input_Type,"--pos")) && flag_file == 1)
            {
                int lin,col;
                char c;
                scanf("%d", &lin);
                c = getchar();
                scanf("%d", &col);
                while((c = getchar()) == ' ')
                    continue;
                scanf("%s", Input_Type);
                if(!(strcmp(Input_Type,"size")))
                {
                    int size;
                    scanf("%d", &size);
                    while((c = getchar()) == ' ')
                        continue;
                    char mov = getchar();
                    Remove(address, lin, col, size, mov);
                    while((c = getchar()) != '\n')
                        continue;
                }
            }
        }
    }
    else if(!strcmp(command,"copy"))
    {
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            CheckAddress(address);
            scanf("%s", Input_Type);
            if(!(strcmp(Input_Type,"--pos")) && flag_file == 1)
            {
                int lin,col;
                char c;
                scanf("%d", &lin);
                c = getchar();
                scanf("%d", &col);
                while((c = getchar()) == ' ')
                    continue;
                scanf("%s", Input_Type);
                if(!(strcmp(Input_Type,"size")))
                {
                    int size;
                    scanf("%d", &size);
                    while((c = getchar()) == ' ')
                        continue;
                    char mov = getchar();
                    Copy(address, lin, col, size, mov);
                    while((c = getchar()) != '\n')
                        continue;
                }
            }
        }
    }
    else if(!strcmp(command,"cut"))
    {
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            CheckAddress(address);
            scanf("%s", Input_Type);
            if(!(strcmp(Input_Type,"--pos")) && flag_file == 1)
            {
                int lin,col;
                char c;
                scanf("%d", &lin);
                c = getchar();
                scanf("%d", &col);
                while((c = getchar()) == ' ')
                    continue;
                scanf("%s", Input_Type);
                if(!(strcmp(Input_Type,"size")))
                {
                    int size;
                    scanf("%d", &size);
                    while((c = getchar()) == ' ')
                        continue;
                    char mov = getchar();
                    Cut(address, lin, col, size, mov);
                    while((c = getchar()) != '\n')
                        continue;
                }
            }
        }
    }
    else if(!strcmp(command,"paste"))
    {
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            CheckAddress(address);
            scanf("%s", Input_Type);
            if(!(strcmp(Input_Type,"--pos")) && flag_file == 1)
            {
                int lin,col;
                char c;
                scanf("%d", &lin);
                c = getchar();
                scanf("%d", &col);
                Paste(address, lin, col);
            }
        }
    }
    else if(!strcmp(command,"undo"))
    {
        scanf("%s", Input_Type);
        if(!(strcmp(Input_Type,"--file")))
        {
            AddressInput(address);
            CheckAddress(address);
            if(flag_file == 1)
            {
                Undo(address);
                char c;
                while((c = getchar()) != '\n')
                    continue;
            }
        }
    }
    else if(!strcmp(command,"compare"))
    {
        AddressInput(address);
        CheckAddress(address);
        if(flag_file == 1)
        {
            AddressInput(address2);
            CheckAddress(address2);
            if(flag_file == 1)
            {
                Compare(address, address2);
                char c;
                while((c = getchar()) != '\n')
                    continue;
            }
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

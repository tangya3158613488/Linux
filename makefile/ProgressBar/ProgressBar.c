#include<stdio.h>
#include<string.h>

int main()
{
    int i=0;
    char bar[102];
    memset(bar,'\0',sizeof(bar)*sizeof(char));
    const char*lable="|/-\\";//这里的两个\\表示一个\
    while(i<=100)
    {
        printf("[%-100s][%d%%][%c]\r",bar,i,lable[i%4]);//这里要注意一些转义字符，%%输出的就是%.
        //其中[%-100s]表示以字符串输出，有100个字符位，从左往右输出，如果不加-，即从右往左输出。
        ////后面加一个\r回车，即将光标移到行首，接下输出的字符将覆盖行首的字符
        fflush(stdout);
        bar[i++]='#';
        usleep(100000);
    }
    printf("\n");
    return 0;
}

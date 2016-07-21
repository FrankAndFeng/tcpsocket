#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int max_buf_size = 256;

    char str[max_buf_size];
    char str_send[max_buf_size];
    int order;
    while (1)
    {
        if (fgets(str, max_buf_size, stdin))
        {
            sscanf(str, "%d:%s", &order, str_send);

           // printf("%s\n",str);
            //num = atoi(str);
            printf("%d\n", order);
            printf("%c\n", (char)itoa(order));
            printf("%s\n", str_send);
            printf("%s\n",str);
            memset(str, 0, strlen(str));
            order = 0;
            memset(str_send, 0, strlen(str_send));
        }
    }
    return 0;

}

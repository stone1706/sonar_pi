#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int kbhit(void)
{
    struct termios oldt, newt;
    int oldf;
    char c;
    int n;

    /* 保存当前终端设置 */
    tcgetattr(STDIN_FILENO, &oldt);
    /* 修改终端设置 */
    newt = oldt;
    /* ICANON：使输入成为非规范模式（即不需要按下回车键） */
    /* ECHO：防止输入字符被显示在终端上。 然后用 tcsetattr 应用这个新的设置 */
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    /* 设置非阻塞输入,这样调用 read 时，如果没有输入，会立即返回而不是等待 */
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    /* 读取输入 */
    n = read(STDIN_FILENO, &c, 1);

    /* 恢复终端设置 */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return n > 0 ? c : 0;
}

int main()
{
    printf("按下字母键，按下Esc键退出。\n");

    while (1)
    {
        char c = kbhit();
        if (c)
        {
            if (c == 27)
            { // Esc键的ASCII码
                break;
            }
            printf("按下的键: %d\n", c);
        }
        usleep(100000); // 防止高CPU使用率
    }

    return 0;
}

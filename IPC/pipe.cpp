#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include <iostream>

using namespace std ;

int main(int argc, char *argv[])
{
    int fds[2] ; // 文件描述符数组
    int fds2[2] ;
    if (-1 == pipe(fds))
    {
        cerr << "pipe create failed." << endl ;
        exit(EXIT_FAILURE) ;
    }

    if (-1 == pipe(fds2))
    {
        cerr << "pipe create failed." << endl ;
        exit(EXIT_FAILURE) ;
    }

    pid_t pid ;
    pid = fork() ; // 创建子进程
    if (-1 == pid)
    {
        cerr << "fork process failed." << endl ;
        exit(EXIT_FAILURE) ;
    }
    // 父进程拿到子进程pid，子进程拿到0
    cout << (pid? "father": "child") << "process get fork() return pid = " << pid << endl ;
    if (0 == pid) // 子进程
    {
        close(fds[0]) ; // 子进程关闭管道读段
        close(fds2[1]) ;
        string strHello = "child -> father: Hello Pipe." ;
        write(fds[1], strHello.c_str(), strHello.length()) ;
        char buf[1000] ;
        read(fds2[0], buf, 1000) ;
        cout << "father -> child: " << buf << endl ;
        close(fds[1]) ;
        close(fds2[0]) ;
        exit(EXIT_SUCCESS) ;
    }

    close(fds[1]) ; // 父进程关闭写端
    close(fds2[0]) ;
    char buf[1000] ;
    read(fds[0], buf, 1000) ;
    cout << buf << endl ;
    string str = "Hoho~" ;
    write(fds2[1], str.c_str(), str.length()) ;

    //要关闭管道只需将这两个文件描述符关闭即可。
    close(fds[0]) ;
    close(fds2[1]) ;

    exit(EXIT_SUCCESS) ;
}

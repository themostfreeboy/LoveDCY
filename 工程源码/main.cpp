#include "logindlg.h"
#include <QApplication>
#include <QDir>
#include "maindlg.h"
//#include <QMessageBox>
//#include <QDebug>

#if defined Q_OS_WIN32//for win
#include <windows.h>
bool checkOnly()
{
    //创建互斥量
    HANDLE m_hMutex = CreateMutex(NULL, FALSE,  L"rocker_love");
    //检查错误代码
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        //如果已有互斥量存在则释放句柄并复位互斥量
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
        //程序退出
        return false;
    }
    else
    {
       return true;
    }
}
#endif
#if defined  Q_OS_LINUX//for linux
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
bool checkOnly()
{
    const char filename[]  = "/tmp/lovelockfile";
    int fd = open (filename, O_WRONLY | O_CREAT , 0644);
    int flock = lockf(fd, F_TLOCK, 0 );
    if (fd == -1) {
            perror("open lockfile/n");
            return false;
    }
    //给文件加锁
    if (flock == -1) {
            perror("lock file error/n");
            return false;
    }
    //程序退出后，文件自动解锁
    return true;
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    if (!checkOnly())
    {
        return 0;
    }
    LoginDlg wLogin;
    if(QDialog::Accepted == wLogin.exec())
    {
        MainDlg wMainDlg;
        wMainDlg.show();
        return a.exec();
    }
    return 0;
}

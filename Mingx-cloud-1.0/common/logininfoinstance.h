#ifndef LOGININFOINSTANCE_H
#define LOGININFOINSTANCE_H

#include "common/common.h"

//单例模式类
class LoginInfoInstance
{
public:
    static LoginInfoInstance* getInstance();    //创建单例模式对象的静态接口
    static void destroy();  //释放堆区空间
    QString getUser();//获取登录用户
    QString getIp();    //获取服务器ip
    QString getPort();  //获取服务器port
    QString getToken(); //获取登录token
    void setLoginInfo( QString tmpUser, QString tmpIp, QString tmpPort,  QString token="");//设置登陆信息
private:
    //把所有构造函数设置为私有
    LoginInfoInstance();
    LoginInfoInstance(const LoginInfoInstance&);
    LoginInfoInstance& operator=(const LoginInfoInstance&);

    ~LoginInfoInstance();

private:
    static LoginInfoInstance* instance;
    QString user;   //当前登录用户
    QString token;  //用户登录token
    QString ip;     //web服务器ip
    QString port;   //port
};

#endif // LOGININFOINSTANCE_H

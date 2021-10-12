#include "logininfoinstance.h"

//类外初始化静态成员变量
LoginInfoInstance* LoginInfoInstance::instance=LoginInfoInstance::getInstance();

//获取服务器port获得唯一实例
LoginInfoInstance *LoginInfoInstance::getInstance()
{
    return instance;
}
//释放堆区空间
void LoginInfoInstance::destroy()
{
    if(LoginInfoInstance::instance){
        delete  LoginInfoInstance::instance;
        LoginInfoInstance::instance=nullptr;
    }
}

QString LoginInfoInstance::getUser()
{
    return this->user;
}

QString LoginInfoInstance::getIp()
{
    return this->ip;
}

QString LoginInfoInstance::getPort()
{
    return port;
}

QString LoginInfoInstance::getToken()
{
    return token;
}
//设置登录信息
void LoginInfoInstance::setLoginInfo(QString tmpUser, QString tmpIp, QString tmpPort, QString token)
{
    this->user=tmpUser;
    this->token=token;
    this->ip=tmpIp;
    this->port=tmpPort;
}

LoginInfoInstance::LoginInfoInstance()
{
}
LoginInfoInstance::LoginInfoInstance(const LoginInfoInstance &)
{
}
LoginInfoInstance &LoginInfoInstance::operator=(const LoginInfoInstance &)
{
    return *this;
}

LoginInfoInstance::~LoginInfoInstance()
{

}

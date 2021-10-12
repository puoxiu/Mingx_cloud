#include "login.h"
#include "ui_login.h"

#include <qpainter.h>
#include <QRegExp>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include "common/logininfoinstance.h"   //单例模式类
#include "common/des.h" //数据加密

#define SERVER_IP "127.0.0.1"
#define USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36 UOS"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    m_mainWin=new MainWindow();

    //去掉窗口边框
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //加载程序Logo
    this->setWindowIcon(QPixmap(":/images/Logo1.png"));


    //密码保护显示（输入密码显示黑点）
    ui->userPasswd->setEchoMode(QLineEdit::Password);
    ui->regPasswd->setEchoMode(QLineEdit::Password);
    ui->regConfirmPwassd->setEchoMode(QLineEdit::Password);

    //数据格式提示

    //读取配置文件
    this->readCfg();
    //加载表示图片类型的图片-显示文件列表的时候用，在此初始化
    m_com.getFileTypeList();

    //当前显示的窗口
    ui->stackedWidget->setCurrentWidget(ui->login_page);
    ui->login_page->setFocus();

    //tittlewg信号处理 x
    connect(ui->tittle_wg,&TittleWg::closeWindow,[=](){
        //判断当前stackWidget显示的页面
        //如果是设置页面或者注册页面，则返回主页面,否则关闭窗口 即退出登录
        if(ui->stackedWidget->currentWidget()==ui->set_page ||
                ui->stackedWidget->currentWidget()==ui->register_page){
            ui->stackedWidget->setCurrentWidget(ui->login_page);
        }else{
            this->close();
        }
    });
    connect(ui->tittle_wg,&TittleWg::showSetWg,[=](){
       ui->stackedWidget->setCurrentWidget(ui->set_page);
       //ui->stackedWidget->setCurrentIndex(2);
    });
    //接收从主窗口发来的切换用户信号
    connect(m_mainWin,&MainWindow::sigChangeUser,[=](){
        m_mainWin->hide();
        this->show();
    });
}

Login::~Login()
{
    delete ui;
}
//给窗口画背景图--OK
void Login::paintEvent(QPaintEvent *e)
{    
    //定义画家类
    QPainter p(this);
    QPixmap pixmap(":/images/beijing3.png");
    p.drawPixmap(0,0,this->width(),this->height(),pixmap);

}
//登录--OK
void Login::on_loginBtn_clicked()
{
    //从控件中取出数据
    QString userName=ui->userName->text();
    QString passWord=ui->userPasswd->text();
    QString ip=ui->setServerIp->text();
    QString port=ui->setServerPort->text();

    //验证格式是否正确--正则表达式
/*
    QRegExp re(USER_REG);
    if(!re.exactMatch(userName)){
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        ui->userName->clear();
        ui->userName->setFocus();
        return;
    }
    re.setPattern(PASSWD_REG);   //莫忘：重新设置正则表达式
    if(!re.exactMatch(passWord)){
        QMessageBox::warning(this, "警告", "密码格式不正确");
        ui->userPasswd->clear();
        ui->userPasswd->setFocus();
        return;
    }
*/
    //是否保存密码
    bool isRemember=ui->rememberBox->isChecked();

    //保存到配置客户端文件--common类封装此函数接口
    m_com.writeLoginInfo(userName,passWord,isRemember);

    //打包成Json  注意密码的加密  md5
    QByteArray postDataJson=setLoginJson(userName,m_com.getStrMd5(passWord));
    //发送HTTP请求--login
    QNetworkAccessManager *manager=Common::getNetManager();
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postDataJson.size());
    QString url=QString("http://%1:%2/login").arg(ip).arg(port);
    request.setUrl(QUrl(url));

    QNetworkReply *reply=manager->post(request,postDataJson); //发送
    //接收服务器的响应
    connect(reply,&QNetworkReply::readyRead,this,[=](){
        //服务器发来的数据格式:
        /*
         * "token" 服务端发来的验证码，用来标记后续识别
         *成功:
            {
               "code":"000",
               "token":"xxx"
            }
         失败:
            {
               "code":"001",
               "token":"xxx"
            }
         */
         QByteArray data=reply->readAll();
         QJsonDocument doc=QJsonDocument::fromJson(data);
         if(doc.isNull()||doc.isEmpty()){
             cout << "doc.isNull() || doc.isEmpty()";
             return "";
         }
         if(doc.isObject()){
             QJsonObject obj=doc.object();
             QString strCode=obj.value("code").toString();
             if(strCode=="000"){
                 cout << "登陆成功";
                 QString token=obj.value("token").toString();
                 //设置登录信息，并保存到单例模式类对象中
                 LoginInfoInstance* p=LoginInfoInstance::getInstance();  //获取单例
                 p->setLoginInfo(userName,ip,port,token);
                 //一切准备就绪，切换到主页面
                 this->hide();
                 m_mainWin->showMainWindow();

             }else if(strCode=="001"){
                 QMessageBox::critical(this,"错误","登录失败，请检查帐号是否输入正确");
             }
         }
         delete reply;  //释放资源
         delete manager;
    });
}
//注册--OK
void Login::on_registerBtn_2_clicked()
{
    //从控件中取出用户输入的数据
    QString userName=ui->regUserName->text();
    QString passWord=ui->regPasswd->text();
    QString confirmPwd=ui->regConfirmPwassd->text();
    QString phone=ui->regPhone->text();
    QString email=ui->regEmail->text();

    //验证数据格式--正则表达式
    /**************************
     {
        userName:xxx;
        passWord:xxx;
        phone:xxx;
        email:xxx;
     }
    ****************************/
    if(passWord!=confirmPwd){
        QMessageBox::warning(this,"错误","两次输入密码不匹配，请重新输入！");
        ui->regPasswd->clear();
        ui->regConfirmPwassd->clear();
        return;
    }
/*一直验证错误，先放置
    QRegExp re;
    re.setPattern(USER_REG);
    if(!re.exactMatch(userName)){
        QMessageBox::warning(this, "警告", "用户名格式不正确");
        ui->regUserName->clear();
        ui->regUserName->setFocus();
        return;
    }
    re.setPattern(PASSWD_REG);   //莫忘：重新设置正则表达式
    if(!re.exactMatch(passWord)){
        QMessageBox::warning(this, "警告", "密码格式不正确");
        ui->regPasswd->clear();
        ui->regPasswd->setFocus();
        return;
    }
    re.setPattern(PHONE_REG);
    if(!re.exactMatch(phone)){
        QMessageBox::warning(this,"警告","手机号码格式不正确");
        ui->regPhone->clear();
        ui->regPhone->setFocus();
        return;
    }
    re.setPattern(EMAIL_REG);
    if(!re.exactMatch(email)){
        QMessageBox::warning(this,"警告","电子邮件格式不正确");
        ui->regEmail->clear();
        ui->regEmail->clear();
        return;
    }
*/

    //发给server端数据 --> post请求
    //注册信息打包为json格式,再作为发送数据发送到server端
    QByteArray postDataJson=setRegisterJson(userName,m_com.getStrMd5(passWord),phone,email);    //md5

    //发送Http请求协议--reg
    //    QNetworkAccessManager *manager=new QNetworkAccessManager(this);
    QNetworkAccessManager *manager=Common::getNetManager();
    if(!manager){
        exit(-1);
    }
    QNetworkRequest request;
    //设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json")); //第二个参数是数据提交的格式
    request.setHeader(QNetworkRequest::ContentLengthHeader,QVariant(postDataJson.size()));
    //server-url
    QString ip=ui->setServerIp->text();
    QString port=ui->setServerPort->text();
    QString url=QString("http://%1:%2/reg").arg(ip).arg(port);  //加 reg 指令
    request.setUrl(QUrl(url));

    QNetworkReply *reply=manager->post(request,postDataJson); //发送
    //接收server返回的数据
    connect(reply,&QNetworkReply::readyRead,this,[=](){
        QByteArray dataJson=reply->readAll();
        //根据制定的发送接收协议解析数据:(使用common类封装好的接口)
            //成功 {"code","002"}
            //用户已存在 {"code","003"}
            //失败 {"code","004"}
        QString res=m_com.getCode(dataJson);
        if(res=="002"){
            //提示注册成功
            QMessageBox::information(this,"提示","注册成功，可以登录啦！");

            //清空注册编辑内容,并自动填写好用户的用户名
            ui->regUserName->clear();
            ui->regPasswd->clear();
            ui->regConfirmPwassd->clear();
            ui->regPhone->clear();
            ui->regEmail->clear();
            ui->userName->setText(userName);
            //切换到登录界面
            ui->stackedWidget->setCurrentWidget(ui->login_page);
        }else if(res=="003"){
            //提示用户已经存在
            QMessageBox::information(this,"提示","用户已存在！");
        }else if(res=="004"){
            //提示注册失败
            QMessageBox::warning(this,"警告","注册失败！");
        }else{
            QMessageBox::critical(this,"错误","请检查相关网络设置！");
        }
        //释放资源
        delete reply;
        delete manager;
    });
}
//服务器设置--OK
void Login::on_setSave_clicked()
{
    //获取输入的数据
    QString ip=ui->setServerIp->text();
    QString port=ui->setServerPort->text();

    //正则表达式校验----------------------------------/
    //校验 ip
    QRegExp exp(IP_REG);
    if(!exp.exactMatch(ip)){
        QMessageBox::warning(this,"警告！","IP格式不正确！");
        //ui->setServerIp->clear();     //不需要清空，让用户自己改
        ui->setServerIp->setFocus();    //设置焦点
        return;
    }
    //校验 port
    QRegExp exp2(PORT_REG);
    if(!exp2.exactMatch(port)){
        QMessageBox::warning(this,"警告！","Port格式不正确！");
        ui->setServerIp->setFocus();    //设置焦点
        return;
    }

    //保存到配置文件--在common类中封装此方法
    m_com.writeWebInfo(ip,port);

    //提示设置成功，(并实现在3s后自动跳转到登录界面,以后解决)
    /*
    QTimer *timer=new QTimer(this);
    timer->start(3000);
    QMessageBox::information(this,"提示！","服务器设置成功，将在3s后自动转到登录界面");
    connect(timer,&QTimer::timeout,this,
    */
    QMessageBox::information(this,"提示","服务器设置成功！");
    ui->stackedWidget->setCurrentWidget(ui->login_page);
}
//将注册信息打包为json格式--OK
QByteArray Login::setRegisterJson(QString _user, QString _pwd, QString _phone, QString _email)
{
    //把用户的信息插入到Json对象中
    /**********Json格式************
     {
        userName:xxx;
        password:xxx;
        phone:xxx;
        email:xxx;
     }
    ****************************/
    QJsonObject usrObj;
    usrObj.insert("userName",_user);
    usrObj.insert("password",_pwd);
    usrObj.insert("phone",_phone);
    usrObj.insert("email",_email);

    QJsonDocument doc(usrObj);
    if(doc.isNull()){
        cout<<"jsonDocument.isNull() ";
        return "";
    }
    QByteArray data=doc.toJson();

    return data;
}
//将登录信息打包成Json格式
QByteArray Login::setLoginJson(QString user, QString pwd)
{
    /*json数据如下
        {
            user:xxxx,
            pwd:xxx
        }
    */
    //与注册时不同，此次使用QMap创建Json对象
    QMap<QString,QVariant> login;
    login.insert("user",user);
    login.insert("pwd",pwd);
    QJsonDocument doc=QJsonDocument::fromVariant(login);
    if(doc.isNull()){
        cout<<"doc.isNUL()";
        return "";
    }
    QByteArray data=doc.toJson();
   return data;
}
//切换到注册页面--OK
void Login::on_registerBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->register_page);
}
//设置页面返回--OK
void Login::on_setReturn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->login_page);
}
//注册页面返回--OK
void Login::on_regReturn_clicked()
{
    ui->regUserName->clear();
    ui->regPasswd->clear();
    ui->regConfirmPwassd->clear();
    ui->regPhone->clear();
    ui->regEmail->clear();
    ui->stackedWidget->setCurrentWidget(ui->login_page);
}
// 读取配置信息，设置默认登录状态，默认设置信息-初始化登陆界面---用户解密？？有疑问--
void Login::readCfg()
{
    //common类封装接口:getCfgValue
    QString user=m_com.getCfgValue("login","user");
    QString pwd=m_com.getCfgValue("login","pwd");
    QString remember=m_com.getCfgValue("login","remember");
    QString ip=m_com.getCfgValue("web_server","ip");
    QString port=m_com.getCfgValue("web_server","port");

    //server set
    ui->setServerIp->setText(ip);
    ui->setServerPort->setText(port);

    int res=0;
    //密码解密
    if(remember=="yes"){ //用户记住密码
        unsigned char encPwd[512] = {0};
        int encPwdLen = 0;
        //toLocal8Bit(), 转换为本地字符集，默认windows则为gbk编码，linux为utf-8编码
        QByteArray tmp = QByteArray::fromBase64( pwd.toLocal8Bit());
        res = DesDec( (unsigned char *)tmp.data(), tmp.size(), encPwd, &encPwdLen);
        if(res != 0)
        {
            cout << "DesDec";
            return;
        }
        #ifdef _WIN32 //如果是windows平台
            //fromLocal8Bit(), 本地字符集转换为utf-8
            ui->userPasswd->setText( QString::fromLocal8Bit( (const char *)encPwd, encPwdLen ) );
        #else
            ui->userPasswd->setText((const char*)encPwd);
        #endif
            ui->rememberBox->setChecked(true);
    }else{  //没有记住密码
        ui->userPasswd->clear();
        ui->rememberBox->setChecked(false);
    }
    //用户解密-----???
    unsigned char encUsr[512] = {0};
    int encUsrLen = 0;
    //toLocal8Bit(), 转换为本地字符集，如果windows则为gbk编码，如果linux则为utf-8编码
    QByteArray tmp = QByteArray::fromBase64( user.toLocal8Bit());
    res = DesDec( (unsigned char *)tmp.data(), tmp.size(), encUsr, &encUsrLen);
    if(res != 0)
    {
        cout << "DesDec";
        return;
    }

    #ifdef _WIN32 //如果是windows平台
        //fromLocal8Bit(), 本地字符集转换为utf-8
        ui->userName->setText( QString::fromLocal8Bit( (const char *)encUsr, encUsrLen ) );
    #else //其它平台
        ui->userName->setText((const char*)encUsr);
    #endif
}

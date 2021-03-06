#ifndef EMAIL_H
#define EMAIL_H

#include <QObject>
#include "simplecrypt.h"
#include <QJsonObject>
#include <QTcpSocket>
#include <QStringList>
#include <QSslSocket>
#include <QLabel>
#include <QTimer>

#include <QFile>
#include <QTextStream>

class Email : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY update)
    Q_PROPERTY(int type READ getType WRITE setType NOTIFY update)
    Q_PROPERTY(QString server READ getServer WRITE setServer NOTIFY update)
    Q_PROPERTY(QString login READ getLogin WRITE setLogin NOTIFY update)
    Q_PROPERTY(QString passwd READ getPasswd WRITE setPasswd NOTIFY update)
    Q_PROPERTY(int port READ getPort WRITE setPort NOTIFY update)
    Q_PROPERTY(bool ssl READ getSsl WRITE setSsl NOTIFY update)
    Q_PROPERTY(int result READ getResult WRITE setResult NOTIFY update)
    Q_PROPERTY(QString parser READ getParser WRITE setParser NOTIFY update)
    Q_PROPERTY(QString patternName READ getPatternName WRITE setPatternName NOTIFY update)
    Q_PROPERTY(int freq READ getFreq WRITE setFreq NOTIFY update)
    Q_PROPERTY(QString value READ getValue WRITE setValue NOTIFY update2)
    Q_PROPERTY(QString email READ getEmail WRITE setEmail NOTIFY update)
    Q_PROPERTY(int unreadCount READ getUnreadCount WRITE setUnreadCount NOTIFY update)
    Q_PROPERTY(QVariantList getErrorsList READ getErrorsList NOTIFY errorsUpdate)
public:
    explicit Email(QString name, QObject *parent = 0);
    QJsonObject toJson();
    void fromJson( QJsonObject obj);
    QString getName();
    void setName(QString name);
    int getType();
    void setType(int type);
    QString getServer();
    void setServer(QString server);
    QString getLogin();
    void setLogin(QString login);
    QString getPasswd();
    void setPasswd(QString passwd);
    int getPort();
    void setPort(int port);
    int getResult();
    void setResult(int result);
    bool getSsl();
    void setSsl(bool ssl);
    QString getParser();
    void setParser(QString parser);
    QString getPatternName();
    void setPatternName(QString patternName);
    int getLastid();
    void setLastid(int lastid);
    int getFreq();
    void setFreq(int freq);
    int getFreqCounter();
    void setFreqCounter(int freqCounter);
    QString getValue();
    void setValue(QString value);
    QString getEmail();
    void setEmail(QString email);
    int getUnreadCount();
    void setUnreadCount(int unreadCount);
    void changeFreqCounter();
    void setEdit(bool edit);

signals:
    void update();
    void update2();
    void runPattern(QString,bool);
    void addReceiveEvent(int,QString,QString);
    void errorsUpdate();
    void addToLog(QString txt);
public slots:
    void readyIMAP();
    void readyPOP3();
    QStringList parseMail(QString);
    void err(QAbstractSocket::SocketError);
    void checkMail();
    void checkState();
    void sslErrors(QList<QSslError>);
    QVariantList getErrorsList();
    void setEditing(bool e);
private:
    QString name;
    int type;
    QString server;
    QString login;
    QString passwd;
    int port;
    bool ssl;
    int result;
    QString parser;
    QString patternName;
    int lastid;
    int freq;
    int freqCounter;
    QString value;
    QString email;
    int unreadCount;

    SimpleCrypt *sc;    //ilee
    QTcpSocket *socket;
    QList<int> ll;
    QList<int> ll2;
    int licznik;
    bool odznacz;

    int ile;    //action
    bool edit;
    int mm; //matching messeges
    int lastId;

    bool info_send;
    bool editing;

    QTimer *time;
    QStringList errorsList;
    QString pobrano;
};

#endif // EMAIL_H

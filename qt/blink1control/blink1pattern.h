#ifndef BLINK1PATTERN_H
#define BLINK1PATTERN_H

#include <QObject>
#include <QList>
#include <QColor>
#include <QJsonObject>
#include <QVariant>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
class Blink1Pattern : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY colorChange)
    Q_PROPERTY(int repeats READ repeats WRITE setRepeats NOTIFY colorChange)
    Q_PROPERTY(int playcount READ playcount WRITE setPlaycount NOTIFY colorChange)
    Q_PROPERTY(QVariantList colors READ getColors NOTIFY colorChange)
    Q_PROPERTY(QVariantList times READ getTimes NOTIFY colorChange)
    Q_PROPERTY(QVariantList leds READ getLeds NOTIFY colorChange)
    Q_PROPERTY(int playing READ isPlaying NOTIFY playChange)
    Q_PROPERTY(bool isReadOnly READ isReadOnly WRITE setReadOnly NOTIFY colorChange)
public:
    Blink1Pattern(QObject *parent=0);
    Blink1Pattern(const char* name, QObject *parent=0);
    QJsonObject toJson();
    QJsonObject toJson2();
    void fromPatternStr(QString);
    void fromPatternStrWithLeds(QString);
    void fromJson( QJsonObject obj);

    QString name() const;
    void setName(const QString& name);

    int repeats();
    void setRepeats(int r);
    int playcount();
    void setPlaycount(int p);
    int isPlaying();
    void stop();
    void play();

    QList<QColor> getColorList();
    void resetObj();
    void clearArrays();
    QString patternStr();
    QString patternStrWithLeds();
    void setColors(QList<QColor>);
    void setTimes(QList<float>);
    QColor activeColor();
    float activeTime();
    bool playing();
    QVariantList getColors();
    QVariantList getTimes();
    QVariantList getLeds();
    void addColorAndTime(QString color, double time);
    void removeColorAndTime(int idx);
    void changeRepeats();
    void editColorAndTime(QString color, double time, int idx);
    void editLed(int idx, int led);
    int getLed(int idx);
    QColor getColor(int idx);
    double getTime(int idx);
    int date();
    void setDate(int date);
    int getCurrentLed();
    void setReadOnly(bool ro);
    bool isReadOnly();

    QTimer *t;
private:
    QString mname;
    int mrepeats;
    int mplaycount;
    int mplaypos;
    bool mplaying;
    int mdate;
    bool mreadonly;
    //int isSingleShot;
    QList<QColor> colors;
    QList<float> times;
    QList<int> leds;

public slots:
    void update();
signals:
    void colorChange();
    void setColor(QColor,QString,int);
    void playChange();

};

#endif // BLINK1PATTERN_H

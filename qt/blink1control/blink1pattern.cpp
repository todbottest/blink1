
#include "blink1pattern.h"

#include <QDebug>
//#include <QJsonArray>

Blink1Pattern::Blink1Pattern( QObject *parent) : QObject(parent)
{
    resetObj();
    setName("");
    mdate=QDateTime::currentDateTime().toTime_t();
    t=NULL;
    mreadonly=false;
}

Blink1Pattern::Blink1Pattern(const char* name, QObject *parent) : QObject(parent)
{
    setName(name);
    mdate=QDateTime::currentDateTime().toTime_t();
    t=NULL;
}

void Blink1Pattern::fromJson( QJsonObject obj)
{
    setName( obj.value("name").toString() );
    setRepeats(obj.value("repeats").toDouble());
    QString tmp=obj.value("pattern").toString();
    fromPatternStrWithLeds(tmp);
    setDate(obj.value("date").toDouble());
    setReadOnly(obj.value("readonly").toBool());

}

QJsonObject Blink1Pattern::toJson()
{
    QJsonObject obj;
    obj.insert("name", name());
    obj.insert("repeats", repeats());
    obj.insert("pattern", patternStrWithLeds());
    obj.insert("date",date());
    obj.insert("readonly",isReadOnly());
    return obj;
}
QJsonObject Blink1Pattern::toJson2()
{
    QJsonObject obj;
    obj.insert("name", name());
    obj.insert("pattern", patternStr());
    return obj;
}
void Blink1Pattern::resetObj()
{
    //mname = 0;
    mrepeats = 0;
    mplaycount = 0;
    mplaying = 0;
    mplaypos = 0;
    colors = QList<QColor>();
    times = QList<float>();
    mreadonly=false;
}

QString Blink1Pattern::patternStr()
{
    QString str = QString("%1").arg( mrepeats );
    for( int i=0; i<colors.count(); i++) {
        str.append( QString(",%1,%2").arg(colors.at(i).name().replace("#","%23")).arg(times.at(i)));
    }
    return str;
}

QString Blink1Pattern::patternStrWithLeds()
{
    QString str = QString("%1").arg( mrepeats );
    for( int i=0; i<colors.count(); i++) {
        str.append( QString(",%1,%2,%3").arg(colors.at(i).name()).arg(times.at(i)).arg(leds.at(i)));
    }
    return str;
}

void Blink1Pattern::fromPatternStr(QString tmp){
    QStringList list=tmp.split(",");
    setRepeats(list.at(0).toDouble());
    for(int i=1;i<list.count();i+=2){
        addColorAndTime(list.at(i),list.at(i+1).toDouble());
    }
}

void Blink1Pattern::fromPatternStrWithLeds(QString tmp){
    QStringList list=tmp.split(",");
    setRepeats(list.at(0).toDouble());
    if(list.count()>=4 && list.at(3).indexOf(QRegExp("#([0-9a-fA-F]{6})"))!=-1){
        fromPatternStr(tmp);
        return;
    }
    for(int i=1;i<list.count();i+=3){
        addColorAndTime(list.at(i),list.at(i+1).toDouble());
        editLed(colors.count()-1,list.at(i+2).toInt());
    }
}

QString Blink1Pattern::name() const
{
    return mname;
}

void Blink1Pattern::setName(const QString &name)
{
    mname = name;
    emit colorChange();
}

void Blink1Pattern::update()
{
    if(!mplaying) return;

    mplaypos++;
    if(mplaypos==colors.count()){
        mplaypos=0;
        mplaycount++;
    }
    if(mplaycount==mrepeats+(mrepeats!=-1)?1:0){
        stop();
        return;
    }
    t->stop();
    delete t;
    t=new QTimer(this);
    t->setInterval((int)(activeTime()*1000));
    t->setSingleShot(true);
    connect(t,SIGNAL(timeout()),this,SLOT(update()));
    t->start();

    emit setColor(activeColor(),mname,activeTime());
}

void Blink1Pattern::play()
{
    //qDebug() << "play:";
    if(mplaying) return;
    mplaycount=0;
    mplaypos=0;
    mplaying=true;
    emit playChange();
    t=new QTimer(this);
    t->setInterval((int)(activeTime()*1000));
    t->setSingleShot(true);
    connect(t,SIGNAL(timeout()),this,SLOT(update()));
    t->start();
    emit setColor(activeColor(),mname,activeTime());
}

void Blink1Pattern::stop()
{
    mplaycount=0;
    mplaypos=0;
    mplaying=false;
    emit playChange();
    emit setColor(QColor("#000000"),"",0);
    if(t!=NULL){
        t->stop();
        delete t;
        t=NULL;
    }
}


int Blink1Pattern::repeats()
{
    return mrepeats;
}
int Blink1Pattern::playcount()
{
    return mplaycount;
}


void Blink1Pattern::setRepeats(int r)
{
    mrepeats = r;
    emit colorChange();
}
void Blink1Pattern::setPlaycount(int p)
{
    mplaycount = p;
    emit colorChange();
}
void Blink1Pattern::setColors(QList<QColor> lc){
    colors=lc;
    emit colorChange();
}
void Blink1Pattern::setTimes(QList<float> lt){
    times=lt;
    emit colorChange();
}
QColor Blink1Pattern::activeColor(){
    return colors.at(mplaypos);//mplaycount%colors.count());
}
float Blink1Pattern::activeTime(){
    return times.at(mplaypos);//mplaycount%times.count());
}
bool Blink1Pattern::playing(){
    return mplaying;
}
QList<QColor> Blink1Pattern::getColorList(){
    return colors;
}
void Blink1Pattern::clearArrays(){
    colors.clear();
    times.clear();
    leds.clear();
}
QVariantList Blink1Pattern::getColors(){
    QVariantList v;
    for(int i=0;i<colors.count();i++){
        v.append(QVariant(colors.at(i).name()));
    }
    return v;
}
QVariantList Blink1Pattern::getTimes(){
    QVariantList v;
    for(int i=0;i<colors.count();i++){
        v.append(QVariant(times.at(i)));
    }
    return v;
}
QVariantList Blink1Pattern::getLeds(){
    QVariantList v;
    for(int i=0;i<leds.count();i++){
        v.append(QVariant(leds.at(i)));
    }
    return v;
}

void Blink1Pattern::addColorAndTime(QString color, double time){
    colors.append(QString(color));
    times.append(time);
    leds.append(0);
    emit colorChange();
}
void Blink1Pattern::removeColorAndTime(int idx){
    if(colors.count()==1) return;
    colors.removeAt(idx);
    times.removeAt(idx);
    leds.removeAt(idx);
    emit colorChange();
}
void Blink1Pattern::changeRepeats(){
    mrepeats=mrepeats+1;
    if(mrepeats==5) mrepeats=-1;
    emit colorChange();
}

void Blink1Pattern::editColorAndTime(QString color, double time, int idx) {
    colors[idx] = QColor(color);
    times[idx] = time;
    emit colorChange();
}
void Blink1Pattern::editLed(int idx, int led){
    leds[idx]=led;
    emit colorChange();
}
int Blink1Pattern::getLed(int idx){
    return leds[idx];
}

int Blink1Pattern::date(){
    return mdate;
}
void Blink1Pattern::setDate(int date){
    this->mdate=date;
}
int Blink1Pattern::getCurrentLed(){
    return leds.at(mplaypos);
}
void Blink1Pattern::setReadOnly(bool ro){
    this->mreadonly=ro;
    emit colorChange();
}
bool Blink1Pattern::isReadOnly(){
    return mreadonly;
}
int Blink1Pattern::isPlaying(){
    return (mplaying)?1:0;
}
QColor Blink1Pattern::getColor(int idx){
    return colors[idx];
}
double Blink1Pattern::getTime(int idx){
    return times[idx];
}

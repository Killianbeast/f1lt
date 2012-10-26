#ifndef SEASONDATA_H
#define SEASONDATA_H

#include <QMap>
#include <QPixmap>
#include <QString>
#include <QTime>
#include <QVector>

#include "f1ltcore.h"
#include "ltpackets.h"

//Two additional stuctures that hold the basic info about all teams and events. Data is loaded from the file.
struct LTTeam
{
    //used for sorting
    bool operator < (const LTTeam &lt) const
    {
        return driver1No < lt.driver1No;
    }

    QString teamName;

    QString driver1Name;
    QString driver1ShortName;
    int driver1No;

    QString driver2Name;
    QString driver2ShortName;
    int driver2No;

    QPixmap carImg;
};

struct LTEvent
{
    LTEvent() : eventNo(0), laps(0), trackImg(100,100) { }
    int eventNo;
    QString eventName;
    QString eventShortName;
    QString eventPlace;
    int laps;
    QDate fpDate;
    QDate raceDate;

    QPixmap trackImg;

    bool operator<(const LTEvent &event) const
    {
        return fpDate < event.fpDate;
    }
};

class CarThumbnailsFactory
{
public:
    ~CarThumbnailsFactory();
    QList<QPixmap*> *loadCarThumbnails(int size, bool clear = true);
    QPixmap &getCarThumbnail(int no, int size);

private:
    QMap<int, QList<QPixmap*> > carThumbnails;
    QPixmap nullPixmap;
};

//this class contains all the basic informations about the season, like quali and fp lengths, etc.
class SeasonData
{
public:
    static SeasonData &getInstance()
    {
        static SeasonData instance;
        return instance;
    }
    bool loadSeasonFile();

    QPixmap getCarImg(int no);
    int getFPNumber();
    int getEventNo(QDate);
    int getFPLength();
    int getFPLength(int fp);
    int getQualiLength(int q);

    LTEvent getEvent(int);
    const LTEvent &getEvent(const QDate&) const;
    const LTEvent &getCurrentEvent() const;
    const LTEvent &getNextEvent() const;

    QTime correctFPTime(const QTime &time);
    QTime correctQualiTime(const QTime &time, int qualiPeriod);

    QString getDriverName(QString &);
    QString getDriverLastName(const QString&);
    QString getDriverShortName(const QString&);
    QString getDriverNameFromShort(const QString&);
    QString getEventNameFromShort(const QString&);
    int getDriverNo(const QString &name);
    QString getTeamName(int);
    QStringList getDriversList();
    QStringList getDriversListShort();

    QColor getColor(LTPackets::Colors color) { return colors[color]; }

    QVector<LTTeam> &getTeams() { return ltTeams; }
    void setTeams(const QVector<LTTeam> &teams) { ltTeams = teams; }
    QVector<LTEvent> &getEvents() { return ltEvents; }

    int timeToMins(const QTime &time);
    int timeToSecs(const QTime &time);

    void fillEventNamesMap();

    CarThumbnailsFactory &getCarThumbnailsFactory() { return carThumbnailsFactory; }

private:
    SeasonData();
    SeasonData(const SeasonData &) { }
    int season;
    int fpLengths[3];
    int qualiLengths[3];

    QVector<LTTeam> ltTeams;
    QVector<LTEvent> ltEvents;

    int baseEventId;
    int baseEventInc;

    QList<QColor> colors;

    QMap<QString, QString> eventNamesMap;
    CarThumbnailsFactory carThumbnailsFactory;
};

#endif // SEASONDATA_H

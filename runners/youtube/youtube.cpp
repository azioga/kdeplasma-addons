/******************************************************************************
 *  Copyright (C) 2012 by Shaun Reich <sreich@kde.org                         *
 *                                                                            *
 *  This library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation; either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library; see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#include "youtube.h"
#include "tubejob.h"

#include <KDebug>
#include <KToolInvocation>

#include <QtCore/QTimer>
#include <QtCore/QWaitCondition>
#include <QtCore/QEventLoop>
#include <qjson/parser.h>
#include <kde4/KDE/KRun>

//TODO: I'd really *love* to be able to embed a video *inside* krunner. you know how sexy that'd be? answer: very much.
//but seeing as youtube doesn't fully support html5 (only for non-ad'ed videos), i guess i'll have to hold off on it?
YouTube::YouTube(QObject *parent, const QVariantList& args)
    : Plasma::AbstractRunner(parent, args)
{
    Q_UNUSED(args);
    setObjectName(QLatin1String("YouTube"));
    setIgnoredTypes(Plasma::RunnerContext::FileSystem | Plasma::RunnerContext::Directory | Plasma::RunnerContext::NetworkLocation);

    Plasma::RunnerSyntax s(QLatin1String(":q:"), i18n("Finds YouTube video matching :q:."));
    s.addExampleQuery(QLatin1String("youtube :q:"));
    addSyntax(s);

    addSyntax(Plasma::RunnerSyntax(QLatin1String("youtube"), i18n("Lists the videos matching the query, using YouTube search")));
    setSpeed(SlowSpeed);
    setPriority(LowPriority);
}

YouTube::~YouTube()
{
}

void YouTube::match(Plasma::RunnerContext &context)
{
    kDebug() << "YouTube Runner, MATCH MADE";

    const QString term = context.query();

    if (term.length() < 3) {
        return;
    }

    if (!context.isValid()) {
        return;
    }

    kDebug() << "YouTube Runner, Sleeping for 1 seconds";

    QEventLoop loop;
    // Wait a second, we don't want to  query on every keypress
    QMutex mutex;
    QWaitCondition waiter;
    mutex.lock();
    waiter.wait(&mutex, 1000);
    mutex.unlock();

    TubeJob tubeJob(term);
    connect(&tubeJob, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    parseJson(tubeJob.data(), context);
}

void YouTube::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    KRun::runUrl(match.data().toString(), "text/html", 0);
}

void YouTube::parseJson(const QByteArray& data, Plasma::RunnerContext &context)
{
    kDebug() << "JSON PARSER ONLINE";
    QJson::Parser parser;
    const QVariantMap resultsMap = parser.parse(data).toMap();

    QVariantMap related = resultsMap.value("feed").toMap();
//    kDebug() << related.value("entry").typeName();

    QVariantList subList = related.value("entry").toList();

    const QString term = context.query();

    foreach (const QVariant& variant, subList) {
        QVariantMap subMap = variant.toMap();

        QVariantList linkList = subMap.value("link").toList();
        //FIXME: hardcoded..
        const QString& url = linkList.at(0).toMap().value("href").toString();

        QVariantMap titleMap = subMap.value("title").toMap();
        const QString& title = titleMap.value("$t").toString();

        QVariantMap subSubMap = subMap.value("media$group").toMap();

        QVariantList thumbnailList = subSubMap.value("media$thumbnail").toList();

        //FIXME: horrible horrible assumption
        const QString& thumbnail = thumbnailList.at(0).toMap().value("url").toString();
        kDebug() << "THUMBNAIL URL: " << thumbnail;


        Plasma::QueryMatch match(this);
        match.setType(Plasma::QueryMatch::PossibleMatch);

        //  match.setRelevance(1.0);
        //  match.setIcon(m_icon);
        match.setData(url);
        match.setText(QString(title + " on YouTube"));

        context.addMatch(term, match);
    }

//    foreach (const QVariant& variant, related) {
//        QVariantMap submap = variant.toMap();
//
//        kDebug() << "FirstURL:" << submap.value("FirstURL");
//        kDebug() << "Text:" << submap.value("Text");
//        kDebug() << "Icon:" << submap.value("Icon").toMap().value("URL");
//    }
}

#include "youtube.moc"

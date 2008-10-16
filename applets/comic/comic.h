/***************************************************************************
 *   Copyright (C) 2007 by Tobias Koenig <tokoe@kde.org>                   *
 *   Copyright (C) 2008 by Marco Martin <notmart@gmail.com>                *
 *   Copyright (C) 2008 Matthias Fuchs <mat69@gmx.net>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef COMIC_H
#define COMIC_H

#include <QtCore/QDate>
#include <QtCore/QStringList>
#include <QtGui/QImage>

#include <plasma/applet.h>
#include <plasma/dataengine.h>
#include <solid/networking.h>

class ConfigWidget;
class FullViewWidget;
class QTimer;
class QAction;
class FadingItem;
namespace Plasma {
    class Frame;
    class PushButton;
}

class ComicApplet : public Plasma::Applet
{
    Q_OBJECT

    public:
        ComicApplet( QObject *parent, const QVariantList &args );
        ~ComicApplet();

        void init();
        void paintInterface( QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect );
        virtual QList<QAction*> contextualActions();

    public Q_SLOTS:
        void dataUpdated( const QString &name, const Plasma::DataEngine::Data &data );
        void createConfigurationInterface( KConfigDialog *parent );

    private Q_SLOTS:
        void slotChosenDay( const QDate &date );
        void slotNextDay();
        void slotPreviousDay();
        void slotFirstDay();
        void slotCurrentDay();
        void slotSaveComicAs();
        void applyConfig();
        void networkStatusChanged( Solid::Networking::Status );
        void checkDayChanged();
        void scaleToContent();

    protected:
        void mousePressEvent( QGraphicsSceneMouseEvent* );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent* );
        void hoverEnterEvent( QGraphicsSceneHoverEvent* );
        void hoverLeaveEvent( QGraphicsSceneHoverEvent* );
        void constraintsEvent( Plasma::Constraints constraints );

    private:
        void updateComic( const QString &identifierSuffix = QString() );
        void updateButtons();
        void updateContextMenu();
        void loadConfig();
        void saveConfig();
        void updateSize();
        void buttonBar();

        QImage mImage;
        QDate mCurrentDay;
        QDate mIdentifierSuffixDate;
        QUrl mWebsiteUrl;
        QString mComicIdentifier;
        QString mNextIdentifierSuffix;
        QString mPreviousIdentifierSuffix;
        QString mFirstDayIdentifierSuffix;
        QString mComicAuthor;
        QString mComicTitle;
        QString mStripTitle;
        QString mAdditionalText;
        QString mSuffixType;
        QString mShownIdentifierSuffix;
        int mIdentifierSuffixNum;
        ConfigWidget *mConfigWidget;
        bool mScaleComic;
        bool mShowPreviousButton;
        bool mShowNextButton;
        bool mShowComicUrl;
        bool mShowComicAuthor;
        bool mShowComicTitle;
        bool mShowComicIdentifier;
        QTimer *mDateChangedTimer;
        QList<QAction*> mActions;
        FullViewWidget *mFullViewWidget;
        QAction *mActionGoFirst;
        QAction *mActionGoLast;
        QMap< QString, int > mMaxStripNum;
        Plasma::Frame *mFrame;
        FadingItem *mFadingItem;
        Plasma::PushButton *mPrevButton;
        Plasma::PushButton *mNextButton;
        QSizeF mIdealSize;
        bool mArrowsOnHover;
};

K_EXPORT_PLASMA_APPLET(comic, ComicApplet)

#endif

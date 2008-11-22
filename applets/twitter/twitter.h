/***************************************************************************
 *   Copyright (C) 2007 by André Duffeck <duffeck@kde.org>                 *
 *   Copyright (C) 2007 Chani Armitage <chanika@gmail.com>                 *
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

#ifndef TWITTER_H
#define TWITTER_H

#include <QMap>

#include <Plasma/PopupApplet>
#include <Plasma/DataEngine>

#include "ui_configuration.h"

class KLineEdit;
class KTextEdit;
class KTextBrowser;
class QSpinBox;
class QCheckBox;
class QGraphicsLinearLayout;
class QGraphicsProxyWidget;
class KColorScheme;

namespace KWallet
{
    class Wallet;
}

namespace Plasma
{
    class Svg;
    class FlashingLabel;
    class IconWidget;
    class TextEdit;
    class WebContent;
    class Frame;
    class ServiceJob;
}

struct Tweet {
    Plasma::Frame *frame;
    Plasma::IconWidget *icon;
    KTextBrowser *content;
    QGraphicsProxyWidget *contentProxy;
    Plasma::IconWidget *favIcon;
};

class Twitter : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        Twitter(QObject *parent, const QVariantList &args);
        ~Twitter();
        void init();
        void constraintsEvent(Plasma::Constraints constraints);
        QGraphicsWidget *graphicsWidget();


    public slots:
        void dataUpdated(const QString &name, const Plasma::DataEngine::Data &data);
        void createConfigurationInterface( KConfigDialog *parent );
        /**
         * read from the opened KWallet
         * @param success whether we got to open it
         */
        void readWallet(bool success);
        /**
         * write to the opened KWallet
         * @param success whether we got to open it
         */
        void writeWallet(bool success);

    protected slots:
        void configAccepted();
        void updateStatus();
        void downloadHistory();
        void openProfile();
        void editTextChanged();
        void serviceFinished(Plasma::ServiceJob *job);

        void themeChanged();
        void showTweets();

    protected:
        QString timeDescription( const QDateTime &dt );
        bool eventFilter(QObject *obj, QEvent *event);
        void popupEvent(bool show);
        void focusInEvent(QFocusEvent *event);

    private:
        /**
         * attempt to get the wallet from KWallet
         * */
        void getWallet();
        /**
         * convenience function for setting the folder
         */
        bool enterWalletFolder(const QString &folder);
        /**
         * disable the UI until user is authenticated
         */
        void setAuthRequired(bool auth);
        /**
         * write the password to config instead of wallet
         */
        void writeConfigPassword();

        /**
         * generates and sets the popup icon
         */
        void paintIcon();

        Plasma::Svg *m_theme;
        Plasma::TextEdit *m_statusEdit;
        Plasma::WebContent *m_historyEdit;
        Plasma::FlashingLabel *m_flash;
        Plasma::IconWidget *m_icon;
        QGraphicsLinearLayout *m_layout;
        QGraphicsLinearLayout *m_headerLayout;
        QGraphicsWidget *m_graphicsWidget;

        KLineEdit *m_usernameEdit;
        KLineEdit *m_passwordEdit;
        QSpinBox *m_historySizeSpinBox;
        QSpinBox *m_historyRefreshSpinBox;
        QCheckBox *m_checkIncludeFriends;

        QString m_username;
        QString m_password;
        int m_historySize;
        int m_historyRefresh;
        int m_newTweets;
        bool m_includeFriends;

        Plasma::DataEngine *m_engine;
        Plasma::Service *m_service;
        Plasma::Service *m_profileService;
        QString m_curTimeline;

        QMap< QString, QPixmap > m_pictureMap;
        QMap< QString, Plasma::DataEngine::Data > m_tweetMap;
        QList< Tweet > m_tweetWidgets;

        uint m_lastTweet;
        KWallet::Wallet *m_wallet;
        enum WalletWait { None=0, Read, Write };
        WalletWait m_walletWait;

        KColorScheme *m_colorScheme;
        Ui::TwitterConfig configUi;
};

K_EXPORT_PLASMA_APPLET(twitter, Twitter)

#endif

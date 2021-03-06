#ifndef FLIST_SESSION_H
#define FLIST_SESSION_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include <QSslError>

#include "flist_channelsummary.h"

class FAccount;
class FChannel;
class FCharacter;
class JSONNode;
class QSslSocket;

class FSession : public QObject
{
Q_OBJECT
public:
	explicit FSession(FAccount *account, QString &character, QObject *parent = 0);
	~FSession();

	QString getSessionID() {return sessionid;}

	void connectSession();
	
	void wsSend(const char *command);
	void wsSend(const char *command, JSONNode &nodes);
	void wsSend(std::string &data);
	void wsRecv(std::string packet);

	bool isCharacterOnline(QString name) {return characterlist.contains(name);}
	bool isCharacterOperator(QString name) {return operatorlist.contains(name);}
	bool isCharacterIgnored(QString name) {return ignorelist.contains(name.toLower(), Qt::CaseInsensitive);}
	bool isCharacterFriend(QString name) {return friendslist.contains(name);}
	FCharacter *addCharacter(QString name);
	FCharacter *getCharacter(QString name) {return characterlist.contains(name) ? characterlist[name] : 0;}
	void removeCharacter(QString name);
	int getCharacterCount() {return characterlist.count();}
	QString getCharacterUrl(QString name) {return "https://www.f-list.net/c/" + name + "/";} //todo: HTTP request character encoding. //todo: Get server address from FServer?
	QString getCharacterHtml(QString name);

	QStringList &getFriendsList() {return friendslist;}
	QStringList &getIgnoreList() {return ignorelist;}

	void joinChannel(QString name);
	void createPublicChannel(QString name);
	void createPrivateChannel(QString name);
	FChannel *addChannel(QString name, QString title);
	FChannel *getChannel(QString name);

	void sendChannelMessage(QString channelname, QString message);
	void sendChannelAdvertisement(QString channelname, QString message);
	void sendCharacterMessage(QString charactername, QString message);
	void sendChannelLeave(QString channelname);
	void sendConfirmStaffReport(QString callid);

signals:
	void socketErrorSignal(QAbstractSocket::SocketError);
	void recvMessage(QString type, QString session, QString chan, QString sender, QString message);

public slots:
	void socketConnected();
	void socketError(QAbstractSocket::SocketError);
	void socketSslError(QList<QSslError> sslerrors);
	void socketReadReady();

public:
	bool connected;
	FAccount *account;
	QString sessionid;
	QString character;

	//QTcpSocket *tcpsocket;
	QSslSocket *tcpsocket;

private:
	QHash<QString, FCharacter *> characterlist; //< List of all known characters on the server/session.
	QStringList friendslist; //<List of friends for this session's character.
	QStringList bookmarklist; //<List of friends for this session's character.
	QMap<QString, QString> operatorlist; //<List of all known characters that are chat operators (indexed by lower case).
	QStringList ignorelist; //<List of all characters that are being ignored.
	QHash<QString, FChannel *> channellist; //<List of channels that this session has joined (or was previously joined to).
public:
	QStringList autojoinchannels; //<List of channels the client should join upon connecting.
	QHash<QString, QString> servervariables; //<List of variables as reported by the server.

	QList<FChannelSummary> knownchannellist; //<List of known public channels, as reported by the server.
	QList<FChannelSummary> knownopenroomlist; //<List of known open rooms, as reported by the server.

private:


	bool wsready;
	std::string socketreadbuffer;

#define COMMAND(name) void cmd##name(std::string &rawpacket, JSONNode &nodes)
	COMMAND(ADL);
	COMMAND(AOP);
	COMMAND(DOP);

	COMMAND(SFC);

	COMMAND(CDS);
	COMMAND(CIU);
	COMMAND(ICH);
	COMMAND(JCH);
	COMMAND(LCH);
	COMMAND(RMO);

	COMMAND(LIS);
	COMMAND(NLN);
	COMMAND(FLN);
	COMMAND(STA);

	COMMAND(CBUCKU);
	COMMAND(CBU);
	COMMAND(CKU);

	COMMAND(COL);
	COMMAND(COA);
	COMMAND(COR);
	
	COMMAND(BRO);
	COMMAND(SYS);

	COMMAND(CON);
	COMMAND(HLO);
	COMMAND(IDN);
	COMMAND(VAR);

	COMMAND(FRL);
	COMMAND(IGN);

	QString makeMessage(QString message, QString charactername, FCharacter *character, FChannel *channel = 0, QString prefix = "", QString postfix = "");
	COMMAND(LRP);
	COMMAND(MSG);
	COMMAND(PRI);
	COMMAND(RLL);

	COMMAND(TPN);

	COMMAND(KID);
	COMMAND(PRD);

	COMMAND(CHA);
	COMMAND(ORS);

	COMMAND(RTB);

	COMMAND(ZZZ);

	COMMAND(ERR);

	COMMAND(PIN);
#undef COMMAND
	
};

#endif // FLIST_SESSION_H

// ----------------------------------------------------------
/*!
	\class AEIAgentPing
	\title
	\brief Template file files/cppheader/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 20:04:51
	\endlist
*/
// ----------------------------------------------------------

// Class header
#include "aeiagentping.h"

// Namesapce
using namespace ARB;


// -----------
/*!
	\fn

	Doc.
*/

AEIAgentPing::AEIAgentPing(QObject* parent) : AThreadAgentTemplate(parent) {

	_A_DEBUG << "AEIAgentPing created";
}


// -----------
/*!
	\fn

	Doc.
*/

AEIAgentPing::~AEIAgentPing(void) {

	_A_DEBUG << "AEIAgentPing deleted";
}


// -----------
/*!
	\fn

	Doc.
*/

void AEIAgentPing::slRun(void) {

	_A_DEBUG << "AEIAgentPing::slRun";
	_A_DEBUG << "pCookie:" << pCookie << "pNodeFullName" << pNodeFullName;

	const QByteArray oBytesNodeFullName = pNodeFullName.toUtf8();
	const QByteArray oCookie = pCookie.toUtf8();
	const int oAtPosition = pNodeFullName.indexOf('@');

	if (oAtPosition <= 0 || oAtPosition == pNodeFullName.size() - 1) {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Invalid node name '%1', expected name@host").arg(pNodeFullName));
		emit this->sgFinished();
		return;
	}

	const QByteArray oRemoteAlive = oBytesNodeFullName.left(oAtPosition);
	const QString oRemoteHost  = QString::fromUtf8(oBytesNodeFullName.mid(oAtPosition + 1));
	const QDeadlineTimer oTimer(inTimeoutMs);
	quint16 oNodePort = 0;
	quint16 oHighestVersion = 0;

	{
		QTcpSocket oEPMD;
		oEPMD.connectToHost(oRemoteHost,kEpmdPort);
		if (!oEPMD.waitForConnected(mRemainingMs(oTimer))) {
			pOutput->pStatus = _A_ENUM_STATUS::Error;
			pOutput->pValue = QVariant::fromValue(QStringLiteral("Cannot reach epmd on %1:%2: %3").arg(oRemoteHost).arg(kEpmdPort).arg(oEPMD.errorString()));
			emit this->sgFinished();
			return;
		}

		QByteArray oRequest;
		oRequest.append(kPortPlease2Req);
		oRequest.append(oRemoteAlive);
		if (!mSendPacket16(oEPMD,oRequest,oTimer)) {
			pOutput->pStatus = _A_ENUM_STATUS::Error;
			pOutput->pValue = QVariant::fromValue(QStringLiteral("Failed to send request to epmd"));
			emit this->sgFinished();
			return;
		}

		char oHeader[2];
		if (!mReadExact(oEPMD,oHeader,2,oTimer) || oHeader[0] != kPort2Resp) {
			pOutput->pStatus = _A_ENUM_STATUS::Error;
			pOutput->pValue = QVariant::fromValue(QStringLiteral("Bad or no response from epmd"));
			emit this->sgFinished();
			return;
		}
		if (oHeader[1] != 0) {
			pOutput->pStatus = _A_ENUM_STATUS::Error;
			pOutput->pValue = QVariant::fromValue(QStringLiteral("Node '%1' is not registered in epmd on %2").arg(QString::fromUtf8(oRemoteAlive), oRemoteHost));
			emit this->sgFinished();
			return;
		}

		char oBody[8];
		if (!mReadExact(oEPMD,oBody,8,oTimer)) {
			pOutput->pStatus = _A_ENUM_STATUS::Error;
			pOutput->pValue = QVariant::fromValue(QStringLiteral("Truncated response from epmd"));
			emit this->sgFinished();
			return;
		}
		oNodePort = mGet16(oBody);
		oHighestVersion = mGet16(oBody + 4);
		_A_DEBUG << "epmd -> nodePort:" << oNodePort << "highestVersion:" << oHighestVersion;
	}

	const QString localFqdn = QString::fromLatin1(mLocalHostPart(true));
	const bool isLocalTarget =
		oRemoteHost.compare(QStringLiteral("localhost"), Qt::CaseInsensitive) == 0 ||
		oRemoteHost == QStringLiteral("127.0.0.1") ||
		oRemoteHost.compare(localFqdn, Qt::CaseInsensitive) == 0 ||
		oRemoteHost.compare(QHostInfo::localHostName(), Qt::CaseInsensitive) == 0;

	QTcpSocket oSocket;
	if (isLocalTarget) {
		oSocket.connectToHost(QHostAddress::LocalHost, oNodePort);
	} else {
		oSocket.connectToHost(oRemoteHost, oNodePort);
	}

	if (!oSocket.waitForConnected(mRemainingMs(oTimer)) || oSocket.state() != QAbstractSocket::ConnectedState) {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(
			QStringLiteral("Cannot connect to %1:%2 (%3)").arg(oRemoteHost).arg(oNodePort).arg(oSocket.errorString())
		);
		emit this->sgFinished();
		return;
	}
	_A_DEBUG << "sock peer:" << oSocket.peerAddress().toString() << "local:" << oSocket.localAddress().toString();

	if (!oSocket.waitForConnected(mRemainingMs(oTimer))) {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Cannot connect to %1:%2: %3").arg(oRemoteHost).arg(oNodePort).arg(oSocket.errorString()));
		emit this->sgFinished();
		return;
	}

	static std::atomic<quint64> oCounter{0};
	const bool oLongNames = oRemoteHost.contains(QLatin1Char('.'));
	const QByteArray hostPart = (oRemoteHost == QStringLiteral("127.0.0.1") || oRemoteHost == QStringLiteral("localhost"))
		? oRemoteHost.toLatin1()
		: mLocalHostPart(oLongNames);
	const QByteArray oThisNode = QStringLiteral("qtping_%1_%2@")
		.arg(QCoreApplication::applicationPid()).arg(oCounter.fetch_add(1)).toLatin1() + hostPart;
	_A_DEBUG << "thisNode:" << oThisNode;

	const bool oUseV6 = oHighestVersion >= 6;
	QByteArray oSendName;
	if (oUseV6) {
		const quint32 oCreation = QRandomGenerator::global()->generate() | 1u;
		oSendName.append('N');
		mPut64(oSendName,kFlagsV6);
		mPut32(oSendName,oCreation);
		mPut16(oSendName,quint16(oThisNode.size()));
		oSendName.append(oThisNode);
	} else {
		oSendName.append('n');
		mPut16(oSendName,5);
		mPut32(oSendName,quint32(kFlagsV5));
		oSendName.append(oThisNode);
	}
	if (!mSendPacket16(oSocket,oSendName,oTimer)) {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Failed to send handshake name"));
		emit this->sgFinished();
		return;
	}

	QByteArray oPacket;
	if (!mReadPacket16(oSocket,oPacket,oTimer) || oPacket.isEmpty() || oPacket[0] != 's') {
		_A_DEBUG << "readPacket16 failed, state:" << oSocket.state() << "err:" << oSocket.errorString() << "pkt:" << oPacket.toHex();
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("No handshake status from %1").arg(pNodeFullName));
		emit this->sgFinished();
		return;
	}

	const QByteArray oStatus = oPacket.mid(1);
	if (oStatus != "ok" && oStatus != "ok_simultaneous") {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Handshake rejected by %1: %2").arg(pNodeFullName, QString::fromLatin1(oStatus)));
		emit this->sgFinished();
		return;
	}

	if (!mReadPacket16(oSocket,oPacket,oTimer) || oPacket.isEmpty()) {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("No challenge from %1").arg(pNodeFullName));
		emit this->sgFinished();
		return;
	}
	quint32 oPeerChallenge = 0;
	if (oPacket[0] == 'N' && oPacket.size() >= 1 + 8 + 4 + 4 + 2) {
		oPeerChallenge = mGet32(oPacket.constData() + 9);
	} else if (oPacket[0] == 'n' && oPacket.size() >= 1 + 2 + 4 + 4) {
		oPeerChallenge = mGet32(oPacket.constData() + 7);
	} else {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Unexpected challenge message"));
		emit this->sgFinished();
		return;
	}

	const quint32 oOurChallenge = QRandomGenerator::system()->generate();
	QByteArray oReply;
	oReply.append('r');
	mPut32(oReply,oOurChallenge);
	oReply.append(mMD5Digest(oCookie,oPeerChallenge));
	if (!mSendPacket16(oSocket,oReply,oTimer)) {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Failed to send challenge reply"));
		emit this->sgFinished();
		return;
	}

	if (!mReadPacket16(oSocket, oPacket, oTimer) || oPacket.size() != 17 || oPacket[0] != 'a') {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Authentication failed for %1 (wrong cookie?)").arg(pNodeFullName));
		emit this->sgFinished();
		return;
	}

	if (oPacket.mid(1) != mMD5Digest(oCookie, oOurChallenge)) {
		pOutput->pStatus = _A_ENUM_STATUS::Error;
		pOutput->pValue = QVariant::fromValue(QStringLiteral("Node %1 has a different cookie").arg(pNodeFullName));
		emit this->sgFinished();
		return;
	}

	oSocket.abort();

	pOutput->pStatus = _A_ENUM_STATUS::Ok;
	pOutput->pValue = QVariant::fromValue(true);

	emit sgFinished();
}


// -----------
/*!
	\fn

	Doc.
*/

void AEIAgentPing::mPut16(QByteArray& inBytes,quint16 inValue) {

	char oTemporaryBuffer[2];
	qToBigEndian(inValue,oTemporaryBuffer);
	inBytes.append(oTemporaryBuffer,2);
}


// -----------
/*!
	\fn

	Doc.
*/

void AEIAgentPing::mPut32(QByteArray& inBytes,quint32 inValue) {

	char oTemporaryBuffer[4];
	qToBigEndian(inValue,oTemporaryBuffer);
	inBytes.append(oTemporaryBuffer,4);
}


// -----------
/*!
	\fn

	Doc.
*/

void AEIAgentPing::mPut64(QByteArray& inBytes, quint64 inValue) {

	char oTemporaryBuffer[8];
	qToBigEndian(inValue,oTemporaryBuffer);
	inBytes.append(oTemporaryBuffer,8);
}


// -----------
/*!
	\fn

	Doc.
*/

quint16 AEIAgentPing::mGet16(const char* inPointer) {

	return qFromBigEndian<quint16>(inPointer);
}


// -----------
/*!
	\fn

	Doc.
*/

quint32 AEIAgentPing::mGet32(const char* inPointer) {

	return qFromBigEndian<quint32>(inPointer);
}


// -----------
/*!
	\fn

	Doc.
*/

int AEIAgentPing::mRemainingMs(const QDeadlineTimer& inTimer) {

	return int(std::max<qint64>(1,inTimer.remainingTime()));
}


// -----------
/*!
	\fn

	Doc.
*/

bool AEIAgentPing::mWriteAll(QTcpSocket& inSocket,const QByteArray& inData,const QDeadlineTimer& inTimer) {

	if (inSocket.state() != QAbstractSocket::ConnectedState) return false;

	qint64 oTotalWritten = 0;
	while (oTotalWritten < inData.size()) {
		const qint64 oWritten = inSocket.write(inData.constData() + oTotalWritten,inData.size() - oTotalWritten);
		if (oWritten < 0) return false;
		oTotalWritten += oWritten;
	}

	inSocket.flush();

	while (inSocket.bytesToWrite() > 0) {
		if (inTimer.hasExpired() || !inSocket.waitForBytesWritten(mRemainingMs(inTimer))) {
			return inSocket.bytesToWrite() == 0;
		}
	}

	return true;
}


// -----------
/*!
	\fn

	Doc.
*/

bool AEIAgentPing::mReadExact(QTcpSocket& inSocket, char* inBuffer, qint64 inNumber, const QDeadlineTimer& inTimer) {

	qint64 oTotalBytesGotten = 0;

	while (oTotalBytesGotten < inNumber) {
		if (inSocket.bytesAvailable() == 0) {
			if (inTimer.hasExpired() || !inSocket.waitForReadyRead(mRemainingMs(inTimer))) return false;
		}
		const qint64 oReadCount = inSocket.read(inBuffer + oTotalBytesGotten, inNumber - oTotalBytesGotten);
		if (oReadCount < 0) return false;
		oTotalBytesGotten += oReadCount;
	}

	return true;
}


// -----------
/*!
	\fn

	Doc.
*/

bool AEIAgentPing::mSendPacket16(QTcpSocket& inSocket,const QByteArray& inPayload,const QDeadlineTimer& inTimer) {

	QByteArray oPacket;
	mPut16(oPacket,quint16(inPayload.size()));
	oPacket.append(inPayload);
	return mWriteAll(inSocket,oPacket,inTimer);
}


// -----------
/*!
	\fn

	Doc.
*/

bool AEIAgentPing::mReadPacket16(QTcpSocket& inSocket, QByteArray& inOut, const QDeadlineTimer& inTimer) {

	char oLength[2];
	if (!mReadExact(inSocket,oLength,2,inTimer)) return false;
	inOut.resize(mGet16(oLength));
	return inOut.isEmpty() || mReadExact(inSocket,inOut.data(),inOut.size(),inTimer);
}


// -----------
/*!
	\fn

	Doc.
*/

QByteArray AEIAgentPing::mMD5Digest(const QByteArray& inCookie, quint32 inChallenge) {

	return QCryptographicHash::hash(
		inCookie + QByteArray::number(inChallenge),
		QCryptographicHash::Md5
	);
}


// -----------
/*!
	\fn

	Doc.
*/

QByteArray AEIAgentPing::mLocalHostPart(bool inLongNames) {

	QString oHost = QHostInfo::localHostName();
	if (!inLongNames) {
		const int oDot = oHost.indexOf(QLatin1Char('.'));
		if (oDot > 0) oHost.truncate(oDot);
	} else if (!oHost.contains(QLatin1Char('.'))) {
		const QString oDomain = QHostInfo::localDomainName();
		oHost = oDomain.isEmpty() ? QStringLiteral("127.0.0.1") : oHost + QLatin1Char('.') + oDomain;
	}
	return oHost.toLatin1();
}

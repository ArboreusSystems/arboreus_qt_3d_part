// ----------------------------------------------------------
/*!
	\headerfile AEIAgentPing
	\title
	\brief Template file wizard/classes/cpp/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 20:04:51
	\endlist
*/
// ----------------------------------------------------------
#ifndef AEIAGENTPING_H
#define AEIAGENTPING_H

// Precompiled includes
#include <aliberlanginterface_pch.h>

// System includes

// Application includes
#include <auniversaldefinitions.h>
#include <aoutputqvariant.h>
#include <alogger.h>
#include <athreadagenttemplate.h>
#include <aliberlanginterfaceservice.h>

// Namesapces
namespace ARB {

// Constants and definitions
constexpr quint64 DFLAG_EXTENDED_REFERENCES   = (1ULL << 2);
constexpr quint64 DFLAG_DIST_MONITOR          = (1ULL << 3);
constexpr quint64 DFLAG_FUN_TAGS              = (1ULL << 4);
constexpr quint64 DFLAG_DIST_MONITOR_NAME     = (1ULL << 5);
constexpr quint64 DFLAG_NEW_FUN_TAGS          = (1ULL << 7);
constexpr quint64 DFLAG_EXTENDED_PIDS_PORTS   = (1ULL << 8);
constexpr quint64 DFLAG_EXPORT_PTR_TAG        = (1ULL << 9);
constexpr quint64 DFLAG_BIT_BINARIES          = (1ULL << 10);
constexpr quint64 DFLAG_NEW_FLOATS            = (1ULL << 11);
constexpr quint64 DFLAG_SMALL_ATOM_TAGS       = (1ULL << 14);
constexpr quint64 DFLAG_UTF8_ATOMS            = (1ULL << 16);
constexpr quint64 DFLAG_MAP_TAG               = (1ULL << 17);
constexpr quint64 DFLAG_BIG_CREATION          = (1ULL << 18);
constexpr quint64 DFLAG_SEND_SENDER           = (1ULL << 19);
constexpr quint64 DFLAG_BIG_SEQTRACE_LABELS   = (1ULL << 20);
constexpr quint64 DFLAG_EXIT_PAYLOAD          = (1ULL << 22);
constexpr quint64 DFLAG_FRAGMENTS             = (1ULL << 23);
constexpr quint64 DFLAG_HANDSHAKE_23          = (1ULL << 24);
constexpr quint64 DFLAG_UNLINK_ID             = (1ULL << 25);
constexpr quint64 DFLAG_SPAWN                 = (1ULL << 32);
constexpr quint64 DFLAG_V4_NC                 = (1ULL << 34);
constexpr quint64 DFLAG_ALIAS                 = (1ULL << 35);
constexpr quint64 DFLAG_MANDATORY_25_DIGEST   = (1ULL << 36);
constexpr quint64 DFLAG_ALTACT_SIG            = (1ULL << 37);
constexpr quint64 DFLAG_OTP_29_MANDATORY_EXTRA = (1ULL << 38);

constexpr quint64 kFlagsV5 =
	DFLAG_EXTENDED_REFERENCES | DFLAG_FUN_TAGS | DFLAG_DIST_MONITOR_NAME |
	DFLAG_NEW_FUN_TAGS | DFLAG_EXTENDED_PIDS_PORTS | DFLAG_EXPORT_PTR_TAG |
	DFLAG_BIT_BINARIES | DFLAG_NEW_FLOATS | DFLAG_SMALL_ATOM_TAGS |
	DFLAG_UTF8_ATOMS | DFLAG_MAP_TAG | DFLAG_BIG_CREATION;

constexpr quint64 kFlagsV6 = 468283523004ULL;
constexpr quint16 kEpmdPort = 4369;
constexpr char kPortPlease2Req = 122;
constexpr char kPort2Resp = 119;

// Class definitions
class AEIAgentPing : public AThreadAgentTemplate {

	Q_OBJECT

	public:

		QString pCookie;
		QString pNodeFullName;
		int inTimeoutMs = 5000;
		AOutputQVariant* pOutput = nullptr;
		ALibErlangInterfaceService* pService = nullptr;

		explicit AEIAgentPing(QObject* parent = nullptr);
		virtual ~AEIAgentPing(void);

	public slots:

		void slRun(void);

	private:

		void mPut16(QByteArray& inBytes, quint16 inValue);
		void mPut32(QByteArray& inBytes, quint32 inValue);
		void mPut64(QByteArray& inBytes, quint64 inValue);
		quint16 mGet16(const char* inPointer);
		quint32 mGet32(const char* inPointer);
		int mRemainingMs(const QDeadlineTimer& inTimer);
		bool mWriteAll(QTcpSocket& inSocket,const QByteArray& inData,const QDeadlineTimer& inTimer);
		bool mReadExact(QTcpSocket& inSocket, char* inBuffer, qint64 inNumber, const QDeadlineTimer& inTimer);
		bool mSendPacket16(QTcpSocket& inSocket,const QByteArray& inPayload,const QDeadlineTimer& inTimer);
		bool mReadPacket16(QTcpSocket& inSocket, QByteArray& inOut, const QDeadlineTimer& inTimer);
		QByteArray mMD5Digest(const QByteArray& inCookie, quint32 inChallenge);
		QByteArray mLocalHostPart(bool inLongNames);
};

} // namespace ARB

#endif // AEIAGENTPING_H

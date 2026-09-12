// ----------------------------------------------------------
/*!
	\headerfile AOpenSSL
	\title
	\brief Template file wizard/classes/cpp/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/09/2025 at 11:09:16
	\endlist
*/
// ----------------------------------------------------------

#ifndef AOPENSSL_H
#define AOPENSSL_H

// Precompiled includes
#include <aopenssl_pch.h>

// System includes

// Application includes
#include <alogger.h>
#include <aoutputqbytearray.h>

// Constants and definitions

// Namesapces
namespace ARB {

// Class definitions
class AOpenSSL : public QObject {

	Q_OBJECT

	public:

		explicit AOpenSSL(QObject* parent = nullptr);
		virtual ~AOpenSSL(void);

		void mInit(void);

		AOutputQByteArray mEncodeAES256CBC(
			const QByteArray& inBytes, const QByteArray& inKey, const QByteArray& inVector
		);
		AOutputQByteArray mDecodeAES256CBC(
			const QByteArray& inBytes, const QByteArray& inKey, const QByteArray& inVector
		);
};

} // namespace ARB

#endif // AOPENSSL_H

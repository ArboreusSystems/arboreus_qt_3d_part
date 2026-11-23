// ----------------------------------------------------------
/*!
	\headerfile ALibSSH
	\title
	\brief Template file wizard/classes/cpp/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 13:22:18
	\endlist
*/
// ----------------------------------------------------------

#ifndef ALIBSSH_H
#define ALIBSSH_H

// Precompiled includes
#include <alibssh_pch.h>

// System includes

// Application includes
#include <alogger.h>

// Namesapces
namespace ARB {

// Class definitions
class ALibSSH : public QObject {

	Q_OBJECT

	public:

		explicit ALibSSH(QObject* parent = nullptr);
		virtual ~ALibSSH(void);
};

} // namespace ARB

#endif // ALIBSSH_H

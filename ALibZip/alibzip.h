// ----------------------------------------------------------
/*!
	\headerfile ALibZip
	\title
	\brief Template file wizard/classes/cpp/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 13:07:28
	\endlist
*/
// ----------------------------------------------------------

#ifndef ALIBZIP_H
#define ALIBZIP_H

// Precompiled includes
#include <alibzip_pch.h>

// System includes

// Application includes
#include <alogger.h>

// Constants and definitions

// Namesapces
namespace ARB {

// Class definitions
class ALibZip: public QObject {

	Q_OBJECT

	public:

		explicit ALibZip(QObject* parent = nullptr);
		virtual ~ALibZip(void);
};

} // namespace ARB

#endif // ALIBZIP_H

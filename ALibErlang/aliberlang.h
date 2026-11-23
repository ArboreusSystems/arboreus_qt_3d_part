// ----------------------------------------------------------
/*!
	\headerfile ALibErlang
	\title
	\brief Template file wizard/classes/cpp/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 10:37:51
	\endlist
*/
// ----------------------------------------------------------

#ifndef ALIBERLANG_H
#define ALIBERLANG_H

// Precompiled includes
#include <aliberlang_pch.h>

// System includes

// Application includes
#include <alogger.h>

// Namesapces
namespace ARB {

// Class definitions
class ALibErlang : public QObject {

	Q_OBJECT

	public:

		explicit ALibErlang(QObject* parent = nullptr);
		virtual ~ALibErlang(void);
};

} // namespace ARB

#endif // ALIBERLANG_H

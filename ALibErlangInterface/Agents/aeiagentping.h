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

// Constants and definitions

// Namesapces
namespace ARB {

// Class definitions
class AEIAgentPing : public AThreadAgentTemplate {

	Q_OBJECT

	public:

		const char* pCookie = nullptr;
		const char* pNodeFullName = nullptr;
		AOutputQVariant pOutput;
		ALibErlangInterfaceService* pService = nullptr;

		explicit AEIAgentPing(QObject* parent = nullptr);
		virtual ~AEIAgentPing(void);

	public slots:

		void slRun(void);
};

} // namespace ARB

#endif // AEIAGENTPING_H

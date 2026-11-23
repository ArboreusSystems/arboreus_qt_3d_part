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

	emit sgFinished();
}


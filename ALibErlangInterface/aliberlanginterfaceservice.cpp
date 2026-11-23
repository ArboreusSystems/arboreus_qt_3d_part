// ----------------------------------------------------------
/*!
	\class ALibErlangInterfaceService
	\title
	\brief Template file files/cppheader/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 19:30:46
	\endlist
*/
// ----------------------------------------------------------

// Class header
#include "aliberlanginterfaceservice.h"

// Namesapce
using namespace ARB;


// -----------
/*!
	\fn

	Doc.
*/

ALibErlangInterfaceService::ALibErlangInterfaceService(QObject* parent) : AThreadServiceTemplate(parent) {

	_A_DEBUG << "ALibErlangInterfaceService created";
}


// -----------
/*!
	\fn

	Doc.
*/

ALibErlangInterfaceService::~ALibErlangInterfaceService(void) {

	_A_DEBUG << "ALibErlangInterfaceService deleted";
}


// -----------
/*!
	\fn

	Doc.
*/

void ALibErlangInterfaceService::slInit(void) {

	_A_DEBUG << "ALibErlangInterfaceService initiated";

	emit sgInitiated();
}


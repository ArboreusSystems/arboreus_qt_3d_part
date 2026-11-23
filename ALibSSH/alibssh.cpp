/* -------------------------------------------------------------------
 *  @doc
 *  @notice Template file tablemodel.h
 *
 *  @copyright Arboreus (http://arboreus.systems)
 *  @author Alexandr Kirilov (http://alexandr.kirilov.me)
 *  @created 23/11/2026 at 13:22:18
 * */// --------------------------------------------------------------

// Class header
#include "alibssh.h"

// Private class header
#include "alibssh_private.h"

// Namesapce
using namespace ARB;


// -----------
/*!
	\fn

	Doc.
*/

ALibSSH::ALibSSH(QObject* parent) : QObject(parent) {

	_A_DEBUG << "ALibSSH created";
}


// -----------
/*!
	\fn

	Doc.
*/

ALibSSH::~ALibSSH(void) {

	_A_DEBUG << "ALibSSH deleted";
}

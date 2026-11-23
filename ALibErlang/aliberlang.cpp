/* -------------------------------------------------------------------
 *  @doc
 *  @notice Template file tablemodel.h
 *
 *  @copyright Arboreus (http://arboreus.systems)
 *  @author Alexandr Kirilov (http://alexandr.kirilov.me)
 *  @created 23/11/2026 at 10:37:51
 * */// --------------------------------------------------------------

// Class header
#include "aliberlang.h"

// Private class header
#include "aliberlang_private.h"

// Namesapce
using namespace ARB;


// -----------
/*!
	\fn

	Doc.
*/

ALibErlang::ALibErlang(QObject* parent) : QObject(parent) {

	_A_DEBUG << "ALibErlang created";
}


// -----------
/*!
	\fn

	Doc.
*/

ALibErlang::~ALibErlang(void) {

	_A_DEBUG << "ALibErlang deleted";
}

/* -------------------------------------------------------------------
 *  @doc
 *  @notice Template file tablemodel.h
 *
 *  @copyright Arboreus (http://arboreus.systems)
 *  @author Alexandr Kirilov (http://alexandr.kirilov.me)
 *  @created 23/11/2026 at 13:07:28
 * */// --------------------------------------------------------------

// Class header
#include "alibzip.h"

// Private class header
#include "alibzip_private.h"

// Namespace
using namespace ARB;


// -----------
/*!
	\fn

	Doc.
*/

ALibZip::ALibZip(QObject* parent) : QObject(parent) {

	_A_DEBUG << "ALibZip created";
}


// -----------
/*!
	\fn

	Doc.
*/

ALibZip::~ALibZip(void) {

	_A_DEBUG << "ALibZip deleted";
}

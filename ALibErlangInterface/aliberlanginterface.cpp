/* -------------------------------------------------------------------
 *  @doc
 *  @notice Template file tablemodel.h
 *
 *  @copyright Arboreus (http://arboreus.systems)
 *  @author Alexandr Kirilov (http://alexandr.kirilov.me)
 *  @created 23/11/2026 at 19:04:36
 * */// --------------------------------------------------------------

// Class header
#include "aliberlanginterface.h"

// Namespace
using namespace ARB;


// -----------
/*!
	\fn

	Doc.
*/

ALibErlangInterface::ALibErlangInterface(QObject* parent)
: AThreadTemplate<ALibErlangInterfaceService>(new ALibErlangInterfaceService,parent) {

	this->setPriority(LowPriority);

	QObject::connect(
		this,&ALibErlangInterface::sgInit,
		this->mService(),&ALibErlangInterfaceService::slInit
	);
	QObject::connect(
		this->mService(),&ALibErlangInterfaceService::sgInitiated,
		this,&ALibErlangInterface::slInitiated
	);

	_A_DEBUG << "ALibErlangInterface created";
}


// -----------
/*!
	\fn

	Doc.
*/

ALibErlangInterface::~ALibErlangInterface(void) {

	_A_DEBUG << "ALibErlangInterface deleted";
}


// -----------
/*!
	\fn

	Doc.
*/

void ALibErlangInterface::mInit(void) {

	emit this->sgInit();
}


// -----------
/*!
	\fn

	Doc.
*/

void ALibErlangInterface::mNodePingHandler(const char* inNodeName, const char* inCookie,AOutputQVariant* inOutput) {

	AThreadObjectControllerTemplate oController;
	QEventLoop oEventLoop;

	AEIAgentPing oAgent;
	oAgent.pService = this->mService();
	oAgent.pCookie = inCookie;
	oAgent.pNodeFullName = inNodeName;
	QObject::connect(
		&oAgent,&AEIAgentPing::sgFinished,
		&oEventLoop,&QEventLoop::quit
	);
	QObject::connect(
		&oController,&AThreadObjectControllerTemplate::sgRun,
		&oAgent,&AEIAgentPing::slRun
	);
	oAgent.moveToThread(this);

	emit oController.sgRun();
	oEventLoop.exec();
}


// -----------
/*!
	\fn

	Doc.
*/

void ALibErlangInterface::slInitiated(void) {

	_A_DEBUG << "ALibErlangInterface initiated";

	emit this->sgInitiated();
}


// -----------
/*!
	\fn

	Doc.
*/

QVariantMap ALibErlangInterface::mNodePing(QString inNodeFullName, QString inCookie) {

	AOutputQVariant oOutput;

	mNodePingHandler(
		inNodeFullName.toUtf8().constData(),
		inCookie.toUtf8().constData(),
		&oOutput
	);

	return oOutput.mToVariantMap();
}


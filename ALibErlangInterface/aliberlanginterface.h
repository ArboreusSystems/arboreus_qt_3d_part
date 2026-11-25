// ----------------------------------------------------------
/*!
	\headerfile ALibErlangInterface
	\title
	\brief Template file wizard/classes/cpp/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 19:04:36
	\endlist
*/
// ----------------------------------------------------------

#ifndef ALIBERLANGINTERFACE_H
#define ALIBERLANGINTERFACE_H

// Precompiled includes
#include <aliberlanginterface_pch.h>

// System includes

// Application includes
#include <alogger.h>
#include <athreadtemplate.h>
#include <athreadobjectcontrollertemplate.h>
#include <aliberlanginterfaceservice.h>
#include <aeiagentping.h>

// Constants and definitions

// Namesapces
namespace ARB {

// Class definitions
class ALibErlangInterface : public AThreadTemplate<ALibErlangInterfaceService>  {

	Q_OBJECT

	public:

		explicit ALibErlangInterface(QObject* parent = nullptr);
		virtual ~ALibErlangInterface(void);

		void mInit(void);

		void mNodePingHandler(const QString& inNodeFullName,const QString& inCookie,AOutputQVariant* inOutput);

	public slots:

		void slInitiated(void);

		bool mNodePing(const QString& inNodeFullName,const QString& inCookie);

	signals:

		void sgInit(void);
		void sgInitiated(void);
};

} // namespace ARB

#endif // ALIBERLANGINTERFACE_H

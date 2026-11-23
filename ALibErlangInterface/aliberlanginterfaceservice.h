// ----------------------------------------------------------
/*!
	\headerfile ALibErlangInterfaceService
	\title
	\brief Template file wizard/classes/cpp/file.h

	\list
	\li @notice Template file classes/file.h
	\li @copyright Arboreus (http://arboreus.systems)
	\li @author Alexandr Kirilov (http://alexandr.kirilov.me)
	\li @created 23/11/2026 at 19:30:46
	\endlist
*/
// ----------------------------------------------------------
#ifndef ALIBERLANGINTERFACESERVICE_H
#define ALIBERLANGINTERFACESERVICE_H

// Precompiled includes
#include <aliberlanginterface_pch.h>

// System includes

// Application includes
#include <alogger.h>
#include <athreadservicetemplate.h>

// Constants and definitions

// Namesapces
namespace ARB {

// Class definitions
class ALibErlangInterfaceService : public AThreadServiceTemplate {

	Q_OBJECT

	public:

		explicit ALibErlangInterfaceService(QObject* parent = nullptr);
		virtual ~ALibErlangInterfaceService(void);

	public slots:

		void slInit(void);

	signals:

		void sgInit(void);
		void sgInitiated(void);

	private:

};

} // namespace ARB

#endif // ALIBERLANGINTERFACESERVICE_H

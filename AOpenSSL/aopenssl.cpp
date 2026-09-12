/* -------------------------------------------------------------------
 *  @doc
 *  @notice Template file tablemodel.h
 *
 *  @copyright Arboreus (http://arboreus.systems)
 *  @author Alexandr Kirilov (http://alexandr.kirilov.me)
 *  @created 23/09/2025 at 11:09:16
 * */// --------------------------------------------------------------

// Class header
#include <aopenssl.h>

// Namespace
using namespace ARB;


// -----------
/*!
	\fn

	Doc.
*/

AOpenSSL::AOpenSSL(QObject* parent) : QObject(parent) {

	_A_DEBUG << "AOpenSSL created";
}


// -----------
/*!
	\fn

	Doc.
*/

AOpenSSL::~AOpenSSL(void) {

	_A_DEBUG << "AOpenSSL deleted";
}


// -----------
/*!
	\fn

	Doc.
*/

AOutputQByteArray AOpenSSL::mEncodeAES256CBC(
	const QByteArray& inBytes, const QByteArray& inKey, const QByteArray& inVector
) {

	AOutputQByteArray oOutput;
	oOutput.pStatus = _A_ENUM_STATUS::Error;

	EVP_CIPHER_CTX* oContext = EVP_CIPHER_CTX_new();
	if (!oContext) {
		_A_DEBUG << "Context not created";
		oOutput.pCode = -1;
		return oOutput;
	}

	int oLengthEncryptedBytes = (int)inBytes.size() + EVP_MAX_BLOCK_LENGTH;
	int oLength;
	int oLengthTotal = 0;

	QByteArray oEncryptedBytes(oLengthEncryptedBytes, 0);

	int oEncryptInit = EVP_EncryptInit_ex(
		oContext,EVP_aes_256_cbc(),NULL,
		(const unsigned char*)inKey.constData(),
		(const unsigned char*)inVector.constData()
	);
	if (1 != oEncryptInit) {
		EVP_CIPHER_CTX_free(oContext);
		_A_DEBUG << "EVP_EncryptInit_ex failed";
		oOutput.pCode = -2;
		return oOutput;
	}

	int oEncryptUpdate = EVP_EncryptUpdate(
		oContext,(unsigned char*)oEncryptedBytes.data(),
		&oLength,(const unsigned char*)inBytes.constData(),(int)inBytes.size()
	);
	if (1 != oEncryptUpdate) {
		EVP_CIPHER_CTX_free(oContext);
		_A_DEBUG << "EVP_EncryptUpdate failed";
		oOutput.pCode = -3;
		return oOutput;
	}
	oLengthTotal = oLength;

	int EncryptFinal = EVP_EncryptFinal_ex(
		oContext,(unsigned char*)oEncryptedBytes.data() + oLength,&oLength
	);
	if (1 != EncryptFinal) {
		EVP_CIPHER_CTX_free(oContext);
		_A_DEBUG << "EVP_EncryptFinal_ex failed";
		oOutput.pCode = -4;
		return oOutput;
	}
	oLengthTotal += oLength;

	EVP_CIPHER_CTX_free(oContext);
	oEncryptedBytes.resize(oLengthTotal);

	_A_DEBUG << "String encoded";

	oOutput.pStatus = _A_ENUM_STATUS::Ok;
	oOutput.pValue = oEncryptedBytes;
	return oOutput;
}


// -----------
/*!
	\fn

	Doc.
*/

AOutputQByteArray AOpenSSL::mDecodeAES256CBC(
	const QByteArray& inBytes, const QByteArray& inKey, const QByteArray& inVector
) {

	AOutputQByteArray oOutput;
	oOutput.pStatus = _A_ENUM_STATUS::Error;

	EVP_CIPHER_CTX* oContext = EVP_CIPHER_CTX_new();
	if (!oContext) {
		_A_DEBUG << "Context not created";
		oOutput.pCode = -1;
		return oOutput;
	}

	int oLength;
	int oLengthTotal = 0;

	QByteArray oDecodedBytes(inBytes.size() + EVP_MAX_BLOCK_LENGTH,0);

	int oDecodeInit = EVP_DecryptInit_ex(
		oContext,EVP_aes_256_cbc(),NULL,
		(const unsigned char*)inKey.constData(),
		(const unsigned char*)inVector.constData()
	);
	if (1 != oDecodeInit) {
		EVP_CIPHER_CTX_free(oContext);
		_A_DEBUG << "EVP_DecryptInit_ex failed";
		oOutput.pCode = -2;
		return oOutput;
	}

	int oDecodeUpdate = EVP_DecryptUpdate(
		oContext,(unsigned char*)oDecodedBytes.data(),
		&oLength,(const unsigned char*)inBytes.constData(),
		(int)inBytes.size()
	);
	if (1 != oDecodeUpdate) {
		EVP_CIPHER_CTX_free(oContext);
		_A_DEBUG << "EVP_DecryptUpdate failed";
		oOutput.pCode = -3;
		return oOutput;
	}
	oLengthTotal = oLength;

	int DecodeFinal = EVP_DecryptFinal_ex(
		oContext,(unsigned char*)oDecodedBytes.data() + oLength,&oLength
	);
	if (1 != DecodeFinal) {
		EVP_CIPHER_CTX_free(oContext);
		_A_DEBUG << "EVP_DecryptFinal_ex failed";
		oOutput.pCode = -4;
		return oOutput;
	}
	oLengthTotal += oLength;

	EVP_CIPHER_CTX_free(oContext);
	oDecodedBytes.resize(oLengthTotal);

	_A_DEBUG << "String decoded";

	oOutput.pStatus = _A_ENUM_STATUS::Ok;
	oOutput.pValue = oDecodedBytes;
	return oOutput;
}


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

	if (inBytes.isEmpty() || inKey.size() != 32 || inVector.size() != 16) {
		_A_WARNING << "Invalid input parameter alignments passed to encryption engine.";
		oOutput.pCode = -100;
		return oOutput;
	}

	EVP_CIPHER_CTX* oContext = EVP_CIPHER_CTX_new();
	if (!oContext) {
		_A_WARNING << "Context not created";
		oOutput.pCode = -1;
		return oOutput;
	}

	mlock(const_cast<char*>(inKey.constData()), inKey.size());
	mlock(const_cast<char*>(inVector.constData()), inVector.size());

	const int oLengthEncryptedBytes = (int)inBytes.size() + 16;
	int oLength = 0;
	int oLengthTotal = 0;

	QByteArray oEncryptedBytes;
	oEncryptedBytes.resize(oLengthEncryptedBytes);

	int oEncryptInit = EVP_EncryptInit_ex(
		oContext, EVP_aes_256_cbc(), nullptr,
		reinterpret_cast<const unsigned char*>(inKey.constData()),
		reinterpret_cast<const unsigned char*>(inVector.constData())
	);
	if (1 != oEncryptInit) {
		EVP_CIPHER_CTX_free(oContext);
		_A_WARNING << "EVP_EncryptInit_ex failed";
		oOutput.pCode = -2;
		return oOutput;
	}

	int oEncryptUpdate = EVP_EncryptUpdate(
		oContext, reinterpret_cast<unsigned char*>(oEncryptedBytes.data()),
		&oLength, reinterpret_cast<const unsigned char*>(inBytes.constData()),(int)inBytes.size()
	);
	if (1 != oEncryptUpdate) {
		EVP_CIPHER_CTX_free(oContext);
		_A_WARNING << "EVP_EncryptUpdate failed";
		oOutput.pCode = -3;
		return oOutput;
	}
	oLengthTotal = oLength;

	int EncryptFinal = EVP_EncryptFinal_ex(
		oContext, reinterpret_cast<unsigned char*>(oEncryptedBytes.data()) + oLengthTotal,&oLength
	);
	if (1 != EncryptFinal) {
		EVP_CIPHER_CTX_free(oContext);
		_A_WARNING << "EVP_EncryptFinal_ex failed";
		oOutput.pCode = -4;
		return oOutput;
	}
	oLengthTotal += oLength;

	EVP_CIPHER_CTX_free(oContext);

	QByteArray oFinalSecureResult;
	oFinalSecureResult.resize(oLengthTotal);
	std::memcpy(oFinalSecureResult.data(),oEncryptedBytes.constData(),oLengthTotal);
	oFinalSecureResult.detach();

	OPENSSL_cleanse(oEncryptedBytes.data(),oEncryptedBytes.size());
	oEncryptedBytes.clear();
	oEncryptedBytes.squeeze();

	std::atomic_thread_fence(std::memory_order_release);
	munlock(const_cast<char*>(inKey.constData()),inKey.size());
	munlock(const_cast<char*>(inVector.constData()),inVector.size());

	_A_DEBUG << "String encoded securely.";

	oOutput.pStatus = _A_ENUM_STATUS::Ok;
	oOutput.pValue = oFinalSecureResult;
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

	if (inBytes.isEmpty() || inKey.size() != 32 || inVector.size() != 16) {
		_A_WARNING << "Invalid input buffer structure constraints passed to decryption engine.";
		oOutput.pCode = -100;
		return oOutput;
	}

	EVP_CIPHER_CTX* oContext = EVP_CIPHER_CTX_new();
	if (!oContext) {
		_A_WARNING << "Context not created";
		oOutput.pCode = -1;
		return oOutput;
	}

	mlock(const_cast<char*>(inKey.constData()),inKey.size());
	mlock(const_cast<char*>(inVector.constData()),inVector.size());

	int oLength = 0;
	int oLengthTotal = 0;

	const int oMaxDecodedLength = inBytes.size() + EVP_MAX_BLOCK_LENGTH;
	QByteArray oDecodedBytes;
	oDecodedBytes.resize(oMaxDecodedLength);
	mlock(oDecodedBytes.data(),oDecodedBytes.size());

	int oDecodeInit = EVP_DecryptInit_ex(
		oContext, EVP_aes_256_cbc(), nullptr,
		reinterpret_cast<const unsigned char*>(inKey.constData()),
		reinterpret_cast<const unsigned char*>(inVector.constData())
	);
	if (1 != oDecodeInit) {
		EVP_CIPHER_CTX_free(oContext);
		munlock(oDecodedBytes.data(),oDecodedBytes.size());
		munlock(const_cast<char*>(inKey.constData()),inKey.size());
		munlock(const_cast<char*>(inVector.constData()),inVector.size());
		_A_WARNING << "EVP_DecryptInit_ex failed";
		oOutput.pCode = -2;
		return oOutput;
	}

	int oDecodeUpdate = EVP_DecryptUpdate(
		oContext, reinterpret_cast<unsigned char*>(oDecodedBytes.data()),
		&oLength, reinterpret_cast<const unsigned char*>(inBytes.constData()),(int)inBytes.size()
	);
	if (1 != oDecodeUpdate) {
		EVP_CIPHER_CTX_free(oContext);
		munlock(oDecodedBytes.data(),oDecodedBytes.size());
		munlock(const_cast<char*>(inKey.constData()),inKey.size());
		munlock(const_cast<char*>(inVector.constData()),inVector.size());
		_A_WARNING << "EVP_DecryptUpdate failed";
		oOutput.pCode = -3;
		return oOutput;
	}
	oLengthTotal = oLength;

	int DecodeFinal = EVP_DecryptFinal_ex(
		oContext, reinterpret_cast<unsigned char*>(oDecodedBytes.data()) + oLengthTotal,&oLength
	);
	if (1 != DecodeFinal) {
		OPENSSL_cleanse(oDecodedBytes.data(),oDecodedBytes.size());
		EVP_CIPHER_CTX_free(oContext);
		munlock(oDecodedBytes.data(),oDecodedBytes.size());
		munlock(const_cast<char*>(inKey.constData()),inKey.size());
		munlock(const_cast<char*>(inVector.constData()),inVector.size());
		_A_WARNING << "EVP_DecryptFinal_ex failed (Invalid Key, Password, or Corrupted Data)";
		oOutput.pCode = -4;
		return oOutput;
	}
	oLengthTotal += oLength;

	EVP_CIPHER_CTX_free(oContext);

	QByteArray oFinalSecureResult;
	oFinalSecureResult.resize(oLengthTotal);
	std::memcpy(oFinalSecureResult.data(),oDecodedBytes.constData(),oLengthTotal);
	oFinalSecureResult.detach();

	OPENSSL_cleanse(oDecodedBytes.data(),oDecodedBytes.size());
	oDecodedBytes.clear();
	oDecodedBytes.squeeze();

	std::atomic_thread_fence(std::memory_order_release);
	munlock(oDecodedBytes.data(),oMaxDecodedLength);
	munlock(const_cast<char*>(inKey.constData()),inKey.size());
	munlock(const_cast<char*>(inVector.constData()),inVector.size());

	_A_DEBUG << "String decoded securely.";

	oOutput.pStatus = _A_ENUM_STATUS::Ok;
	oOutput.pValue = oFinalSecureResult;
	return oOutput;
}


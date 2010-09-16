#ifndef HTTPSERVER_P_H
#define HTTPSERVER_P_H

#include "KDSoapGlobal.h"
#include <QBuffer>
#include <QThread>
#include <QSemaphore>
#include <QTcpServer>
#include <QTcpSocket>
#ifndef QT_NO_OPENSSL
#include <QSslSocket>
#endif
#include <QUrl>
#include <QStringList>
#include <QSslCertificate>
#include <QSslKey>
namespace KDSoapUnitTestHelpers
{
    KDSOAP_EXPORT bool xmlBufferCompare(const QByteArray& source, const QByteArray& dest);
    KDSOAP_EXPORT void httpGet(const QUrl& url);
}

class KDSOAP_EXPORT HttpServerThread : public QThread
{
    Q_OBJECT
public:
    enum Feature {
        Public = 0,    // HTTP with no ssl and no authentication needed
        Ssl = 1,       // HTTPS
        BasicAuth = 2,  // Requires authentication
        Error404 = 4   // Return "404 not found"
        // bitfield, next item is 8
    };
    Q_DECLARE_FLAGS(Features, Feature)

    HttpServerThread(const QByteArray& dataToSend, Features features)
        : m_dataToSend(dataToSend), m_features(features)
    {
        start();
        m_ready.acquire();

    }
    ~HttpServerThread()
    {
        finish();
        wait();
    }

    inline int serverPort() const { return m_port; }
    QString endPoint() const {
        return QString::fromLatin1("%1://127.0.0.1:%2/path")
                           .arg(QString::fromLatin1((m_features & Ssl)?"https":"http"))
                           .arg(serverPort());
    }

    inline void finish() {
        KDSoapUnitTestHelpers::httpGet(endPoint() + QLatin1String("/terminateThread"));
    }

    QByteArray receivedData() const { return m_receivedData; }
    QByteArray receivedHeaders() const { return m_receivedHeaders; }
    void resetReceivedBuffers() {
        m_receivedData.clear();
        m_receivedHeaders.clear();
    }

    QByteArray header(const QByteArray& value) const {
        return m_headers.value(value);
    }
protected:
    /* \reimp */ void run();

private:

    enum Method { None, Basic, Plain, Login, Ntlm, CramMd5, DigestMd5 };
    static void parseAuthLine(const QString& str, Method* method, QString* headerVal)
    {
        *method = None;
        // The code below (from QAuthenticatorPrivate::parseHttpResponse)
        // is supposed to be run in a loop, apparently
        // (multiple WWW-Authenticate lines? multiple values in the line?)

        //qDebug() << "parseAuthLine() " << str;
        if (*method < Basic && str.startsWith(QLatin1String("Basic"), Qt::CaseInsensitive)) {
            *method = Basic;
            *headerVal = str.mid(6);
        } else if (*method < Ntlm && str.startsWith(QLatin1String("NTLM"), Qt::CaseInsensitive)) {
            *method = Ntlm;
            *headerVal = str.mid(5);
        } else if (*method < DigestMd5 && str.startsWith(QLatin1String("Digest"), Qt::CaseInsensitive)) {
            *method = DigestMd5;
            *headerVal = str.mid(7);
        }
    }

    QByteArray makeHttpResponse(const QByteArray& responseData)
    {
        QByteArray httpResponse;
        if (m_features & Error404)
            httpResponse += "HTTP/1.1 404 Not Found\r\n";
        else
            httpResponse += "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: text/xml\r\nContent-Length: ";
        httpResponse += QByteArray::number(responseData.size());
        httpResponse += "\r\n";

        // We don't support multiple connexions (TODO) so let's ask the client
        // to close the connection every time. See testCallNoReply which performs
        // multiple connexions at the same time (QNAM keeps the old connexion open).
        httpResponse += "Connection: close\r\n";
        httpResponse += "\r\n";
        httpResponse += responseData;
        return httpResponse;
    }

private:
    QSemaphore m_ready;
    QByteArray m_dataToSend;
    QByteArray m_receivedData;
    QByteArray m_receivedHeaders;
    QMap<QByteArray, QByteArray> m_headers;
    int m_port;
    Features m_features;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(HttpServerThread::Features)

#endif /* HTTPSERVER_P_H */


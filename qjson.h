//From http://codereview.stackexchange.com/questions/11927/yet-another-json-parser-and-serializer-for-qt-but-with-additional-features
#ifndef QJSON_H
#define QJSON_H

#include <QString>
#include <QVariant>

class QJson
{
	Q_FLAGS(EncodeOption EncodeOptions)
		Q_FLAGS(DecodeOption DecodeOptions)

public:
	enum EncodeOption
	{
		EncodeUnknownTypesAsNull = 0x01,
		Compact = 0x02
	};
	Q_DECLARE_FLAGS(EncodeOptions, EncodeOption)

	enum DecodeOption
	{
		DecodeObjectsAsHash = 0x01,
		AllowUnquotedStrings = 0x02,
		AllowMissingComma = 0x04,
		AllowLazyJSON = AllowUnquotedStrings | AllowMissingComma
	};
	Q_DECLARE_FLAGS(DecodeOptions, DecodeOption)


		static QString encode(const QVariant &data, QString *errorMessage = 0, int indentation = 4);
	static QString encode(const QVariant &data, EncodeOptions options, QString *errorMessage = 0, int indentation = 4);

	static QVariant decode(const QString &json, QString *errorMessage = 0);
	static QVariant decode(const QString &json, DecodeOptions options, QString *errorMessage = 0);

private:
	QJson();

	static QString encodeData(const QVariant &data, EncodeOptions options, QString *errorMessage, int indentation, QString currentLinePrefix);
	static QString encodeString(QString data);
	static QString encodeByteArray(QByteArray data);

	static QVariant parseValue(const QString &json, int &index, DecodeOptions options, bool &success, QString *errorMessage);
	template<typename ContainerType>
	static QVariant parseObject(const QString &json, int &index, DecodeOptions options, bool &success, QString *errorMessage);
	static QVariant parseArray(const QString &json, int &index, DecodeOptions options, bool &success, QString *errorMessage);
	static QVariant parseString(const QString &json, int &index, bool &success, QString *errorMessage);
	static QVariant parseUnquotedString(const QString &json, int &index, bool &success, QString *errorMessage);
	static QVariant parseNumber(const QString &json, int &index, bool &success, QString *errorMessage);
	static QVariant parseBool(const QString &json, int &index, bool &success, QString *errorMessage);
	static QVariant parseNull(const QString &json, int &index, bool &success, QString *errorMessage);
	static int skipWhitespace(const QString &json, int &index);
	static bool checkAvailable(const QString &json, int &index, bool &success, QString *errorMessage, int minAvailable = 1);
	static bool checkToken(const QString &json, int &index, bool &success, QString *errorMessage, QString token);
};

#endif // QJSON_H
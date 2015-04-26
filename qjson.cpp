//From http://codereview.stackexchange.com/questions/11927/yet-another-json-parser-and-serializer-for-qt-but-with-additional-features
#include "qjson.h"
#include <QStringList>

QJson::QJson()
{
}

/* ----------------------------------------------------------------------------------------------------------------- */
// ENCODER / SERIALIZER
/* ----------------------------------------------------------------------------------------------------------------- */

QString QJson::encode(const QVariant &data, QString *errorMessage, int indentation)
{
	return encodeData(data, EncodeOptions(), errorMessage, indentation, QString());
}

QString QJson::encode(const QVariant &data, EncodeOptions options, QString *errorMessage, int indentation)
{
	return encodeData(data, options, errorMessage, indentation, QString());
}

QString QJson::encodeData(const QVariant &data, EncodeOptions options, QString *errorMessage,
	int indentation, QString currentLinePrefix)
{
	QString indentedLinePrefix = options.testFlag(Compact) ?
		QString::fromLatin1("") :
		(currentLinePrefix + QString::fromLatin1(" ").repeated(indentation));

	QString optionalNewLine = options.testFlag(Compact) ?
		QString::fromLatin1("") :
		(QString::fromLatin1("\n") + currentLinePrefix);

	QString optionalIndentedNewLine = options.testFlag(Compact) ?
		QString::fromLatin1("") :
		(QString::fromLatin1("\n") + indentedLinePrefix);

	QString encoded;

	switch (data.type())
	{
	case(QVariant::Bool) :
		encoded += QString::fromLatin1(data.toBool() ? "true" : "false");
		break;

	case(QVariant::Int) :
	case(QVariant::UInt) :
	case(QVariant::LongLong) :
	case(QVariant::ULongLong) :
							  Q_ASSERT(data.canConvert(QVariant::String));
		encoded = data.toString();
		break;

	case(QVariant::Double) :
		encoded = QString::number(data.toDouble(), 'g', 16);
		if (!encoded.contains(QString::fromLatin1(".")) && !encoded.contains(QString::fromLatin1("e")))
			encoded += ".0";
		break;

	case(QVariant::String) :
		encoded = encodeString(data.toString());
		break;

	case(QVariant::ByteArray) :
		encoded = encodeByteArray(data.toByteArray());
		break;

	case(QVariant::List) :
	{
		encoded = QString::fromLatin1("[") + optionalIndentedNewLine;
		QVariantList list = data.toList();
		for (int i = 0; i < list.count(); ++i)
		{
			if (i) encoded += QString::fromLatin1(",") + optionalIndentedNewLine;
			encoded += encodeData(list.at(i), options, errorMessage, indentation, indentedLinePrefix);
			if (errorMessage && !errorMessage->isNull())
				return QString();
		}
		encoded += optionalNewLine + QString::fromLatin1("]");
	}
						 break;

	case(QVariant::StringList) :
	{
		encoded = QString::fromLatin1("[") + optionalIndentedNewLine;
		QStringList list = data.toStringList();
		for (int i = 0; i < list.count(); ++i)
		{
			if (i) encoded += QString::fromLatin1(",") + optionalIndentedNewLine;
			encoded += encodeData(list.at(i), options, errorMessage, indentation, indentedLinePrefix);
			if (errorMessage && !errorMessage->isNull())
				return QString();
		}
		encoded += optionalNewLine + QString::fromLatin1("]");
	}
							   break;

	case(QVariant::Map) :
	{
		encoded = QString::fromLatin1("{") + optionalIndentedNewLine;
		QVariantMap map = data.toMap();
		QVariantMap::iterator i;
		bool first = true;
		for (i = map.begin(); i != map.end(); ++i)
		{
			if (!first)
				encoded += QString::fromLatin1(",") + optionalIndentedNewLine;
			first = false;
			encoded += encodeString(i.key());
			encoded += options.testFlag(Compact) ? QString::fromLatin1(":") : QString::fromLatin1(" : ");
			encoded += encodeData(i.value(), options, errorMessage, indentation, indentedLinePrefix);
			if (errorMessage && !errorMessage->isNull())
				return QString();
		}
		encoded += optionalNewLine + QString::fromLatin1("}");
	}
						break;

	case(QVariant::Hash) :
	{
		encoded = QString::fromLatin1("{") + optionalIndentedNewLine;
		QVariantHash hash = data.toHash();
		QVariantHash::iterator i;
		bool first = true;
		for (i = hash.begin(); i != hash.end(); ++i)
		{
			if (!first)
				encoded += QString::fromLatin1(",") + optionalIndentedNewLine;
			first = false;
			encoded += encodeString(i.key());
			encoded += options.testFlag(Compact) ? QString::fromLatin1(":") : QString::fromLatin1(" : ");
			encoded += encodeData(i.value(), options, errorMessage, indentation, indentedLinePrefix);
			if (errorMessage && !errorMessage->isNull())
				return QString();
		}
		encoded += optionalNewLine + QString::fromLatin1("}");
	}
						 break;

	case(QVariant::Invalid) :
		encoded = QString::fromLatin1("null");
		break;

	default:
		if (!options.testFlag(EncodeUnknownTypesAsNull))
		{
			if (errorMessage)
				*errorMessage = QString::fromLatin1("Can't encode this type of data to JSON: %1")
				.arg(data.typeName());
			return QString();
		}
		encoded = QString::fromLatin1("null");
		break;
	}

	return encoded;
}

QString QJson::encodeString(QString data)
{
	QString encoded;
	encoded.append(QChar::fromLatin1('"'));
	for (int i = 0; i < data.length(); ++i)
	{
		QChar ch = data.at(i);

		// printable ASCII character?
		if (ch.unicode() >= 32 && ch.unicode() < 128)
			encoded.append(ch);
		else
		{
			switch (ch.unicode())
			{
			case(8) :
				encoded.append(QString::fromLatin1("\\b"));
				break;
			case(9) :
				encoded.append(QString::fromLatin1("\\t"));
				break;
			case(10) :
				encoded.append(QString::fromLatin1("\\n"));
				break;
			case(12) :
				encoded.append(QString::fromLatin1("\\f"));
				break;
			case(13) :
				encoded.append(QString::fromLatin1("\\r"));
				break;
			case('"') :
				encoded.append(QString::fromLatin1("\\\""));
				break;
			case('\\') :
				encoded.append(QString::fromLatin1("\\\\"));
				break;
			case('/') :
				encoded.append(QString::fromLatin1("\\/"));
				break;
			default:
				encoded.append(QString::fromLatin1("\\u") + QString::number(ch.unicode(), 16)
					.rightJustified(4, QChar::fromLatin1('0')));
			}
		}
	}
	encoded.append(QChar::fromLatin1('"'));
	return encoded;
}

QString QJson::encodeByteArray(QByteArray data)
{
	return encodeString(QString::fromLocal8Bit(data));
}


/* ----------------------------------------------------------------------------------------------------------------- */
// DECODER / PARSER
/* ----------------------------------------------------------------------------------------------------------------- */

QVariant QJson::decode(const QString &json, QString *errorMessage)
{
	return decode(json, DecodeOptions(), errorMessage);
}

QVariant QJson::decode(const QString &json, DecodeOptions options, QString *errorMessage)
{
	//there are currently no options defined
	Q_UNUSED(options);

	bool success = true;
	if (!json.isNull() && !json.isEmpty()) {
		int index = 0;
		return parseValue(json, index, options, success, errorMessage);
	}
	else
		// To simplify things, this is not treated as an error but as valid input.
		return QVariant();
}

QVariant QJson::parseValue(const QString &json, int &index, DecodeOptions options, bool &success, QString *errorMessage)
{
	skipWhitespace(json, index);
	if (!checkAvailable(json, index, success, errorMessage))
	{
		success = false;
		if (errorMessage)
			*errorMessage = QString("Expected more input at position %1").arg(index);
		return QVariant();
	}

	switch (json[index].toLatin1())
	{
	case '"':
		return QJson::parseString(json, index, success, errorMessage);

	case '{':
		if (options & DecodeObjectsAsHash)
			return QJson::parseObject<QVariantHash>(json, index, options, success, errorMessage);
		else
			return QJson::parseObject<QVariantMap>(json, index, options, success, errorMessage);

	case '[':
		return QJson::parseArray(json, index, options, success, errorMessage);

	case 't':
	case 'f':
		if (options & AllowUnquotedStrings)
			return QJson::parseUnquotedString(json, index, success, errorMessage);
		else
			return QJson::parseBool(json, index, success, errorMessage);

	case 'n':
		if (options & AllowUnquotedStrings)
			return QJson::parseUnquotedString(json, index, success, errorMessage);
		else
			return QJson::parseNull(json, index, success, errorMessage);

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '-':
		return QJson::parseNumber(json, index, success, errorMessage);

	default:
		if (options & AllowUnquotedStrings)
			return QJson::parseUnquotedString(json, index, success, errorMessage);
		else
		{
			success = false;
			if (errorMessage)
				*errorMessage = QString("Unexpected character at position %1").arg(index);
			return QVariant();
		}
	}
}

template<typename ContainerType>
QVariant QJson::parseObject(const QString &json, int &index, DecodeOptions options, bool &success, QString *errorMessage)
{
	Q_ASSERT(json[index] == '{');
	index++;
	skipWhitespace(json, index);

	ContainerType object;

	while (checkAvailable(json, index, success, errorMessage))
	{
		if (json[index] == QChar::fromLatin1('}'))
		{
			index++;
			return object;
		}
		else
		{
			QString key = QJson::parseValue(json, index, options, success, errorMessage).toString();
			if (!success)
				return QVariant();

			skipWhitespace(json, index);
			checkAvailable(json, index, success, errorMessage);

			if (json[index] == QChar::fromLatin1(':'))
				index++;
			else
			{
				success = false;
				if (errorMessage)
					*errorMessage = QString("Expected colon at position %1").arg(index);
				return QVariant();
			}

			skipWhitespace(json, index);

			QVariant value = QJson::parseValue(json, index, options, success, errorMessage);
			if (!success)
				return QVariant();

			// Add the key / value pair to the object
			object.insert(key, value);

			int skippedWhitespaces = skipWhitespace(json, index);
			checkAvailable(json, index, success, errorMessage);

			switch (json[index].toLatin1())
			{
			case ',':
				index++;
				skipWhitespace(json, index);
				break;

			case '}':
				//'}' will be processed in the next iteration
				break;

			default:
				// Only allow missing comma if there is at least one whitespace instead of the comma!
				if ((options & AllowMissingComma) && skippedWhitespaces > 0)
					break;
				else
				{
					success = false;
					if (errorMessage)
						*errorMessage = QString("Unexpected character at index %1").arg(index);
					return QVariant();
				}
			}
		}
	}

	return QVariant();
}

QVariant QJson::parseArray(const QString &json, int &index, DecodeOptions options, bool &success, QString *errorMessage)
{
	Q_ASSERT(json[index] == '[');
	index++;
	skipWhitespace(json, index);

	QVariantList array;

	while (checkAvailable(json, index, success, errorMessage))
	{
		if (json[index] == QChar::fromLatin1(']'))
		{
			index++;
			return array;
		}
		else
		{
			QVariant value = QJson::parseValue(json, index, options, success, errorMessage);
			if (!success)
				return QVariant();

			// Add the value pair to the array
			array.append(value);

			int skippedWhitespaces = skipWhitespace(json, index);
			checkAvailable(json, index, success, errorMessage);

			switch (json[index].toLatin1())
			{
			case ',':
				index++;
				skipWhitespace(json, index);
				break;

			case ']':
				//']' will be processed in the next iteration
				break;

			default:
				// Only allow missing comma if there is at least one whitespace instead of the comma!
				if ((options & AllowMissingComma) && skippedWhitespaces > 0)
					break;
				else
				{
					success = false;
					if (errorMessage)
						*errorMessage = QString("Unexpected character at index %1").arg(index);
					return QVariant();
				}
			}
		}
	}

	return QVariant();
}

QVariant QJson::parseString(const QString &json, int &index, bool &success, QString *errorMessage)
{
	Q_ASSERT(json[index] == '"');
	index++;

	QString string;
	QChar ch;

	while (checkAvailable(json, index, success, errorMessage))
	{
		ch = json[index++];

		switch (ch.toLatin1())
		{
		case '\\':
			// Escaped character
			if (!checkAvailable(json, index, success, errorMessage))
				return QVariant();
			ch = json[index++];
			switch (ch.toLatin1())
			{
			case 'b':
				string.append('\b');
				break;
			case 'f':
				string.append('\f');
				break;
			case 'n':
				string.append('\n');
				break;
			case 'r':
				string.append('\r');
				break;
			case 't':
				string.append('\t');
				break;
			case 'u':
				if (!checkAvailable(json, index, success, errorMessage, 4))
					return QVariant();
				string.append(QChar(json.mid(index, 4).toInt(0, 16)));
				index += 4;
				break;
			default:
				string.append(ch);
			}
			break;

		case '"':
			// End of string
			return QVariant(string);

		default:
			string.append(ch);
		}
	}

	return QVariant();
}

QVariant QJson::parseUnquotedString(const QString &json, int &index, bool &success, QString *errorMessage)
{
	QString string;
	QChar ch;

	bool end = false;
	while (!end && checkAvailable(json, index, success, errorMessage))
	{
		ch = json[index++];

		switch (ch.toLatin1())
		{
		case '\\':
			// Escaped character
			if (!checkAvailable(json, index, success, errorMessage))
				return QVariant();
			ch = json[index++];
			switch (ch.toLatin1())
			{
			case 'b':
				string.append('\b');
				break;
			case 'f':
				string.append('\f');
				break;
			case 'n':
				string.append('\n');
				break;
			case 'r':
				string.append('\r');
				break;
			case 't':
				string.append('\t');
				break;
			case 'u':
				if (!checkAvailable(json, index, success, errorMessage, 4))
					return QVariant();
				string.append(QChar(json.mid(index, 4).toInt(0, 16)));
				index += 4;
				break;
			default:
				string.append(ch);
			}
			break;

		case ':':
		case ',':
		case ']':
		case '}':
		case '\n':
			// End of string (was one character before this!)
			end = true;
			index--;
			break;

		default:
			string.append(ch);
		}
	}

	//trim string
	string = string.trimmed();

	//handle keywords
	if (string == "true")
		return QVariant(true);
	if (string == "false")
		return QVariant(false);
	if (string == "null")
		return QVariant();

	return QVariant(string);
}

QVariant QJson::parseNumber(const QString &json, int &index, bool &success, QString *errorMessage)
{
	int end = index;
	bool endFound = false;
	while (!endFound && end < json.length())
	{
		switch (json[end].toLatin1())
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
		case 'e':
		case 'E':
		case '+':
		case '-':
			end++;
			break;

		default:
			endFound = true;
			break;
		}
	}

	QString numberStr = json.mid(index, end - index);
	index = end;

	QVariant result;
	bool ok;

	// is floating point number?
	if (numberStr.contains(QChar::fromLatin1('.')) ||
		numberStr.contains(QChar::fromLatin1('e')) ||
		numberStr.contains(QChar::fromLatin1('E')))
		result = QVariant(numberStr.toDouble(&ok));
	else
		result = QVariant(numberStr.toLongLong(&ok));

	if (ok)
		return result;
	else {
		success = false;
		if (errorMessage)
			*errorMessage = QString("Can't parse number at position %1: %2").arg(index).arg(numberStr);
		return QVariant();
	}
}

int QJson::skipWhitespace(const QString &json, int &index)
{
	int skipped = 0;
	while (index < json.size())
	{
		switch (json[index].toLatin1())
		{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			index++;
			skipped++;
			break;
		default:
			return skipped;
		}
	}
	return skipped;
}

QVariant QJson::parseBool(const QString &json, int &index, bool &success, QString *errorMessage)
{
	if (checkToken(json, index, success, errorMessage, "true"))
	{
		index += 4;
		return QVariant(true);
	}
	else if (checkToken(json, index, success, errorMessage, "false"))
	{
		index += 5;
		return QVariant(false);
	}
	else
	{
		success = false;
		if (errorMessage)
			*errorMessage = QString("Can't parse value at position %1").arg(index);
		return QVariant();
	}
}

QVariant QJson::parseNull(const QString &json, int &index, bool &success, QString *errorMessage)
{
	if (checkToken(json, index, success, errorMessage, "null"))
	{
		index += 4;
		return QVariant(true);
	}
	else
	{
		success = false;
		if (errorMessage)
			*errorMessage = QString("Can't parse value at position %1").arg(index);
		return QVariant();
	}
}

bool QJson::checkAvailable(const QString &json, int &index, bool &success, QString *errorMessage, int minAvailable)
{
	if (index + minAvailable > json.length())
	{
		success = false;
		if (errorMessage)
			*errorMessage = QString("Unexpected end of JSON input.");
		return false;
	}
	else
		return true;
}

bool QJson::checkToken(const QString &json, int &index, bool &success, QString *errorMessage, QString token)
{
	return checkAvailable(json, index, success, errorMessage, token.length())
		&& json.mid(index, token.length()) == token;
}
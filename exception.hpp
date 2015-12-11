#pragma once

namespace libblock {

class Error {};

// parser
class ErrorParserFailed: public Error {};
class ErrorParserInternalError: public Error {};

// access directions
class ErrorDiscardNotAllowed: public Error {};
class ErrorReadNotAllowed: public Error {};
class ErrorWriteNotAllowed: public Error {};

// instances
class ErrorLookupNotAllowed: public Error {};
class ErrorCallNotAllowed: public Error {};

// lookup
class ErrorSymbolNotFound: public Error {};
class ErrorTypeCollision: public Error {};

// blocks
class ErrorBlockNotFound: public Error {};
class ErrorOverloadNotFound: public Error {};
class ErrorOverloadAmbiguous: public Error {};
class ErrorTooManyArguments: public Error {};

}

#pragma once

namespace libblock {

class Error {};

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
class ErrorOverloadNotFound: public Error {};
class ErrorOverloadAmbiguous: public Error {};
class ErrorTooManyArguments: public Error {};

}

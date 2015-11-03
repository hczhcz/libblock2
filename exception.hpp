#pragma once

class Error {};

class ErrorSymbolNotFound: public Error {};
class ErrorTypeCollision: public Error {};

class ErrorReadNotAllowed: public Error {};
class ErrorWriteNotAllowed: public Error {};
class ErrorLookupNotAllowed: public Error {};
class ErrorCallNotAllowed: public Error {};

class ErrorCallNotConsistent: public Error {};

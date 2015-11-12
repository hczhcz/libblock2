#pragma once

class Error {};

class ErrorSymbolNotFound: public Error {};
class ErrorTypeCollision: public Error {};

class ErrorReadNotAllowed: public Error {};
class ErrorWriteNotAllowed: public Error {};
class ErrorDiscardNotAllowed: public Error {};
class ErrorLookupNotAllowed: public Error {};
class ErrorCallNotAllowed: public Error {};

class ErrorTooManyArguments: public Error {};

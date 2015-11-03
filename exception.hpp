#pragma once

class Error {};

class ErrorSymbolNotFound: public Error {};
class ErrorSymbolNotReadable: public Error {};
class ErrorSymbolNotWriteable: public Error {};

class ErrorTypeCollision: public Error {};

class ErrorCallNotAllowed: public Error {};
class ErrorCallNotConsistent: public Error {};

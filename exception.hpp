#pragma once

struct Error {};

struct ErrorSymbolNotFound: public Error {};
struct ErrorTypeCollision: public Error {};

struct ErrorReadNotAllowed: public Error {};
struct ErrorWriteNotAllowed: public Error {};
struct ErrorLookupNotAllowed: public Error {};
struct ErrorCallNotAllowed: public Error {};

struct ErrorCallNotConsistent: public Error {};

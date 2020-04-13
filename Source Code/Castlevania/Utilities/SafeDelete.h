#pragma once

#define SAFE_DELETE(pPtr) { delete pPtr; pPtr = nullptr; }


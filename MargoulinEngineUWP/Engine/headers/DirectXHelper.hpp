#ifndef __DIRECTXHELPER_HPP__
#define __DIRECTXHELPER_HPP__

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#endif

#endif /*__DIRECTXHELPER_HPP__*/
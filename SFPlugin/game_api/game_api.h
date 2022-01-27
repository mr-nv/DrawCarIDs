#ifndef _GAME_API
#define _GAME_API

#ifndef API
	#if defined(SF_EXPORTS)
		#define API _declspec(dllexport)
	#else
		#define API _declspec(dllimport)
	#endif
#endif

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "sdk/SharedUtil.h"
#include "game/CGame.h"

extern CGame API *GAME;
extern CPed API *PEDSELF;
#endif
#include <windows.h>
#include <string>
#include <assert.h>
#include <process.h>

#include "SAMPFUNCS_API.h"
#include "game_api\game_api.h"

SAMPFUNCS* sf = new SAMPFUNCS( );

bool shoulddraw = false;
stFontInfo* font;

void PrintMessage( const char* str )
{
	sf->getSAMP( )->getChat( )->AddChatMessage( ChatMessageType::CHAT_TYPE_CHAT,
		str, "DrawCarIDs", D3DCOLOR_RGBA( 255, 255, 255, 255 ), D3DCOLOR_RGBA( 78, 105, 242, 255 ) );
}

bool CALLBACK Present( CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride,
	CONST RGNDATA* pDirtyRegion )
{
	if( shoulddraw )
	{
		if( SUCCEEDED( sf->getRender( )->BeginRender( ) ) )
		{
			const auto vehicles = sf->getSAMP( )->getVehicles( );
			for( int i = 0; i < vehicles->iVehicleCount; i++ )
			{
				const auto gta = vehicles->pGTA_Vehicle[ i ];

				if( gta )
				{
					bool found = false;
					for( int j = 0; j < gta->m_nMaxPassengers; j++ )
					{
						if( !found && gta->passengers[ j ] )
							found = true;
					}

					if( !found )
					{
						CVector pos;
						CVector2D pos2d;

						// https://www.blast.hk/threads/11275/post-98104
						// íó è ÕÓÉÍß
						pos.fX = gta->base.matrix[ 4 * 3 ];
						pos.fY = gta->base.matrix[ 4 * 3 + 1 ];
						pos.fZ = gta->base.matrix[ 4 * 3 + 2 ];

						sf->getGame( )->convert3DCoordsToScreen( pos.fX, pos.fY, pos.fZ, &pos2d.fX, &pos2d.fY );

						// ýòî ïðîåáûâàåòñÿ è åñëè òû íå ñìîòðèøü íà ìàøèíó è òû äàëåêî òî îíî â ðàíäîì ìåñòå íà ýêðàíå ðèñóåò èä
						// íî ìíå ÏÎÎÕÓÓÓÓÓÓÓÓÓÓÓÉéééééééééééééééééééé
						if( pos2d.fX > 0 && pos2d.fY > 0 )
						{
							char id[ 12 ];
							sprintf_s( id, "%i", i );
							font->PrintShadow( id, -1, pos2d.fX, pos2d.fY, true, true );
						}
					}
				}
			}

			sf->getRender( )->EndRender( );
		}
	}

	return true;
}

void CALLBACK Callback_DrawIDs( std::string param )
{
	if( shoulddraw )
	{
		shoulddraw = false;
		PrintMessage( "Âûêëþ÷åíî" );
	}
	else
	{
		shoulddraw = true;
		PrintMessage( "Âêëþ÷åíî" );
	}
}

void CALLBACK mainloop( )
{
	static bool init = false;
	if( !init )
	{
		if( GAME == nullptr )
			return;
		if( GAME->GetSystemState( ) != eSystemState::GS_PLAYING_GAME )
			return;
		if( !sf->getSAMP( )->IsInitialized( ) )
			return;

		sf->getSAMP( )->registerChatCommand( "carids", Callback_DrawIDs );
		sf->getRender( )->registerD3DCallback( eDirect3DDeviceMethods::D3DMETHOD_PRESENT, Present );
		font = sf->getRender( )->CreateNewFont( "Tahoma", 12, FCR_BORDER );

		init = true;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved )
{
	switch( dwReasonForCall )
	{
		case DLL_PROCESS_ATTACH:
			sf->initPlugin( mainloop, hModule );
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

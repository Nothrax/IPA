/*Template for IPA project, 3D game
*Anton Claes (https://www.youtube.com/watch?v=a3MACw5hB2Q)
*
*/


#include <SDL_mixer.h>
#include "sound.h"

void play_shot()
{
    static Mix_Music *feu;
    static bool loaded=false;
    if(!loaded)
    {
        loaded = true;
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);//init de la bibliotheque audio
        feu=Mix_LoadMUS("feu.wav");
    }
    else
    {
        Mix_PlayMusic(feu, 1);
    }


}

void quit_audio()
{
    Mix_CloseAudio();

}

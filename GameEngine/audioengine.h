#pragma once
#include <SDL\SDL_mixer.h>
#include <string>
#include <map>

namespace GameEngine
{
	class SoundEffect {
	public:
		//gör så att soundefect har accesss till AudioEngine's private variables
		friend class AudioEngine;
		void play(int loops = 0);

	private:
		Mix_Chunk* m_chunk = nullptr;
	};


	//Klass som används för att hantera ett Mix_Music objekt
	class Music {
	public:
		friend class AudioEngine;

		//om loops == -1, loopa för alltid, annars spelas klippet "loop" ggr
		void play(int loops = -1);
		static void pause();
		static void stop();
		static void resume();

	private:
		Mix_Music* m_music = nullptr;
	};


	//Klass som hanterar ljud som används i spelet
	class AudioEngine
	{
		public:
			AudioEngine();
			~AudioEngine();

			void init();
			void destroy();

			SoundEffect loadSoundEffect(const std::string& filePath);
			Music loadMusic(const std::string& filePath);

		private:
			bool m_isInitialized = false;
			std::map<std::string, Mix_Chunk*> m_effectMap;
			std::map<std::string, Mix_Music*> m_musicMap;
	};
}
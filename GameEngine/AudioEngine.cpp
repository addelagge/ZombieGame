#include "AudioEngine.h"
#include "GameEngineErrors.h"

namespace GameEngine
{
	void SoundEffect::play(int loops/* = 0*/)
	{
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1){
			fatalError("MixPlayChannel error " + std::string(Mix_GetError()));
		}
	}

	void Music::play(int loops /* = -1 */)
	{
		if (Mix_PlayMusic(m_music, loops) == -1){
			fatalError("MixPlayMusic error " + std::string(Mix_GetError()));
		}
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	
	AudioEngine::AudioEngine()
	{

	}

	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	void AudioEngine::init()
	{
		if (m_isInitialized){
			fatalError("Audioengine already initialized");
		}
		
		//Parameter can be a bitwise combination of MIX_INIT_FAC
		//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1){
			fatalError("Mix init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1){
			fatalError("Mix open audio error: " + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}


	//Frigör alla ljudklipp som lagrats i m_musicMap & m_effectMap
	void AudioEngine::destroy()
	{
		if (m_isInitialized){
			m_isInitialized = false;
			for (auto it = m_effectMap.begin(); it != m_effectMap.end(); it++){
				Mix_FreeChunk(it->second);
			}

			for (auto it = m_musicMap.begin(); it != m_musicMap.end(); it++){
				Mix_FreeMusic(it->second);
			}

			m_effectMap.clear();
			m_musicMap.clear();
			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	//Laddar en ljudeffekt. Först kollas om ljudet redan laddats tidigare och lagrats i m_effectMap
	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath)
	{
		//Finns ljudeffekten i m_effectMap?
		auto mit = m_effectMap.find(filePath);
		SoundEffect effect;

		//Om ljudeffekten inte laddats tidigare
		if (mit == m_effectMap.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr){
				fatalError("Mix_LoadWAV error " + std::string(Mix_GetError()));
			}
		
			m_effectMap[filePath] = chunk;
			effect.m_chunk = chunk;			
		}

		//om ljudet laddats tidigare
		else{
			effect.m_chunk = mit->second;
		}

		return effect;
	}

	//Laddar ett ljudklipp. Först kollas om ljudet redan laddats tidigare och lagrats i m_musicMap.
	Music AudioEngine::loadMusic(const std::string& filePath)
	{
		//Finns ljudet i m_musicMap?
		auto mit = m_musicMap.find(filePath);
		Music music;

		//Om ljudet inte laddats tidigare
		if (mit == m_musicMap.end())
		{
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr){
				fatalError("Mix_LoadMUS error " + std::string(Mix_GetError()));
			}

			m_musicMap[filePath] = mixMusic;
			music.m_music = mixMusic;
		}

		//om ljudet laddats tidigare
		else{
			music.m_music = mit->second;
		}

		return music;
	}
}
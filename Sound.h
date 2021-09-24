#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <iostream>
class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	LPDIRECTSOUND8 m_DirectSound;
	LPDIRECTSOUNDBUFFER m_primaryBuffer;
	LPDIRECTSOUNDBUFFER8 m_secondaryBuffer;
public:
	Sound();
	Sound(const Sound&);
	~Sound();

	bool OnInit(HWND hWnd, const char* filename);
	void OnRelease();
	bool PlayWaveFile();
	bool PlayWaveFile(float distance);
private:
	bool OnInitDirectSound(HWND hWnd);
	void OnReleaseDirectSound();

	bool LoadWaveFile(char* file, LPDIRECTSOUNDBUFFER8 buf);
	void ReleaseWaveFile(LPDIRECTSOUNDBUFFER8);
};


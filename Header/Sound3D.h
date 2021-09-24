#pragma once
#pragma comment(lib, "WinMM")
#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <iostream>
class Sound3D
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
private:
	LPDIRECTSOUND8 m_DirectSound = nullptr;
	LPDIRECTSOUNDBUFFER m_primaryBuffer = nullptr;
	LPDIRECTSOUND3DLISTENER8 m_Listener = nullptr;
	LPDIRECTSOUNDBUFFER8 m_secondaryBuffer = nullptr;
	LPDIRECTSOUND3DBUFFER8 m_secondary3DBuffer = nullptr;

private:
	bool InitDirectSound(HWND hWnd);
	void ReleaseDirectSound();

	bool ReadWaveFile(const char* filePath, WAVEFORMATEX& dest, DWORD& dataSize, BYTE* Data);
	bool LoadWaveFile(const char*, LPDIRECTSOUNDBUFFER8*, LPDIRECTSOUND3DBUFFER8*);
	bool LoadWaveFileWithMMIO(const char* filename, LPDIRECTSOUNDBUFFER8* secondaryBuffer, LPDIRECTSOUND3DBUFFER8* secondary3DBuffer);
	void ReleaseWaveFile(LPDIRECTSOUNDBUFFER8*, LPDIRECTSOUND3DBUFFER*);
public:
	Sound3D();
	Sound3D(const Sound3D&);
	~Sound3D();

	bool Init(HWND hWnd);
	bool Init(HWND hWnd, const char* filePath);
	bool PlayWaveFile();
	bool PlayWaveFile(float x, float y, float z);
	bool SetListener(float x, float y, float z);
	void Shutdown();
};



#include "Sound.h"

Sound::Sound()
{

}

Sound::Sound(const Sound& other)
{

}

Sound::~Sound()
{

}

bool Sound::OnInit(HWND hWnd, const char* filename)
{
	if (!OnInitDirectSound(hWnd))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init Direct Sound.", L"Error", MB_OK);
#endif
		return false;
	}

	if (!LoadWaveFile((char*)filename, m_secondaryBuffer))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Load Wav File.", L"Error", MB_OK);
#endif
		return false;
	}
	return true;
}

void Sound::OnRelease()
{
	ReleaseWaveFile(m_secondaryBuffer);
	OnReleaseDirectSound();
}

bool Sound::OnInitDirectSound(HWND hWnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Direct Sound.", L"Error", MB_OK);
#endif
		return false;
	}

	if (FAILED(m_DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot SetCooperativeLevel.", L"Error", MB_OK);
#endif
		return false;
	}

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init Direct Sound.", L"Error", MB_OK);
#endif
		return false;
	}

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	if (FAILED(m_primaryBuffer->SetFormat(&waveFormat)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init Direct Sound.", L"Error", MB_OK);
#endif
		return false;
	}

	return true;
}
void Sound::OnReleaseDirectSound()
{
	if (m_primaryBuffer != NULL)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if (m_DirectSound != NULL)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

bool Sound::LoadWaveFile(char* file, LPDIRECTSOUNDBUFFER8 buf)
{
	// wav File
	// BitsPerSample = 16;
	// SampleRate = 44100;
	// FormatTag = WAVE_FORMAT_PCM
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, file, "rb");
	if (error != 0)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Open Wav File.", L"Error", MB_OK);
#endif
		return false;
	}

	WaveHeaderType waveFileHeader;
	unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Read Wav File.", L"Error", MB_OK);
#endif
		return false;
	}

	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"File isn't RIFF format", L"Error", MB_OK);
#endif
		return false;
	}

	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"File isn't WAVE format", L"Error", MB_OK);
#endif
		return false;
	}

	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"File isn't fmt format", L"Error", MB_OK);
#endif
		return false;
	}

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"File isn't fmt format", L"Error", MB_OK);
#endif
		return false;
	}

	if (waveFileHeader.sampleRate != 44100)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Please Set SampleRate 44100.", L"Error", MB_OK);
#endif
		return false;
	}

	if (waveFileHeader.bitsPerSample != 16)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Please Set BitsPerSample 16", L"Error", MB_OK);
#endif
		return false;
	}

	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Data Chunk header is not data", L"Error", MB_OK);
#endif
		return false;
	}

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8);
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	LPDIRECTSOUNDBUFFER tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Temp Sound Buffer.", L"Error", MB_OK);
#endif
		return false;
	}

	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_secondaryBuffer)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create To Secondary Buffer.", L"Error", MB_OK);
#endif
		return false;
	}

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create  temporary Buffer.", L"Error", MB_OK);
#endif
		return false;
	}

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot read Sound Data.", L"Error", MB_OK);
#endif
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot close Sound File.", L"Error", MB_OK);
#endif
		return false;
	}

	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((m_secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Lock Secondary Buffer.", L"Error", MB_OK);
#endif
		return false;
	}
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	if (FAILED((m_secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Unlock Secondary Buffer.", L"Error", MB_OK);
#endif
		return false;
	}

	delete[] waveData;
	waveData = 0;

	return true;
}

void Sound::ReleaseWaveFile(LPDIRECTSOUNDBUFFER8 secondaryBuffer)
{
	if (secondaryBuffer != NULL)
	{
		secondaryBuffer->Release();
		secondaryBuffer = 0;
	}
}

bool Sound::PlayWaveFile()
{
	if (FAILED(m_secondaryBuffer->SetCurrentPosition(0)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Set Position of Sound.", L"Error", MB_OK);
#endif
		return false;
	}

	if (FAILED(m_secondaryBuffer->SetVolume(DSBVOLUME_MAX)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Set Position of Sound.", L"Error", MB_OK);
#endif
		return false;
	}

	if (FAILED(m_secondaryBuffer->Play(0, 0, 0)))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Set Position of Sound.", L"Error", MB_OK);
#endif
		return false;
	}
}

bool Sound::PlayWaveFile(float distance)
{
	return true;
}
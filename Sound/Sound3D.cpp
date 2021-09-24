#include "Sound3D.h"

Sound3D::Sound3D()
{

}

Sound3D::Sound3D(const Sound3D& other)
{

}

Sound3D::~Sound3D()
{

}

bool Sound3D::Init(HWND hWnd)
{
	if (!InitDirectSound(hWnd))
		return false;

	if (!LoadWaveFile("./Sound/Test.wav", &m_secondaryBuffer, &m_secondary3DBuffer))
		return false;

	return true;
}

bool Sound3D::Init(HWND hWnd, const char* filePath)
{
	//ReadWaveFile(filePath);
	if (!InitDirectSound(hWnd))
		return false;

	if (!LoadWaveFile(filePath, &m_secondaryBuffer, &m_secondary3DBuffer))
		return false;

	//if (!LoadWaveFileWithMMIO(filePath, &m_secondaryBuffer, &m_secondary3DBuffer))
	//	return false;

	return true;
}

void Sound3D::Shutdown()
{
	ReleaseWaveFile(&m_secondaryBuffer, &m_secondary3DBuffer);

	ReleaseDirectSound();
}

bool Sound3D::InitDirectSound(HWND hWnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
	{
		return false;
	}

	if (FAILED(m_DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return false;
	}

	// 기본 버퍼 설명을 설정합니다.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL)))
	{
		return false;
	}

	// 기본 사운드 버퍼의 형식을 설정합니다.
	// 이 경우 16 비트 스테레오 (cd 오디오 형식)에서 44,100 샘플 / 초로 기록되는 .WAV 파일입니다.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	if (FAILED(m_primaryBuffer->SetFormat(&waveFormat)))
	{
		return false;
	}

	if (FAILED(m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_Listener)))
	{
		return false;
	}

	// 리스너의 초기 위치를 장면의 중간에 설정합니다.
	m_Listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);
	return true;
}

void Sound3D::ReleaseDirectSound()
{
	// 리스너 인터페이스를 해제합니다.
	if (m_Listener != NULL)
	{
		m_Listener->Release();
		m_Listener = 0;
	}

	// 기본 사운드 버퍼 포인터를 놓습니다.
	if (m_primaryBuffer != NULL)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// 직접 사운드 인터페이스 포인터를 놓습니다.
	if (m_DirectSound != NULL)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

// 제작중
bool Sound3D::LoadWaveFileWithMMIO(const char* filename, LPDIRECTSOUNDBUFFER8* secondaryBuffer, LPDIRECTSOUND3DBUFFER8* secondary3DBuffer)
{
	WAVEFORMATEX m_ReadFile;
	DWORD waveSize = 0;
	BYTE* waveData = nullptr;
	ReadWaveFile(filename, m_ReadFile, waveSize, waveData);
	
	// 웨이브 파일이로드 될 2 차 사운드 버퍼의 버퍼 설명을 설정합니다.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveSize; // 크기 설정
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &m_ReadFile;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// 특정 버퍼 설정으로 임시 사운드 버퍼를 만듭니다.
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return false;
	}

	// 다이렉트 사운드 인터페이스에 대해 버퍼 형식을 테스트하고 보조 버퍼를 만듭니다.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
	{
		return false;
	}

	// 임시 버퍼를 해제한다.
	tempBuffer->Release();
	tempBuffer = 0;

	// 웨이브 데이터를 쓰기 위해 2차 버퍼를 잠급니다.
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((*secondaryBuffer)->Lock(0, waveSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return false;
	}

	// 웨이브 데이터를 버퍼에 복사합니다.
	memcpy(bufferPtr, waveData, waveSize);

	// 데이터가 쓰여진 후 보조 버퍼의 잠금을 해제합니다.
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return false;
	}

	// 보조 버퍼에 복사 된 웨이브 데이터를 해제합니다.
	delete[] waveData;
	waveData = 0;

	// 2 차 사운드 버퍼에 3D 인터페이스 가져 오기.
	if (FAILED((*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer)))
	{
		return false;
	}

	return true;
}

bool Sound3D::LoadWaveFile(const char* filename, LPDIRECTSOUNDBUFFER8* secondaryBuffer, LPDIRECTSOUND3DBUFFER8* secondary3DBuffer)
{
	// 웨이브 파일을 바이너리 모드로 엽니다.
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// 웨이브 파일 헤더를 읽는다
	WaveHeaderType waveFileHeader;
	unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// RIFF 포멧 chunk id 체크한다.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// 파일포멧이 wave 인지 체크 한다
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// fmt 포멧 chunk id 체크한다.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// 오디오 형식이 WAVE_FORMAT_PCM인지 체크한다
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// 웨이브 파일이 모노 형식인지 스테레오인지 체크한다.
	if (waveFileHeader.numChannels != 1)
	{
		return false;
	}

	// 웨이브 파일이 44.1 KHz의 샘플 속도로 레코딩되었는지 확인합니다.
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// 웨이브 파일이 16 비트 형식으로 레코딩되었는지 확인합니다.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// 헤더 데이터의 chunk를 확인한다.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// 이 웨이브 파일이로드 될 보조 버퍼의 웨이브 형식을 설정합니다.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// 웨이브 파일이로드 될 2 차 사운드 버퍼의 버퍼 설명을 설정합니다.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// 특정 버퍼 설정으로 임시 사운드 버퍼를 만듭니다.
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return false;
	}

	// 다이렉트 사운드 인터페이스에 대해 버퍼 형식을 테스트하고 보조 버퍼를 만듭니다.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
	{
		return false;
	}

	// 임시 버퍼를 해제한다.
	tempBuffer->Release();
	tempBuffer = 0;

	// 데이터 청크 헤더의 끝에서 시작하는 웨이브 데이터의 시작 부분으로 이동합니다.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// 웨이브 파일 데이터를 저장할 임시 버퍼를 만듭니다.
	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// 웨이브 파일 데이터를 새로 생성 된 버퍼로 읽어들입니다.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// 읽고 나면 파일을 닫는다.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// 웨이브 데이터를 쓰기 위해 2차 버퍼를 잠급니다.
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return false;
	}

	// 웨이브 데이터를 버퍼에 복사합니다.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// 데이터가 쓰여진 후 보조 버퍼의 잠금을 해제합니다.
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return false;
	}

	// 보조 버퍼에 복사 된 웨이브 데이터를 해제합니다.
	delete[] waveData;
	waveData = 0;

	// 2 차 사운드 버퍼에 3D 인터페이스 가져 오기.
	if (FAILED((*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer)))
	{
		return false;
	}

	return true;
}

void Sound3D::ReleaseWaveFile(LPDIRECTSOUNDBUFFER8* secondaryBuffer, LPDIRECTSOUND3DBUFFER* secondary3DBuffer)
{
	if (*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}

	// 2 차 사운드 버퍼를 해제한다.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

bool Sound3D::PlayWaveFile()
{
	// 사운드를 배치 할 위치의 3D 위치를 설정합니다.
	float positionX = -2.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;

	// 사운드 버퍼의 시작 부분에 위치를 설정합니다.
	if (FAILED(m_secondaryBuffer->SetCurrentPosition(0)))
	{
		return false;
	}

	// 버퍼의 볼륨을 100 %로 설정합니다.
	if (FAILED(m_secondaryBuffer->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}

	// 사운드의 3D 위치를 설정합니다.
	m_secondary3DBuffer->SetPosition(positionX, positionY, positionZ, DS3D_IMMEDIATE);

	// 2 차 사운드 버퍼의 내용을 재생합니다.
	if (FAILED(m_secondaryBuffer->Play(0, 0, 0)))
	{
		return false;
	}

	return true;
}

bool Sound3D::PlayWaveFile(float x, float y, float z)
{
	// 사운드를 배치 할 위치의 3D 위치를 설정합니다.
	float positionX = x;
	float positionY = y;
	float positionZ = z;

	// 사운드 버퍼의 시작 부분에 위치를 설정합니다.
	if (FAILED(m_secondaryBuffer->SetCurrentPosition(0)))
	{
		return false;
	}

	// 버퍼의 볼륨을 100 %로 설정합니다.
	if (FAILED(m_secondaryBuffer->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}

	// 사운드의 3D 위치를 설정합니다.
	m_secondary3DBuffer->SetPosition(positionX, positionY, positionZ, DS3D_IMMEDIATE);

	// 2 차 사운드 버퍼의 내용을 재생합니다.
	if (FAILED(m_secondaryBuffer->Play(0, 0, 0)))
	{
		return false;
	}

	return true;
}

bool Sound3D::ReadWaveFile(const char* filePath, WAVEFORMATEX& dest, DWORD& dataSize, BYTE* Data)
{
	if (nullptr == filePath)
		return -1;

	// Wave File Open
	HMMIO hmmio = nullptr;
	hmmio = mmioOpenA((LPSTR)filePath, nullptr, MMIO_READ | MMIO_ALLOCBUF);
	if (nullptr == hmmio)
		return -2;

	// Find RIFF Chunk
	MMCKINFO mmckParent;
	memset(&mmckParent, 0x00, sizeof(mmckParent));
	mmckParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	MMRESULT mmRes = mmioDescend(hmmio, &mmckParent, nullptr, MMIO_FINDRIFF);
	if (MMSYSERR_NOERROR != mmRes)
	{
		mmioClose(hmmio, 0);
		return -3;
	}

	// Find Format Chunk
	MMCKINFO mmckChild;
	memset(&mmckChild, 0x00, sizeof(mmckChild));
	mmckChild.fccType = mmioFOURCC('f', 'm', 't', ' ');

	mmRes = mmioDescend(hmmio, &mmckChild, &mmckParent, MMIO_FINDCHUNK);
	if (MMSYSERR_NOERROR != mmRes)
	{
		mmioClose(hmmio, 0);
		return -4;
	}

	WAVEFORMATEX wfex;
	memset(&wfex, 0x00, sizeof(wfex));
	mmioRead(hmmio, (HPSTR)&wfex, mmckChild.cksize);
	dest = wfex;
	//std::cout << "wFormatTag      : " << wfex.wFormatTag << std::endl;
	//std::cout << "nChannels       : " << wfex.nChannels << std::endl;
	//std::cout << "nSamplesPerSec  : " << wfex.nSamplesPerSec << std::endl;
	//std::cout << "wBitsPerSample  : " << wfex.wBitsPerSample << std::endl;
	//std::cout << "nBlockAlign     : " << wfex.nBlockAlign << std::endl;
	//std::cout << "nAvgBytesPerSec : " << wfex.nAvgBytesPerSec << std::endl;

	// Find Data Chunk
	mmioAscend(hmmio, &mmckChild, 0);

	mmckChild.ckid = mmioFOURCC('d', 'a', 't', 'a');

	mmRes = mmioDescend(hmmio, &mmckChild, &mmckParent, MMIO_FINDCHUNK);
	if (MMSYSERR_NOERROR != mmRes)
	{
		mmioClose(hmmio, 0);
		return -5;
	}

	DWORD dwDataSize = mmckChild.cksize;
	dataSize = dwDataSize;
	// std::cout << "Data Size       : " << dwDataSize << std::endl;

	// Read Wave Data
 	BYTE* pData = nullptr;
 	try
 	{
		pData = new BYTE[dwDataSize];
 	}
 	catch (std::bad_alloc e)
 	{
 		mmioClose(hmmio, 0);
 		return -6;
 	}
 	mmioRead(hmmio, (LPSTR)pData, dwDataSize);
	Data = pData;

	mmioClose(hmmio, 0);

	return 0;
}

bool Sound3D::SetListener(float x, float y, float z)
{
	m_Listener->SetPosition(x, y, z, DS3D_IMMEDIATE);
	return true;
}
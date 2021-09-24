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

	// �⺻ ���� ������ �����մϴ�.
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

	// �⺻ ���� ������ ������ �����մϴ�.
	// �� ��� 16 ��Ʈ ���׷��� (cd ����� ����)���� 44,100 ���� / �ʷ� ��ϵǴ� .WAV �����Դϴ�.
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

	// �������� �ʱ� ��ġ�� ����� �߰��� �����մϴ�.
	m_Listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);
	return true;
}

void Sound3D::ReleaseDirectSound()
{
	// ������ �������̽��� �����մϴ�.
	if (m_Listener != NULL)
	{
		m_Listener->Release();
		m_Listener = 0;
	}

	// �⺻ ���� ���� �����͸� �����ϴ�.
	if (m_primaryBuffer != NULL)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// ���� ���� �������̽� �����͸� �����ϴ�.
	if (m_DirectSound != NULL)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

// ������
bool Sound3D::LoadWaveFileWithMMIO(const char* filename, LPDIRECTSOUNDBUFFER8* secondaryBuffer, LPDIRECTSOUND3DBUFFER8* secondary3DBuffer)
{
	WAVEFORMATEX m_ReadFile;
	DWORD waveSize = 0;
	BYTE* waveData = nullptr;
	ReadWaveFile(filename, m_ReadFile, waveSize, waveData);
	
	// ���̺� �����̷ε� �� 2 �� ���� ������ ���� ������ �����մϴ�.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveSize; // ũ�� ����
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &m_ReadFile;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Ư�� ���� �������� �ӽ� ���� ���۸� ����ϴ�.
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return false;
	}

	// ���̷�Ʈ ���� �������̽��� ���� ���� ������ �׽�Ʈ�ϰ� ���� ���۸� ����ϴ�.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
	{
		return false;
	}

	// �ӽ� ���۸� �����Ѵ�.
	tempBuffer->Release();
	tempBuffer = 0;

	// ���̺� �����͸� ���� ���� 2�� ���۸� ��޴ϴ�.
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((*secondaryBuffer)->Lock(0, waveSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return false;
	}

	// ���̺� �����͸� ���ۿ� �����մϴ�.
	memcpy(bufferPtr, waveData, waveSize);

	// �����Ͱ� ������ �� ���� ������ ����� �����մϴ�.
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return false;
	}

	// ���� ���ۿ� ���� �� ���̺� �����͸� �����մϴ�.
	delete[] waveData;
	waveData = 0;

	// 2 �� ���� ���ۿ� 3D �������̽� ���� ����.
	if (FAILED((*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer)))
	{
		return false;
	}

	return true;
}

bool Sound3D::LoadWaveFile(const char* filename, LPDIRECTSOUNDBUFFER8* secondaryBuffer, LPDIRECTSOUND3DBUFFER8* secondary3DBuffer)
{
	// ���̺� ������ ���̳ʸ� ���� ���ϴ�.
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// ���̺� ���� ����� �д´�
	WaveHeaderType waveFileHeader;
	unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// RIFF ���� chunk id üũ�Ѵ�.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// ���������� wave ���� üũ �Ѵ�
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// fmt ���� chunk id üũ�Ѵ�.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// ����� ������ WAVE_FORMAT_PCM���� üũ�Ѵ�
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// ���̺� ������ ��� �������� ���׷������� üũ�Ѵ�.
	if (waveFileHeader.numChannels != 1)
	{
		return false;
	}

	// ���̺� ������ 44.1 KHz�� ���� �ӵ��� ���ڵ��Ǿ����� Ȯ���մϴ�.
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// ���̺� ������ 16 ��Ʈ �������� ���ڵ��Ǿ����� Ȯ���մϴ�.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// ��� �������� chunk�� Ȯ���Ѵ�.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// �� ���̺� �����̷ε� �� ���� ������ ���̺� ������ �����մϴ�.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// ���̺� �����̷ε� �� 2 �� ���� ������ ���� ������ �����մϴ�.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Ư�� ���� �������� �ӽ� ���� ���۸� ����ϴ�.
	IDirectSoundBuffer* tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return false;
	}

	// ���̷�Ʈ ���� �������̽��� ���� ���� ������ �׽�Ʈ�ϰ� ���� ���۸� ����ϴ�.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
	{
		return false;
	}

	// �ӽ� ���۸� �����Ѵ�.
	tempBuffer->Release();
	tempBuffer = 0;

	// ������ ûũ ����� ������ �����ϴ� ���̺� �������� ���� �κ����� �̵��մϴ�.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// ���̺� ���� �����͸� ������ �ӽ� ���۸� ����ϴ�.
	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// ���̺� ���� �����͸� ���� ���� �� ���۷� �о���Դϴ�.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// �а� ���� ������ �ݴ´�.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// ���̺� �����͸� ���� ���� 2�� ���۸� ��޴ϴ�.
	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return false;
	}

	// ���̺� �����͸� ���ۿ� �����մϴ�.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// �����Ͱ� ������ �� ���� ������ ����� �����մϴ�.
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return false;
	}

	// ���� ���ۿ� ���� �� ���̺� �����͸� �����մϴ�.
	delete[] waveData;
	waveData = 0;

	// 2 �� ���� ���ۿ� 3D �������̽� ���� ����.
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

	// 2 �� ���� ���۸� �����Ѵ�.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

bool Sound3D::PlayWaveFile()
{
	// ���带 ��ġ �� ��ġ�� 3D ��ġ�� �����մϴ�.
	float positionX = -2.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;

	// ���� ������ ���� �κп� ��ġ�� �����մϴ�.
	if (FAILED(m_secondaryBuffer->SetCurrentPosition(0)))
	{
		return false;
	}

	// ������ ������ 100 %�� �����մϴ�.
	if (FAILED(m_secondaryBuffer->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}

	// ������ 3D ��ġ�� �����մϴ�.
	m_secondary3DBuffer->SetPosition(positionX, positionY, positionZ, DS3D_IMMEDIATE);

	// 2 �� ���� ������ ������ ����մϴ�.
	if (FAILED(m_secondaryBuffer->Play(0, 0, 0)))
	{
		return false;
	}

	return true;
}

bool Sound3D::PlayWaveFile(float x, float y, float z)
{
	// ���带 ��ġ �� ��ġ�� 3D ��ġ�� �����մϴ�.
	float positionX = x;
	float positionY = y;
	float positionZ = z;

	// ���� ������ ���� �κп� ��ġ�� �����մϴ�.
	if (FAILED(m_secondaryBuffer->SetCurrentPosition(0)))
	{
		return false;
	}

	// ������ ������ 100 %�� �����մϴ�.
	if (FAILED(m_secondaryBuffer->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}

	// ������ 3D ��ġ�� �����մϴ�.
	m_secondary3DBuffer->SetPosition(positionX, positionY, positionZ, DS3D_IMMEDIATE);

	// 2 �� ���� ������ ������ ����մϴ�.
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
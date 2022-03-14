#pragma once
#include <stdio.h>

class LoadFile final
{
public: //�V���O���g����
	static LoadFile* Get();
private:
	LoadFile() {}
	LoadFile(const LoadFile&) = delete;
	~LoadFile() {}
	LoadFile operator=(const LoadFile&) = delete;

public: //�ÓI�����o�֐�
	/// <summary>
	/// CSV�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
	/// <param name="mapSize"> �}�b�v�`�b�v�ꖇ������̑傫�� </param>
	/// <param name="filePath"> CSV�t�@�C���̃p�X </param>
	/// <param name="loadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
	/// <returns> 0�Ő����A0�ȊO�Ŏ��s </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, const char* filePath, const int& loadStopNumber = -1);
	/// <summary>
	/// CSV�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
	/// <param name="mapSize"> �}�b�v�`�b�v�ꖇ������̑傫�� </param>
	/// <param name="filePath"> CSV�t�@�C���̃p�X </param>
	/// <param name="loadStopCharacter"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
	/// <returns> 0�Ő����A0�ȊO�Ŏ��s </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, const char* filePath, const char& loadStopCharacter);
	/// <summary>
	/// CSV�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
	/// <param name="mapSize"> �}�b�v�`�b�v�ꖇ������̑傫�� </param>
	/// <param name="fileHandle"> CSV�t�@�C���̃n���h�� </param>
	/// <param name="loadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
	/// <returns> 0�Ő����A-1�Ŏ��s </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber = -1);
	/// <summary>
	/// CSV�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
	/// <param name="mapSize"> �}�b�v�`�b�v�ꖇ������̑傫�� </param>
	/// <param name="fileHandle"> CSV�t�@�C���̃n���h�� </param>
	/// <param name="loadStopCharacter"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
	/// <returns> 0�Ő����A-1�Ŏ��s </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, FILE* fileHandle, const char& loadStopCharacter);
	/// <summary>
	/// CSV�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
	/// <param name="mapSize"> �}�b�v�`�b�v�ꖇ������̑傫�� </param>
	/// <param name="fileHandle"> CSV�t�@�C���̃n���h�� </param>
	/// <param name="loadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
	/// <returns> 0�Ő����A-1�Ŏ��s </returns>
	static int LoadCSV(char* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber);
	/// <summary>
	/// CSV�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="mapArray"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
	/// <param name="mapSize"> �}�b�v�`�b�v�ꖇ������̑傫�� </param>
	/// <param name="fileHandle"> CSV�t�@�C���̃n���h�� </param>
	/// <param name="loadStopCharacter"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
	/// <returns> 0�Ő����A-1�Ŏ��s </returns>
	static int LoadCSV(char* mapArray, const size_t& mapSize, FILE* fileHandle, const char& loadStopCharacter);
};

#include "LoadFile.h"

#define EF (-1) //Error Function

LoadFile* LoadFile::Get()
{
	static LoadFile instance = {};
	return &instance;
}

int LoadFile::LoadCSV(int* mapArray, const size_t& mapSize, const char* filePath, const int& loadStopNumber)
{
	for (size_t i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	static FILE* fileHandle;
	static errno_t err;
	static char string[256];
	static int index;
	for (size_t i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = 0;
	index = 0;

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	static bool isMinus;
	isMinus = false;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		static bool end;
		static bool isSlash;
		end = false;
		isSlash = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}

				if (mapArray[index] == loadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}

int LoadFile::LoadCSV(int* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber)
{
	if (fileHandle == nullptr)
	{
		return EF;
	}
	for (size_t i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	static char string[256];
	static int index;
	for (size_t i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = 0;
	index = 0;

	static bool isMinus;
	isMinus = false;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		static bool end;
		static bool isSlash;

		end = false;
		isSlash = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] == '@')
			{
				end = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}

				if (mapArray[index] == loadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}

	return 0;
}

int LoadFile::LoadCSV(char* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber)
{
	if (fileHandle == nullptr)
	{
		return EF;
	}
	for (size_t i = 0; i < mapSize; i++)
	{
		mapArray[i] = '\0';
	}

	static char string[256];
	static int index;
	for (size_t i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = 0;
	index = 0;

	static bool isMinus;
	isMinus = false;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		static bool end;
		static bool isSlash;

		end = false;
		isSlash = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] == '@')
			{
				end = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}

				if (mapArray[index] == loadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}

	return 0;
}

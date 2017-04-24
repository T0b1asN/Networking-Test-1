#include "NetworkHelpers.h"

bool ntwrk::validIp(std::string ip)
{
	//Check if correct number of dots are there
	int dotCount = 0;
	for (int i = 0; i < (int)ip.length(); i++)
	{
		if (ip[i] == '.')
			dotCount++;
	}
	if (dotCount != 3)
		return false;
	//Check individual numbers
	int lastDotIndex = -1;
	for (int i = 0; i < 4; i++)
	{
		std::string currNum;
		bool dot = false;
		int j = lastDotIndex + 1;
		//loop for numbers before dots
		while (!dot && i < 3)
		{
			//Check if all chars until the next dot are numbers
			if (ip[j] != '.')
			{
				if (ip[j] >= 48 && ip[j] <= 57)
					currNum += ip[j];
				else
					return false;
			}
			else
			{
				lastDotIndex = j;
				dot = true;
			}

			j++;
		}
		//special case after last dot
		if (i == 3)
		{
			for (int h = lastDotIndex + 1; h < (int)ip.length(); h++)
			{
				if (ip[h] >= 48 && ip[h] <= 57)
					currNum += ip[h];
				else
					return false;
			}
		}

		//Check for 4digit parts is unnessecary, because it is checked if the number is lower than 255
		//a 4digit number is always bigger than 255
		try
		{
			int bit = std::stoi(currNum);
			if (!(bit >= 0 && bit <= 255))
				return false;
		}
		catch (const std::invalid_argument&)
		{
			return false;
		}
	}

	return true;
}
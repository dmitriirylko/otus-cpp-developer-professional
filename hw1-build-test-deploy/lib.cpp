#include "lib.h"

bool isVersionGreater(std::string v1, std::string v2)
{
	/* Stores integer value of each part of version. */
    int vnum1 = 0, vnum2 = 0;

    for(int i = 0, j = 0; (i < v1.length() || j < v2.length());)
	{
		/* Convert one part of version to number. */
        while(i < v1.length() && v1[i] != '.')
		{
            vnum1 = vnum1 * 10 + (v1[i] - '0');
            i++;
        }
		/* Convert other part of version to number. */
        while(j < v2.length() && v2[j] != '.')
		{
            vnum2 = vnum2 * 10 + (v2[j] - '0');
            j++;
        }
        /* Compare part of versions. */
        if(vnum1 > vnum2)
		{
			return true;
		}
        else if(vnum1 < vnum2)
		{
			return false;
		}
		else
		{
            /* Reset number variables and go for next numeric part. */ 
            vnum1 = vnum2 = 0;
            i++;
            j++;
            continue;
        }
    }
    return false;
}


std::string version()
{
    return std::string(PROJECT_VERSION);
}
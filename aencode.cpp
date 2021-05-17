/*
Author: Anant Krishna Mahale.
zID: z5277610

Structure:
______________________________________________
| Symbol | Probability | lowValue | highValue|
|________|_____________|__________|__________|

*/

#include <string>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <map>
#include <mpfr.h>
#include<cstring>

using namespace std;

struct lookUpTable
{
    int symbol; //input Symbol.
    mpfr_t probability;
    mpfr_t highValue;
    mpfr_t lowValue;
};

int main()
{
    mpfr_rnd_t rnd = MPFR_RNDN;
    string rawInput;
    getline(cin, rawInput);

    //declarations of the variables. 
    int lenRawInput = rawInput.length();
    mpfr_t lenRawInput1, tempHighValue, tempLowValue, charOccur, interMediate;
    mpfr_init2(lenRawInput1, 8000);
    mpfr_init2(tempHighValue, 8000);
    mpfr_init2(tempLowValue, 8000);
    mpfr_init2(charOccur, 8000);
    mpfr_init2(interMediate, 8000);

    mpfr_set_d(tempHighValue, 1, rnd);
    mpfr_set_d(tempLowValue, 0, rnd);
    mpfr_set_d(lenRawInput1, lenRawInput, rnd);

    char inputCharArray[lenRawInput + 1];
    strcpy(inputCharArray, rawInput.c_str());

    map<int, int> inputMap;
    for (int i = 0; i < sizeof(inputCharArray) - 1 / sizeof(inputCharArray[0]); i++)
    {
        inputMap[inputCharArray[i]]++; // a map to compute the occurance of the characters. 
    }

    map<int, int>::iterator itr;
    for (itr = inputMap.begin(); itr != inputMap.end(); ++itr)
    {
        cout << static_cast<char>(itr->first) << " " << itr->second << '\n';
    }

    lookUpTable probTable[256];

    for (int k = 0; k < 256; k++)
    {
        probTable[k].symbol = k;
        mpfr_init2(probTable[k].probability, 8000);
        mpfr_init2(probTable[k].highValue, 8000);
        mpfr_init2(probTable[k].lowValue, 8000);
    }

    map<int, int>::iterator mapFind;
    for (int k = 0; k < 256; k++)
    {
        mapFind = inputMap.find(k);
        if (mapFind != inputMap.end())
        {
            mpfr_set_d(charOccur, inputMap[k], rnd);
            mpfr_div(probTable[k].probability, charOccur, lenRawInput1, rnd);
            mpfr_add(tempHighValue, tempLowValue, probTable[k].probability, rnd);
            mpfr_set(probTable[k].highValue, tempHighValue, rnd);
            mpfr_set(probTable[k].lowValue, tempLowValue, rnd);
            mpfr_set(tempLowValue, tempHighValue, rnd);
        }
    }

    mpfr_set_d(tempHighValue, 1, rnd);
    mpfr_set_d(tempLowValue, 0, rnd);
    
    //initialization of mpfr variables and building the probability table.
    for (int k = 0; k < lenRawInput; k++)
    {
        mpfr_t code_range, interHigh, interLow;
        mpfr_init2(code_range, 8000);
        mpfr_init2(interHigh, 8000);
        mpfr_init2(interLow, 8000);

        mpfr_sub(code_range, tempHighValue, tempLowValue, MPFR_RNDA);
        mpfr_mul(interHigh, code_range, probTable[rawInput[k]].highValue, MPFR_RNDA);
        mpfr_add(tempHighValue, tempLowValue, interHigh, MPFR_RNDA);
        mpfr_mul(interLow, code_range, probTable[rawInput[k]].lowValue, MPFR_RNDA);
        mpfr_add(tempLowValue, tempLowValue, interLow, MPFR_RNDA);

        mpfr_clears(code_range, interHigh, interLow, (mpfr_ptr)0);
    }

    //calculating the precision.
    mpfr_t one, ten, temp_result;
    int p = 0;
    bool flag = true;
    mpfr_init2(one, 8000);
    mpfr_init2(ten, 8000);
    mpfr_init2(temp_result, 8000);

    mpfr_set_d(one, 1, rnd);
    mpfr_set_d(ten, 10, rnd);
    mpfr_sub(temp_result, tempHighValue, tempLowValue, rnd);

    while (flag)
    {
        mpfr_mul(temp_result, temp_result, ten, rnd);
        p++;
        if (mpfr_cmp(temp_result, one) > 0)
        {
            flag = false;
        }
    }
    mpfr_printf("%.*R*f", p + 1, MPFR_RNDA, tempLowValue);
    cout << " ";
    mpfr_printf("%.*R*f", p + 1, MPFR_RNDZ, tempHighValue);
    
    //releasing the memroy. 
    mpfr_clears(lenRawInput1, tempHighValue, tempLowValue, charOccur, interMediate, one, ten, temp_result, (mpfr_ptr)0);
    for (int k = 0; k < 256; k++)
    {
        mpfr_clear(probTable[k].probability);
        mpfr_clear(probTable[k].highValue);
        mpfr_clear(probTable[k].lowValue);
    }
    mpfr_free_cache();

    return 0;
}
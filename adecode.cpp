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
#include <mpfr.h>
#include <map>
#include <gmp.h>
#include <cstring>

using namespace std;

//Structure to store the Symbol, probability, lowValue and highValue.
struct lookUpTable
{
    int symbol; //input Symbol.
    mpfr_t probability;
    mpfr_t highValue;
    mpfr_t lowValue;
};

int main()
{
    string line, outputString;
    int k = 0;
    int i = 0;
    int calLen = 0;
    string bigstring;
    map<int, int> inputMap;

    while (getline(cin, line)) // read from cin
    {
        if (isspace(line[1]))
        {
            string tempString;
            tempString.push_back(line[2]);
            tempString.push_back(line[3]);
            tempString.push_back(line[4]);
            tempString.push_back(line[5]);
            inputMap.insert(make_pair((int)line[0], stoi(tempString)));
        }
        k++;
    }

    while ((!isspace(line[i])))
    {

        bigstring.push_back(line[i]);
        i++;
        if (line[i] == '\0')
        {
            break;
        }
    }
    char char_array[bigstring.length() + 1];
    strcpy(char_array, bigstring.c_str());

    mpfr_rnd_t rnd = MPFR_RNDN; //variable to round the value to the nearest.
    mpfr_t readValue;
    mpfr_init2(readValue, 8000);
    mpfr_set_d(readValue, 0, rnd);
    mpfr_set_str(readValue, char_array, 10, MPFR_RNDD);

    map<int, int>::iterator itr;

    for (itr = inputMap.begin(); itr != inputMap.end(); ++itr)
    {
        calLen += itr->second;
    }

    //declaraiton of the MPFR variable for the computations
    mpfr_t lenRawInput1, tempHighValue, tempLowValue, interMediate, zeroVar, charOccur;

    mpfr_init2(lenRawInput1, 8000);
    mpfr_init2(tempHighValue, 8000);
    mpfr_init2(tempLowValue, 8000);
    mpfr_init2(charOccur, 8000);
    mpfr_init2(interMediate, 8000);
    mpfr_init2(zeroVar, 8000);

    mpfr_set_d(lenRawInput1, calLen, rnd);
    mpfr_set_d(tempLowValue, 0, rnd);
    mpfr_set_d(tempHighValue, 1, rnd);
    mpfr_set_d(zeroVar, 0, rnd);
    mpfr_set_d(charOccur, 1, rnd);

    lookUpTable probTable[256]; //initializing the table for mpfr datatype.

    for (int k = 0; k < 256; k++)
    {
        probTable[k].symbol = k;
        mpfr_init2(probTable[k].probability, 8000);
        mpfr_init2(probTable[k].highValue, 8000);
        mpfr_init2(probTable[k].lowValue, 8000);

        mpfr_set_d(probTable[k].probability, 0, rnd);
        mpfr_set_d(probTable[k].highValue, 0, rnd);
        mpfr_set_d(probTable[k].lowValue, 0, rnd);
    }

    //building the table for the calculations based on the input.
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

    //--------------------------Actual Decode Calculations------------------//

    while (calLen >= 1)
    {

        for (int i = 0; i < 256; i++)
        {
            if ((mpfr_cmp(zeroVar, probTable[i].probability) != 0))
            {

                if ((mpfr_cmp(readValue, probTable[i].lowValue) >= 0) && (mpfr_cmp(probTable[i].highValue, readValue) > 0))
                {
                    mpfr_t code_range, tempProbLow, tempProbHigh;
                    mpfr_init2(code_range, 8000);
                    mpfr_init2(tempProbLow, 8000);
                    mpfr_init2(tempProbHigh, 8000);
                    mpfr_set(tempProbLow, probTable[i].lowValue, rnd);
                    mpfr_set(tempProbHigh, probTable[i].highValue, rnd);

                    outputString.push_back(probTable[i].symbol);
                    calLen--;
                    mpfr_sub(code_range, tempProbHigh, tempProbLow, rnd);
                    mpfr_sub(readValue, readValue, tempProbLow, rnd);
                    mpfr_div(readValue, readValue, code_range, rnd);
                    mpfr_clears(code_range, tempProbLow, tempProbHigh, (mpfr_ptr)0);
                }
            }
        }
    }
    //reasing the memory used in structure.
    for (int i = 0; i < 256; i++)
    {
        mpfr_clear(probTable[i].probability);
        mpfr_clear(probTable[i].highValue);
        mpfr_clear(probTable[i].lowValue);
    }
    cout << outputString;

    //relasing the mfpr memory.
    mpfr_clears(readValue, lenRawInput1, tempHighValue, tempLowValue, interMediate, zeroVar, charOccur, (mpfr_ptr)0);
    mpfr_mp_memory_cleanup();
    mpfr_free_cache();
}

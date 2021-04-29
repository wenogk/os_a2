#include <iostream>

using namespace std;

//Vegetable types
enum Vegetable
{
    Tomato,
    Onions,
    GreenPepper
};

//Vegetable pair types, basically what Salad makers may want from the Chef
enum VegetablePair
{
    Tomato_GreenPepper,
    Tomato_Onions,
    GreenPepper_Onions
};

//Chefbook struct is going to be stored in the shared memory
struct ChefBook
{

    // weights of veggies currently randomly picked by Chef
    float currentPickedTomatoWeight;
    float currentPickedGreenPepperWeight;
    float currentPickedOnionWeight;

    //array for total number of salads made by each salad maker
    int NumberOfTotalSaladsMadeBySaladMaker[3];

    //array for current start time for each salad maker
    double SaladMakerCurrentStartTime[3];

    //array for current end time for each salad maker
    double SaladMakerCurrentEndTime[3];

    //array for total time waited for each salad maker
    double SaladMakerTotalTimeWaiting[3];

    //array for total time worked for each salad maker
    double SaladMakerTotalTimeWorking[3];

    //array for boolean of whether work is being done for each salad maker
    bool isSaladMakerDoingWork[3];

    //total weight picked for each veggie for salad maker 0
    float totalPickedGreenPepperWeightForSaladMaker0;
    float totalPickedOnionWeightForSaladMaker0;
    float totalSelfPickedTomatoWeightForSaladMaker0;

    //total weight picked for each veggie for salad maker 1
    float totalPickedTomatoWeightForSaladMaker1;
    float totalPickedOnionWeightForSaladMaker1;
    float totalSelfPickedGreenPepperWeightForSaladMaker1;

    //total weight picked for each veggie for salad maker 2
    float totalPickedTomatoWeightForSaladMaker2;
    float totalPickedGreenPepperWeightForSaladMaker2;
    float totalSelfPickedOnionWeightForSaladMaker2;
};

//Salad Maker struct
struct SaladMaker
{
    Vegetable vegetableInfiniteSupply;
    VegetablePair vegetablesNeeded;
};

//Salad Maker 1 with the veggies needed and which infinite supply it has
struct SaladMaker1
{
    Vegetable vegetableInfiniteSupply = Tomato;
    VegetablePair vegetablesNeeded = GreenPepper_Onions;
};

//Salad Maker 2 with the veggies needed and which infinite supply it has
struct SaladMaker2
{
    Vegetable vegetableInfiniteSupply = GreenPepper;
    VegetablePair vegetablesNeeded = Tomato_Onions;
};

//Salad Maker 3 with the veggies needed and which infinite supply it has
struct SaladMaker3
{
    Vegetable vegetableInfiniteSupply = Onions;
    VegetablePair vegetablesNeeded = Tomato_GreenPepper;
};

//function to get the salad maker from an int of 0,1 or 2
SaladMaker *getSaladMakerFromSaladMakerNumber(int saladMakerNumber)
{
    SaladMaker *me = new SaladMaker();
    if (saladMakerNumber == 0)
    {
        me->vegetableInfiniteSupply = Tomato;
        me->vegetablesNeeded = GreenPepper_Onions;
    }
    else if (saladMakerNumber == 1)
    {
        me->vegetableInfiniteSupply = GreenPepper;
        me->vegetablesNeeded = Tomato_Onions;
    }
    else if (saladMakerNumber == 2)
    {
        me->vegetableInfiniteSupply = Onions;
        me->vegetablesNeeded = Tomato_GreenPepper;
    }
    return me;
}

//log the chefbook struct variables
void logChefBook(ChefBook *chefBook)
{
    printf("\tSalad Maker 0 total salads made: %d \n", chefBook->NumberOfTotalSaladsMadeBySaladMaker[0]);
    printf("\tSalad Maker 1 total salads made: %d \n", chefBook->NumberOfTotalSaladsMadeBySaladMaker[1]);
    printf("\tSalad Maker 2 total salads made: %d \n\n", chefBook->NumberOfTotalSaladsMadeBySaladMaker[2]);

    printf("\tSalad Maker 0 total onions %.2fg + green pepper %.2fg + self picked tomatoes %.2fg = %.2fg\n", chefBook->totalPickedOnionWeightForSaladMaker0, chefBook->totalPickedGreenPepperWeightForSaladMaker0, chefBook->totalSelfPickedTomatoWeightForSaladMaker0, chefBook->totalPickedOnionWeightForSaladMaker0 + chefBook->totalPickedGreenPepperWeightForSaladMaker0 + chefBook->totalSelfPickedTomatoWeightForSaladMaker0);
    printf("\tSalad Maker 1 total onions %.2fg + self picked green pepper %.2fg + tomatoes %.2fg = %.2fg\n", chefBook->totalPickedOnionWeightForSaladMaker1, chefBook->totalSelfPickedGreenPepperWeightForSaladMaker1, chefBook->totalPickedTomatoWeightForSaladMaker1, chefBook->totalPickedOnionWeightForSaladMaker1 + chefBook->totalSelfPickedGreenPepperWeightForSaladMaker1 + chefBook->totalPickedTomatoWeightForSaladMaker1);
    printf("\tSalad Maker 2 total self picked onions %.2fg + green pepper %.2fg + tomatoes %.2fg = %.2fg\n\n", chefBook->totalSelfPickedOnionWeightForSaladMaker2, chefBook->totalPickedGreenPepperWeightForSaladMaker2, chefBook->totalPickedTomatoWeightForSaladMaker2, chefBook->totalSelfPickedOnionWeightForSaladMaker2 + chefBook->totalPickedGreenPepperWeightForSaladMaker2 + chefBook->totalPickedTomatoWeightForSaladMaker2);

    printf("\tSalad Maker 0 total time spent waiting: %g seconds \n", chefBook->SaladMakerTotalTimeWaiting[0]);
    printf("\tSalad Maker 1 total time spent waiting: %g seconds \n", chefBook->SaladMakerTotalTimeWaiting[1]);
    printf("\tSalad Maker 2 total time spent waiting: %g seconds \n\n", chefBook->SaladMakerTotalTimeWaiting[2]);

    printf("\tSalad Maker 0 total time spent working on salads: %.2f seconds \n", chefBook->SaladMakerTotalTimeWorking[0]);
    printf("\tSalad Maker 1 total time spent working on salads: %.2f seconds \n", chefBook->SaladMakerTotalTimeWorking[1]);
    printf("\tSalad Maker 2 total time spent working on salads: %.2f seconds \n", chefBook->SaladMakerTotalTimeWorking[2]);
}

//get the vegetable pair needed using the salad maker index
VegetablePair saladMakerNumberToVegetablePairNeeded(int index)
{
    switch (index)
    {
    case 0:
        return GreenPepper_Onions;
    case 1:
        return Tomato_Onions;
    case 2:
        return Tomato_GreenPepper;
    default:
        return Tomato_GreenPepper;
    }
}

//get string to print / log of vegetable pair and their weights
string vegetablePairEnumToNormalStringWithWeights(VegetablePair type, ChefBook *chefBook)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return to_string(chefBook->currentPickedTomatoWeight) + "g Tomato and " + to_string(chefBook->currentPickedGreenPepperWeight) + "g Green Pepper";
    case Tomato_Onions:
        return to_string(chefBook->currentPickedTomatoWeight) + "g Tomato and " + to_string(chefBook->currentPickedOnionWeight) + "g Onions";
    case GreenPepper_Onions:
        return to_string(chefBook->currentPickedGreenPepperWeight) + "g Green Pepper and " + to_string(chefBook->currentPickedOnionWeight) + "g Onions";
    default:
        return "Invalid vegetable pair";
    }
}

//vegetable pair struct to string type
string vegetablePairEnumToNormalString(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "Tomato and Green Pepper";
    case Tomato_Onions:
        return "Tomato and Onions";
    case GreenPepper_Onions:
        return "Green Pepper and Onions";
    default:
        return "Invalid vegetable pair";
    }
}

//vegetable pair to semaphore prefix name
string vegetablePairEnumToSemaphoreName(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/newTomatoGreenPepperPair";
    case Tomato_Onions:
        return "/newTomatoOnions";
    case GreenPepper_Onions:
        return "/newGreenPepperOnions";
    default:
        return "Invalid vegetable pair";
    }
}

//vegetable pair to semaphore prefix name empty semaphore
string vegetablePairEnumToSemaphoreName_Empty(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/newTomatoGreenPepperPairEmpty";
    case Tomato_Onions:
        return "/newTomatoOnionsEmpty";
    case GreenPepper_Onions:
        return "/newGreenPepperOnionsEmpty";
    default:
        return "Invalid vegetable pair";
    }
}

//vegetable pair to semaphore prefix name empty full
string vegetablePairEnumToSemaphoreName_Full(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/newTomatoGreenPepperPairFull";
    case Tomato_Onions:
        return "/newTomatoOnionsFull";
    case GreenPepper_Onions:
        return "/newGreenPepperOnionsFull";
    default:
        return "Invalid vegetable pair";
    }
}

//vegetable pair to semaphore prefix name empty mutex
string vegetablePairEnumToSemaphoreName_Mutex(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/newTomatoGreenPepperPairMutex";
    case Tomato_Onions:
        return "/newTomatoOnionsMutex";
    case GreenPepper_Onions:
        return "/newGreenPepperOnionsMutex";
    default:
        return "Invalid vegetable pair";
    }
}

//vegetable pair to semaphore prefix name empty done
string vegetablePairEnumToSemaphoreName_Done(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/newTomatoGreenPepperPairDone";
    case Tomato_Onions:
        return "/newTomatoOnionsDone";
    case GreenPepper_Onions:
        return "/newGreenPepperOnionsDone";
    default:
        return "Invalid vegetable pair";
    }
}

int randNum(int min, int max)
{ //random number generaor function from https://stackoverflow.com/questions/29381843/generate-random-number-in-range-min-max/29382078
    return rand() % (max - min + 1) + min;
}

double randDouble(int min, int max)
{
    return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}
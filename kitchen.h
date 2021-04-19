#include <iostream>

using namespace std;
enum Vegetable
{
    Tomato,
    Onions,
    GreenPepper
};

enum VegetablePair
{
    Tomato_GreenPepper,
    Tomato_Onions,
    GreenPepper_Onions
};

struct ChefTable
{
    int totalSaladsNeeded;
    int totalSaladsServed;
    VegetablePair pickedVegetables;
    float currentPickedTomatoWeight;
    float currentPickedGreenPepperWeight;
    float currentPickedOnionWeight;
    int NumberOfSaladsMadeBySaladMaker0;
    int NumberOfSaladsMadeBySaladMaker1;
    int NumberOfSaladsMadeBySaladMaker2;
    int SaladMaker0StartTime;
    int SaladMaker1StartTime;
    int SaladMaker2StartTime;
    int SaladMaker0EndTime;
    int SaladMaker1EndTime;
    int SaladMaker2EndTime;
    int SaladMaker0TotalTimeWaiting;
    int SaladMaker1TotalTimeWaiting;
    int SaladMaker2TotalTimeWaiting;
};

struct SaladMaker
{
    Vegetable vegetableInfiniteSupply;
    VegetablePair vegetablesNeeded;
};

struct SaladMaker1
{
    Vegetable vegetableInfiniteSupply = Tomato;
    VegetablePair vegetablesNeeded = GreenPepper_Onions;
};

struct SaladMaker2
{
    Vegetable vegetableInfiniteSupply = GreenPepper;
    VegetablePair vegetablesNeeded = Tomato_Onions;
};

struct SaladMaker3
{
    Vegetable vegetableInfiniteSupply = Onions;
    VegetablePair vegetablesNeeded = Tomato_GreenPepper;
};

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
        return "Invalid animal";
    }
}

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
        return "Invalid animal";
    }
}

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
        return "Invalid animal";
    }
}

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
        return "Invalid animal";
    }
}

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
        return "Invalid animal";
    }
}

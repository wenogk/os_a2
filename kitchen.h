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
        return "/TomatoGreenPepperPair";
    case Tomato_Onions:
        return "/TomatoOnions";
    case GreenPepper_Onions:
        return "/GreenPepperOnions";
    default:
        return "Invalid animal";
    }
}

string vegetablePairEnumToSemaphoreName_Empty(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/TomatoGreenPepperPairEmpty";
    case Tomato_Onions:
        return "/TomatoOnionsEmpty";
    case GreenPepper_Onions:
        return "/GreenPepperOnionsEmpty";
    default:
        return "Invalid animal";
    }
}

string vegetablePairEnumToSemaphoreName_Full(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/TomatoGreenPepperPairFull";
    case Tomato_Onions:
        return "/TomatoOnionsFull";
    case GreenPepper_Onions:
        return "/GreenPepperOnionsFull";
    default:
        return "Invalid animal";
    }
}

string vegetablePairEnumToSemaphoreName_Mutex(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/TomatoGreenPepperPairMutex";
    case Tomato_Onions:
        return "/TomatoOnionsMutex";
    case GreenPepper_Onions:
        return "/GreenPepperOnionsMutex";
    default:
        return "Invalid animal";
    }
}

string vegetablePairEnumToSemaphoreName_Done(VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/TomatoGreenPepperPairDone";
    case Tomato_Onions:
        return "/TomatoOnionsDone";
    case GreenPepper_Onions:
        return "/GreenPepperOnionsDone";
    default:
        return "Invalid animal";
    }
}

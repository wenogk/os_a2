#include "kitchen.h"

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
    enum VegetablePair pickedVegetables;
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
    enum Vegetable vegetableInfiniteSupply;
    enum VegetablePair vegetablesNeeded;
};

struct SaladMaker1
{
    enum Vegetable vegetableInfiniteSupply = Tomato;
    enum VegetablePair vegetablesNeeded = GreenPepper_Onions;
};

struct SaladMaker2
{
    enum Vegetable vegetableInfiniteSupply = GreenPepper;
    enum VegetablePair vegetablesNeeded = Tomato_Onions;
};

struct SaladMaker3
{
    enum Vegetable vegetableInfiniteSupply; // = Onions;
    enum VegetablePair vegetablesNeeded = Tomato_GreenPepper;
};

struct SaladMaker *getSaladMakerFromSaladMakerNumber(int saladMakerNumber)
{
    struct SaladMaker *me = new SaladMaker();
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

string vegetablePairEnumToSemaphoreName(enum VegetablePair type)
{
    switch (type)
    {
    case Tomato_GreenPepper:
        return "/TomatoGreenPepperPair";
    case Tomato_Onions:
        return "/TomatoOnionsPair";
    case GreenPepper_Onions:
        return "/GreenPepperOnionsPair";
    default:
        return "Invalid animal";
    }
}

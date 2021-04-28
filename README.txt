In this assignment, I wrote independent programs that run concurrently, get synchronized at various points, and their collaborative work empowers the operation of the Expo’21 Salad (E21S) production for visitors to the site of the event.

There are 2 ways to run the code: 
    1) By calling the chef which will fork all the salad makers
        - The command is ./chef -n numOfSalads -m cheftime -s eachSaladMakerTime
    2) By calling the chef in "standalone" mode so you would have to manually call the salad makers
        - The command for the chef is ./chef -n numOfSalads -m cheftime
        - The command for each salad maker is 

Regarding the logistics of the salad making process:
    - Salad Maker 0 is the salad maker who has a self supply of Tomato but requires Green Pepper and Onions from the chef
    - Salad Maker 1 is the salad maker who has a self supply of Green Pepper but requires Tomato and Onions from the chef
    - Salad Maker 2 is the salad maker who has a self supply of Onions but requires Green Pepper and Tomato from the chef

File structure
    - Chef.cpp contains chef code and forking code for salad makers
    - timeLogger.cpp contains code that logs the times two or more salad makers are working at the same time
    - saladMaker.cpp contains the salad maker code
    - kitchen.h contains the structs used for shared memory and some shared functions

Running details
    - Timeline operation log of each saladmaker program and the chef are appended to a file called "logFile.txt" in the same directory
    - The logs of the times two or more salad makers were working at the same time are appended to a file called "intersectingTimeLogger.txt" and these results are also printed out in the Chef program
    - After all the salads have been made the Chef will display the required statistics such as 
        - Number of salads produced
        - Sum weight of each ingredient by every saladmaker
        - Sum weight of all ingredients for every saladmaker (extra work)
        - Total time each saladmaker spent on salads 
        - Total time every saladmaker waits for ingredients’ pairs to be delivered
        - Listings of time periods where 2 or more saladmakers were busy at the same time

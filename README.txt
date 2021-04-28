In this assignment, I wrote independent programs that run concurrently, get synchronized at various points, and their collaborative work empowers the operation of the Expo’21 Salad (E21S) production for visitors to the site of the event.

There are 2 ways to run the code: 
    1) By calling the chef which will fork all the salad makers
        - The command is ./chef -n numOfSalads -m cheftime -s eachSaladMakerTime
        - the s argument is optional, it specifies the time for making the salad for each salad maker
    2) By calling the chef in "standalone" mode so you would have to manually call the salad makers
        - The command for the chef is ./chef -n numOfSalads -m cheftime
        - Thhe command for each salad
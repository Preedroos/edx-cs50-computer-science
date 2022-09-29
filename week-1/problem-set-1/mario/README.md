# In Mario, my task was to re-implement some pyramids from the game Super Mario Brothers in code in a C program.

* *The program we’ll write will be called mario. And let’s allow the user to decide just how tall the pyramids should be by first prompting them for a positive integer between, say, 1 and 8, inclusive.*

## Here’s how the program might work if the user inputs 4 when prompted:

```
$ ./mario
Height: 4
   #  #
  ##  ##
 ###  ###
####  ####
```

## If the user doesn’t, in fact, input a positive integer between 1 and 8, inclusive, when prompted, the program should re-prompt the user until they cooperate:

```
$ ./mario
Height: -1
Height: 0
Height: 42
Height: 50
Height: 4
   #  #
  ##  ##
 ###  ###
####  ####
```

For more details, see the [PROBLEM SET 1 - MARIO](https://cs50.harvard.edu/x/2022/psets/1/mario/more/) reference.
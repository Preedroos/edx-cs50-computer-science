# In this program, my task was to write a program that implements the Tideman Voting System. This voting system is an example of a ranked-preference voting system, which means that every voter, instead of just voting for one candidate, will get to rank their candidates in order of preference.

## For this program, I've implemented a program that runs a Tideman election, per the below.

```
$ ./tideman Alice Bob Charlie
Number of voters: 5
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Charlie
```

For more details, see the [PROBLEM SET 3 - TIDEMAN](https://cs50.harvard.edu/x/2022/psets/3/tideman/) reference.
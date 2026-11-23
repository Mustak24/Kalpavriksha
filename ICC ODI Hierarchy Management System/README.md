# ICC ODI Player Performance Analyzer

## Objective

- Design and implement a data management and analysis system for ICC ODI player statistics that organizes and processes data using Searching, Sorting and Linked list concepts.
- The system should allow storing, searching, sorting, and merging team and player data for analytical insights.

## `Players_data.h`

- Provides predefined data of 200 players of 10 teams.
- Includes an array of all 10 team names.
- Can be included in the program to load initial player and team data.
- A separate `Player` object must be defined according to the data model below, independent of the `Player` structure defined in this header file.

## Data Model

### Player

Each player has:

- `PlayerId`
- `Name`
- `TeamName`
- `Role` (Batsman, Bowler, All-rounder)
- `TotalRuns`
- `BattingAverage`
- `StrikeRate`
- `Wickets`
- `EconomyRate`
- `PerformanceIndex`

### Team

Each team has:

- `TeamId`
- `Name`
- `TotalPlayers`
- `AverageBattingStrikerate`

**Note:** To calculate `AverageBattingStrikerate`, consider the strike rate of batters and all-rounders only.

## Performance Index

Each player has a performance index according to their role:

- **Batsman:** `(BattingAverage × StrikeRate) / 100`
- **Bowler:** `(Wickets × 2) + (100 − EconomyRate)`
- **All-Rounder:** `[(BattingAverage × StrikeRate) / 100] + (Wickets × 2)`

**Note:** Performance indexes for different roles are not directly comparable. For example, if Batsman X has a higher performance index than Bowler Y, it does not necessarily mean X's performance is better than Y's.

## Functional Requirements

Create a menu-driven program to simulate ICC's internal system.

When the program starts, initialize players and teams using the given header file. Then provide these features:

1. Allow the user to add new players to teams using team ID.
2. Allow the user to display all players of a specific team, along with the total number of players and average batting strike rate.
3. Allow the user to display teams according to the average batting strike rate of batters + all-rounders of that team, in descending order.
4. Allow the user to display the top K players of a specific team according to their performance index and given role.
   - For example, if the input role is Batsman, display the top K batsmen of that specific team according to their performance index in descending order.
   - The same applies to Bowlers and All-rounders.
5. Allow the user to display players of all teams of a specific role according to their performance index in descending order.
   - For example, if the input role is Bowler, display all Bowlers of all teams according to their performance index in descending order.
   - The same applies to Batsmen and All-rounders.

## Performance Requirements

- After initialization, team searching should be done in `O(log n)` time when required.
- Requirement 4 should be done in `O(K)` time.
- Requirement 5 should be done in `O(N log t)` time, where `t` is the number of teams and `N` is the total number of players of that role. An extra list can be created.

## Constraints

- Number of teams = 10
- `1 <= PlayerId/TeamId <= 1000`
- `11 <= Players per team <= 50`
- `1 <= Name length <= 50`

## Example

**Note:** The purpose of the data displayed in the example is only to explain each requirement. It may not align with the data given in the header file.

### Menu

```text
==============================================================================
ICC ODI Player Performance Analyzer
==============================================================================
1. Add Player to Team
2. Display Players of a Specific Team
3. Display Teams by Average Batting Strike Rate
4. Display Top K Players of a Specific Team by Role
5. Display all Players of specific role Across All Teams by performance index
6. Exit
==============================================================================
Enter your choice:
```

### Choice 1 — Add Player to Team

```text
Enter Team ID to add player: 1
Enter Player Details:
Player ID: 121
Name: Yuzvendra Chahal
Role (1-Batsman, 2-Bowler, 3-All-rounder): 2
Total Runs: 50
Batting Average: 8.5
Strike Rate: 70.0
Wickets: 180
Economy Rate: 4.6

Player added successfully to Team India!
```

### Choice 2 — Display All Players of a Specific Team

```text
Enter Team ID: 1
Players of Team India:
====================================================================================
ID Name Role Runs Avg SR Wkts ER Perf.Index
====================================================================================
101 Rohit Sharma Batsman 9800 48.3 94.5 0 0.0 45.64
102 Virat Kohli Batsman 13000 57.3 93.2 0 0.0 53.40
103 Jasprit Bumrah Bowler 120 7.8 70.0 145 4.6 236.40
104 Hardik Pandya All-Rounder 2300 36.5 121.0 80 5.2 242.65
====================================================================================
Total Players: 4
Average Batting Strike Rate: 94.68
```

### Choice 3 — Display Teams by Average Batting Strike Rate

```text
Teams Sorted by Average Batting Strike Rate
=========================================================
ID Team Name Avg Bat SR Total Players
=========================================================
1 India 102.9 4
2 Australia 99.4 4
3 England 97.6 4
4 Pakistan 95.3 4
5 New Zealand 93.2 4
6 South Africa 91.8 4
7 Bangladesh 90.7 4
8 Sri Lanka 89.5 4
9 Afghanistan 88.9 4
10 West Indies 87.4 4
=========================================================
```

### Choice 4 — Display Top K Players of a Specific Team by Role

```text
Enter Team ID: 1
Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): 1
Enter number of players: 2

Top 2 Batsmen of Team India:
====================================================================================
ID Name Role Runs Avg SR Wkts ER Perf.Index
====================================================================================
102 Virat Kohli Batsman 13000 57.3 93.2 0 0.0 53.40
101 Rohit Sharma Batsman 9800 48.3 94.5 0 0.0 45.64
```

### Choice 5 — Display All Players Across All Teams of a Specific Role

```text
Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): 3
All-rounders of all teams:
======================================================================================
ID Name Team Role Runs Avg SR Wkts ER Perf.Index
======================================================================================
301 Jasprit Bumrah India Bowler 190 12.2 80.1 230 4.6 330.4
202 Mitchell Starc Australia Bowler 210 10.5 84.2 240 4.7 335.3
204 Pat Cummins Australia Bowler 180 11.8 75.5 215 4.9 325.1
402 Jofra Archer England Bowler 160 11.0 75.0 205 4.9 325.1
405 Chris Woakes England Bowler 230 14.1 92.4 198 5.1 322.9
502 Trent Boult NewZealand Bowler 180 10.8 82.3 200 5.0 320.0
602 Shaheen Afridi Pakistan Bowler 130 9.5 70.0 190 4.8 315.2
702 Kagiso Rabada SouthAfrica Bowler 170 11.4 79.8 185 5.2 314.8
704 Anrich Nortje SouthAfrica Bowler 120 8.9 68.4 178 5.0 311.0
802 Alzarri Joseph WestIndies Bowler 150 9.8 72.1 180 5.0 310.0
902 Dushmantha Chameera SriLanka Bowler 110 8.5 65.3 175 5.1 304.9
904 Lahiru Kumara SriLanka Bowler 90 7.6 59.2 168 5.3 302.7
1002 Taskin Ahmed Bangladesh Bowler 140 9.9 68.2 172 5.3 303.7
1102 Rashid Khan Afghanistan Bowler 280 14.5 95.0 170 4.2 301.8
1103 Mujeeb Ur Rahman Afghanistan Bowler 100 8.1 60.0 162 4.9 297.1
======================================================================================
```

## Guidelines

### GitHub Repository Setup

- Create a public GitHub repository (accessible by mentors) for submitting Kalpavriksha assignments throughout the program.
- Create a separate branch for assignment submission and raise a Pull Request (PR) for this branch.

### Submission Deadline

- The assignments should be done in C language.
- The final submission deadline is **23rd November, 12:00 PM**.
- Late submissions will not be accepted under any circumstances.
- Any commits made to the repository after **23rd November, 12:00 PM** will be disregarded.

### Prohibited AI Usage

- The use of AI or automated tools to complete the assignment is strictly prohibited.
- Assignments should be completed independently.

### Assignment Review and Compliance

- Mentors will thoroughly review each assignment submission.
**# Assignment 2: CRUD Operations in File using C Language**

**### Problem Statement**

Write a C program to perform CRUD (Create, Read, Update, Delete) operations on user records stored in a text file named `users.txt`.

Each user record should contain the following fields:

* Unique ID
* Name
* Age

The program should allow the user to add new records, display existing records, update a user's details using their ID, and delete a user record from the file.

**### Requirements**

* Store user data in a structured format using a C `struct`.
* Use a text file named `users.txt` to store the records.
* Create the `users.txt` file if it does not already exist.
* Implement the following CRUD operations:

  * **Create:** Add a new user record to the file.
  * **Read:** Read and display all user records from the file.
  * **Update:** Modify the details of a specific user based on their ID.
  * **Delete:** Remove a user record based on their ID.
* Each user should have a unique ID.
* Use file handling functions such as `fopen()`, `fprintf()`, `fscanf()`, and `fclose()`.
* For updating and deleting records, modify the file content using a temporary file and replace the original file after the operation.
* Display an appropriate message if the requested user ID is not found.

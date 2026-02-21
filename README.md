# Bank Management System (C++)

A console-based bank management system written in C++.
This project focuses on practicing core programming concepts such as file handling, data structures, and menu-driven applications.

## Features
- Add new clients
- Update client information
- Delete clients
- Search for a client by account number
- Deposit and withdraw funds
- Display total balances
- Persistent data storage using text files
  
## Version 2 Update – Authentication & Permissions

This version introduces a basic authentication system and role-based access control.

### New Additions
- User login system (`Users.txt`)
- Role-based permission management
- Restricted access to specific menu operations
- Improved program flow with authentication check before system access

This update reflects a transition from a simple CRUD-based system to a more structured, permission-aware system design.
## Technologies Used
- C++
- File I/O (fstream)
- STL (vector, string)
- Console-based UI

## Data Storage
- Client data is stored in `Clients.txt`
- User data and roles are stored in `Users.txt`
- Custom delimiter-based format is used for parsing records

## Purpose
This project was built as part of my learning journey in C++.
It aims to strengthen understanding of:
- Structs and functions
- Separation of logic
- File-based data persistence
- Program flow using menus

## Notes
- This is not intended to be a production-ready system.
- The focus is on learning and applying fundamental C++ concepts.

## Future Improvements
- Refactor using Object-Oriented Programming (OOP)
- Implement password hashing for better security
- Improve modular structure (separate user & client management)
- Add transaction history logging
- Replace file storage with a database system

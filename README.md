Implemented in just one C++ file.

There are three classes

   1. Account : contain account information(account number, pin number, balance) and interfaces
   
   2. Bank: contain two map data(one is for pin number - accounts; the other for account number to account pointer) and interfaces
   
   3. ATM: contain bank pointer and several functions to interact with users


In main function, 1000 accounts and 600 pin numbers are generated. Several account can be assigned to the same pin number.

Pin number is in the range of 10000 ~ 10599. Each account has initial balance of 0~1000000 dollars


Compile: g++ -o simple_atm_controller simple_atm_controller.cpp

Run: simple_atm_controller.exe (Windows)

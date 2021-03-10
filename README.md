# SIMPLE INVENTORY MANAGER (SIM)

### Version 0.1.0
#### The Alpha Stage

***This is an early alpha version of SIM, a simple open-source inventory management program. Most features are up and running, but some critical features are currently half implemented. (For example: The search function only works on half of the tables, and there is no sample database for a user to see how this thing actually works). So why is this on Github? I'm about to restructure and simplify almost all of the codebase, and I would like to keep a copy of where the project was at before somewhere.***

I don't expect this project to get any traction right now, but if anyone does happen to see this, come back at the end of March 2021. That's the **projected release date of v1.0.0**


## WHAT IS SIM?
Everything SIM will be able to do is seen in its name.
#### SIMPLE
SIM is meant to be simple and intuitive to operate, use, debug, etc. When version 1.0.0 is released, all operations will take place in one main window, and you will be able to freely navigate to any value you see on the screen just by clicking on it. Minimal RAM usage. Minimal bloat.
#### INVENTORY MANAGER
SIM is not meant to be anything more than an inventory manager. It is not meant to replace fully fledged solutions, because it is not designed to do everything those inventory solutions do. Imagine this as a ledger for your inventory and where it goes.

## WHY DOES SIM EXIST?
During a search for an inventory management system, almost every system I found did too much. They ended up being overly complicated and confusing for new users. SIM is designed around minimalism and simplicity. You download copy the appropriate files to your system and it just works.

## WHAT CAN SIM DO?
SIM is an inventory manager that fundamentally operates on a very simple, traceable level:
- Items can be **RECEIVED** into the inventory
- Items can be **DISTRIBUTED** out of the inventory

These two actions are referred to as transactions, and every transaction is logged with a timestamp, a number, and a name of who performed the transaction.

Every other action that a user can perform simply supplements these primary transactions. The other actions follow:
#### ITEM REQUESTS:
A user can request that an item be purchased. 
#### PURCHASE ORDERS:
That users request can then be turned into a purchase order by the purchasing department, or whoever is in charge of ordering new items

*It is possible that items may need to be RETURNED, but this is not implemented yet.*

## HOW DOES SIM WORK?
SIM uses:
- SQLite3 to manage the Inventory Database
- Qt6 to handle the GUI on 64-bit systems
- Qt5 to handle the GUI on 32-bit systems
*- A Python script currently handles the conversion from the old Inventory software I was using, Inventoria, to the new one. Most of SIM is written in C and C++, with a focus on speed. Since the python script only gets executed one time ever, and is specifically tailored to our needs, it's not part of SIM. The reason this disclaimer exists is because it's possible I'll forget to remove it before v1.0.0 is released.

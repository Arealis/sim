# SIMPLE INVENTORY MANAGER (SIM)

### Version 0.2.0
#### The Alpha Stage

***This is an early alpha version of SIM, an open-source Simple Inventory Management program. If anybody happens to have interest in this project. Come back to this page at the end of March. That is the projected release date of SIM 1.0***

#### Notes on v0.2.0
With v0.2.0, the entirety of SIM has been redesigned, both visually and on the back end. It is still not in a functional state, but users can now create drafts of PRs within SIM. This may not seem like much, but it is all that is necessary to demonstrate the core functionality of SIM. It's almost functional, and I can choose to celebrate that.

## WHAT IS SIM?
SIM is an inventory manager designed for manufacturing companies. It is a...  
#### SIMPLE
SIM is meant to be simple and intuitive to install, operate and debug. It is meant to have a small size footprint, a small memory footprit, and be light on resources. Ideally (if I've achieved what I set out to do) you should be able to fully understand everything about SIM within 5 minutes of using it.
#### INVENTORY MANAGER
SIM is not meant to be anything more than an inventory manager. This means is not meant to replace fully fledged solutions, because it is not designed to do everything that those programs do. SIM does the essentials. It manages inventory.

## WHY DOES SIM EXIST?
During a search for an inventory management system a few years ago, almost every system I found didn't fit our company's needs. They were much more than inventory managers, they managed hundreds of other features as well, some of them even had their own accounting suites!. As a result, even though these systems were fantastic, it was overkill. It was too complicated for new users to understand and gave us options that we never really planeed to use (We had our own accounting suite). SIM is designed as a simple and minimal alternative.

## How can I download SIM?
Download a copy of the folder with the latest build in it and you're good to go.
The latest build right now is v0.2.0

## WHAT CAN SIM DO?
SIM was designed to mirror the ideal procurement process that should happen in a business. As a result there are 5 main operations:
- (PRs) **PURCHASE REQUISITIONS**  
- (QRs) **QUOTATION REQUESTS** *(also known as Requests for Quotation)
- (POs) **PURCHASE ORDERS**
- (RRs) **RECEIVING REPORTS**
- (MRs) **MATERIAL REQUISITIONS**
- 
When any document is created, it is logged with a timestamp, a unique number, and the name of the person who created (or authorized) the document.
A user (depending on their privelages) can create or view any one of these documents. Or they can see an overview of these documents specified by a certain date range.
*Further information about how this works can be found in the 'How Does SIM Work' section*

## HOW DOES SIM WORK?
[This is probably the most important part of the documentation and I haven't written this yet. I'll get around to it soon.]

## Yes, but how does SIM work technically?
SIM uses:
- SQLite3 to manage the Inventory Database
- Qt6 to handle the GUI on 64-bit systems
- Qt5 to handle the GUI on 32-bit systems
- C++ tying all of that together

### Distance to release
The following features have to be implemented before I consider SIM 1.0 to be ready for release and use. 
- All 5 documents can be either **saved as draft**, **saved as final**, and **updated**
- User privelages actually work<sup>[1]</sup> (ie. The actions a user can do and the tables they can view are authorized by their privelage flag)
- Any table that is available in the main SIM window is printable.
- The ability to return items
- *It is possible that items may need to be LOANED out and RETURNED, but I haven't decided on implementing this yet*
- 
[1] User privelages are almost pointless currently. I also expect they will be pointless almost up until the final point of release. This is because while SIM is designed with security in mind, I am writing SIM into a location where security by obscurity is a viable option. Or rather, security by ignorance. SQLite by default is not an encrypted or locked database, so anyone who is technically minded enough to find this GitHub page is also probably technically minded enough to learn some SQL and destroy the illusion of security that SIM has. This will be changed before SIM 1.0, but this will probably be the last thing to change 

### Post Release and LTS
I am designing SIM to be a piece of software that can be deployed and rarely will need an update. That being said, post 1.0 release when the core functionality of SIM is locked down, there will still be a few updates regarding the following categories
- Bugfixing. I do not expect there will be many, given that SIM is not much more than a touched up database browser.
- A web based browser to accompany SIM. It would be fantastic to be able to access SIM from anywhere you go so that any user can be updated on the status of their inventory from anywhere. It's not an absolute neccessity for release, but this will be the first thing that I work on after the 1.0 release.
- Porting SIM to mySQL (or another DBMS). For someone running SIM on a single computer, SQLite is fine. Even for somewhere where you host a few people on a server within a shared network, SQLite should manage, but for more than that, while I believe in most cases SQLite should still be able to manage, there is always a chance that things could fail in the unlikely event that two people try to access the database at the same time. Given enough time and people, this highly unlikely chance is 100% guaranteed to happen, and it WILL happen at the worst possible moment. In order to ensure that the database behind SIM has the power to support multiple connections, after release it will be ported to a different DBMS.
- Aesthetics. SIM will probably release looking like shareware from 1993. A nice way to put it is that it was designed with function in mind, and the form followed suite. while functionality is imporant, the aesthetics of a program can greatly affect how you percieve it and use it. Following release I would like to make SIM much more sleek.

That's it, enjoy.

***SIM - Simple Inventory Manager --- an open source project by Arealis AKA Nathan Williams***

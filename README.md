========================================================================
    CONSOLE APPLICATION : Bank Project Overview
========================================================================

This file contains a summary of what you will find in each of the files that
make up your Bank application.


Bank.cpp
    This is the main application source file.

Make sure you have the following compiled with it:
	- client.h
	- client.cpp
	- Manager.cpp
	- Manager.h
	- Mperson.h
	- Mperson.cpp

Make sure the following text files are in the directory:
	- client_purchases.txt
	- clients.txt
	- execution_traces.txt
	- externals.txt
	- failed_payments.txt
	- vendor.txt

Format for the following text files:
/////////////////////////////////////////////////////////////////////////////
clients.txt
	[ID] [chequing account balance] [sav act balance] [1st name] [2nd name] [PIN] ["Frozen" or "ok"] [credit limit] ["part" or "full"] [debt]
	...
/////////////////////////////////////////////////////////////////////////////
client_purchases.txt
	[ID] [purchase #1] [purchase #2] ... [last purchase]
	...
/////////////////////////////////////////////////////////////////////////////
externals.txt
	"MaintainPerson" [Mperson ID]
	"Manager" [Manager ID] 	
	"EndOfMonth" [# of days until end of month]
/////////////////////////////////////////////////////////////////////////////
failed_payments.txt
	[ID] [ammount failed to pay]
	...
/////////////////////////////////////////////////////////////////////////////
vendor.txt
	[ID] [Purchase ammount]
	...
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

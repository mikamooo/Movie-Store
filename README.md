# Movie-Store

This is a shopping system for purchasing movies that will function as an online store in which users must create/log into their account in order to purchase a movie from the store’s catalog. The system is backed by PostgresSql and fronted by C++ programs that connect to the database to perform queries. The user interface is text-based, and users will interact by responding to given prompts in the terminal.

To compile, use the command: ***make all***

To populate the tables, use the command: ***./populate***

To test the store, use the command: ***./driver***

If you would like to reset the store to its initial state, run populate again. Otherwise only run driver to save any changes.

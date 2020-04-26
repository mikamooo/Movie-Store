#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main()
{
    string db, user, pass, connect;

    // Get the info for the database to connect to
    cout << "Enter the name of the database: "; 
    cin >> db;
    cout << "Enter the username: "; 
    cin >> user;
    cout << "Enter the password: "; 
    cin >> pass;

    connect = "dbname = " + db + " user = " + user + " password = " + pass + " hostaddr = 127.0.0.1 port = 5432";

    try // Try connecting to the database
    {
        connection C(connect);

        if (C.is_open())
        {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else
        {
            cout << "Can't open database" << endl;
            return 1;
        }

        // Create SQL statements for creating the tables and creating a guest user
        string tables = "DROP TABLE IF EXISTS Customers CASCADE;"
                        "CREATE TABLE Customers("  
                            "CID INTEGER,"
                            "Email VARCHAR(50),"
                            "Password VARCHAR(20),"
                            "CName VARCHAR(50),"
                            "Created DATE,"
                            "Address VARCHAR(100),"
                            "DOB DATE,"
                            "PRIMARY KEY(CID));"

                        "DROP TABLE IF EXISTS Movies CASCADE;"
                        "CREATE TABLE Movies("
                            "MID INTEGER,"
                            "Title VARCHAR(60),"
                            "Genre VARCHAR(20),"
                            "Year DATE,"
                            "Qty INTEGER CHECK(Qty >= 0),"
                            "Price NUMERIC(4,2) CHECK(Price >= 0),"
                            "Rating CHAR(5),"
                            "Des VARCHAR(600),"
                            "PRIMARY KEY(MID));"

                        "DROP TABLE IF EXISTS Orders CASCADE;"
                        "CREATE TABLE Orders("
                            "OID INTEGER,"
                            "CID INTEGER,"
                            "Received DATE,"
                            "Shipped DATE,"
                            "Address VARCHAR(100),"
                            "Status CHAR(15) CHECK(Status IN ('In Progress', 'Shipped', 'Delivered')),"
                            "PRIMARY KEY(OID),"
                            "CONSTRAINT order_fk FOREIGN KEY(CID) REFERENCES Customers);"

                        "DROP TABLE IF EXISTS Cart CASCADE;"
                        "CREATE TABLE Cart("
                            "CID INTEGER,"
                            "MID INTEGER,"
                            "Qty INTEGER,"
                            "OID INTEGER DEFAULT -1,"
                            "Purchased NUMERIC(4,2) DEFAULT NULL CHECK(Purchased >= 0),"
                            "CONSTRAINT cart_pk PRIMARY KEY(CID, MID,OID),"
                            "CONSTRAINT cart_fk1 FOREIGN KEY(CID) REFERENCES Customers,"
                            "CONSTRAINT cart_fk2 FOREIGN KEY(MID) REFERENCES Movies);"

                        "DROP TABLE IF EXISTS Admins CASCADE;"
                        "CREATE TABLE Admins("
                            "AID INTEGER,"
                            "Email VARCHAR(50),"
                            "Password VARCHAR(20),"
                            "AName VARCHAR(50),"
                            "DOB DATE,"
                            "PRIMARY KEY(AID));"
                            
                        "DROP USER IF EXISTS movie_guest;"
                        "CREATE USER movie_guest WITH PASSWORD 'password';"
                        "GRANT SELECT ON TABLE Movies TO movie_guest;"
                        "GRANT INSERT ON TABLE Customers TO movie_guest;"
                        
                        "DROP USER IF EXISTS movie_customer;"
                        "CREATE USER movie_customer WITH PASSWORD 'password';"
                        "GRANT SELECT,UPDATE ON TABLE Movies TO movie_customer;"
                        "GRANT SELECT, INSERT ON TABLE Orders TO movie_customer;"
                        "GRANT SELECT, UPDATE ON TABLE Customers TO movie_customer;"
                        "GRANT SELECT, INSERT, UPDATE,DELETE ON TABLE Cart TO movie_customer;"
                        
                        "DROP USER IF EXISTS movie_admin;"
                        "CREATE USER movie_admin WITH PASSWORD 'admin123';"
                        "GRANT SELECT, UPDATE, DELETE, INSERT ON TABLE Movies TO movie_admin;"
                        "GRANT SELECT, UPDATE, INSERT ON TABLE Admins TO movie_admin";

        work W1(C); // Create a transactional object
        W1.exec(tables);
        W1.commit();

        // Create SQL statements to insert information into the customers table
        string insert = "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (101, 'johndoe@usf.edu', 'password', 'John Doe', '2018-08-15'," 
                                "'123 Web Lane Tampa, FL 33612', '1995-01-02');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (102, 'janedoe@gmail.com', '123abc', 'Jane Doe', '2019-02-04'," 
                                "'456 Holly Ave Portland, OR 17892', '2000-12-17');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (103, 'ally17@hotmail.com', 'turtles', 'Ally Smith', '2020-01-23'," 
                                "'789 4th St Charleston, SC 53948', '2007-03-24');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (104, 'spiderpeter@gmail.com', 'spiderman', 'Peter Parker', '2018-05-18'," 
                                "'1567 Avenger Ave Queens, NY 73290', '2001-08-10');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (105, 'bestboss@outlook.com', 'office7', 'Michael Scott', '2009-10-21'," 
                                "'9518 Dunder St Scranton, PA 18503', '1965-03-15');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (106, 'schrutefarms@outlook.com', 'Beets50!*?', 'Dwight Schrute', '2010-10-22'," 
                                "'6789 Schrute Lane Scranton, PA 18503', '1966-01-20');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (107, 'waltwhite@hotmail.com', 'H3is3nb3rg!', 'Walter White', '2008-04-03'," 
                                "'417 12th St NW Albuquerque, NM 87101', '1958-09-07');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (108, 'jessep@gmail.com', 'B**CH*!', 'Jesse Pinkman', '2011-02-18'," 
                                "'3958 28th St SW Albuquerque, NM 87103', '1984-09-24');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (109, 'dextermorgan@gmail.com', 'r3drum', 'Dexter Morgan', '2012-03-29'," 
                                "'193 Sunset Blvd Miami, FL 33101', '1971-02-01');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (110, 'queenB@gmail.com', 'BWl0vesCB!%', 'Blair Waldorf', '2014-07-16'," 
                                "'2037 Madison Ave Manhattan, NY 10021', '1990-11-15');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (111, 'bryanl0we@live.com', 'sG@!mq1Na4x*', 'Bryan Lowe', '2015-11-30'," 
                                "'4031 Pine St Seattle, WA 98101', '2003-05-08');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (112, 'ktjohnson@usf.edu', 'p@$$w0rd!', 'Katie Johnson', '2017-09-25'," 
                                "'526 Grace St Tampa, FL 33201', '1999-12-31');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (113, 'chloeXlynnXr@gmail.com', 'PtvSwsBmth!', 'Chloe Rhodes', '2019-10-07'," 
                                "'1895 Cactus Ave Phoenix, AZ 85009', '2006-06-19');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (114, 'mrsteve@outlook.com', 'il0veb0at$', 'Steve Richard', '2014-08-11'," 
                                "'710 James St Dallas, TX 75063', '1971-01-23');"
                        "INSERT INTO CUSTOMERS(CID, Email, Password, CName, Created, Address, DOB)"
                        "VALUES (115, 'lissaw@hotmail.com', 'm3li$$@', 'Melissa Wallace', '2017-12-13'," 
                                "'9471 Waters Ave Chicago, IL 60624', '1993-03-21');";

        work W2(C); // Create a transactional object
        W2.exec(insert);
        W2.commit();

        // Create SQL statements to insert information into the movies table
        insert = "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1001, 'The Gold Rush', 'Comedy', '1925-06-26', 103, 21.88, 'NR',"
                    "'A prospector goes to the Klondike in search of gold and finds it and more.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1002, 'Sunrise', 'Drama', '1927-11-04', 126, 28.79, 'NR',"
                    "'An allegorical tale about a man fighting the good and evil within him. Both sides "
                    "are made flesh - one a sophisticated woman he is attracted to and the other his wife.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1003, 'Modern Times', 'Comedy', '1936-02-25', 97, 19.50, 'G',"
                    "'The Tramp struggles to live in modern industrial society with the help of a young homeless woman.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1004, 'It''s a Wonderful Life', 'Drama', '1947-01-07', 84, 14.96, 'PG',"
                    "'An angel is sent from Heaven to help a desperately frustrated businessman by showing him what "
                    "life would have been like if he had never existed.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1005, '12 Angry Men', 'Drama', '1957-04-10', 91, 10.80, 'NR',"
                    "'A jury holdout attempts to prevent a miscarriage of justice by forcing his colleagues to reconsider the evidence.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1006, 'Psycho', 'Horror', '1960-09-08', 52, 14.99, 'R',"
                    "'A Phoenix secretary embezzles forty thousand dollars from her employer''s client, goes on the run, and checks into " 
                    "a remote motel run by a young man under the domination of his mother.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1007, 'To Kill a Mockingbird', 'Crime', '1962-03-16', 67, 8.39, 'NR',"
                    "'Atticus Finch, a lawyer in the Depression-era South, defends a black man against an undeserved rape charge, and his " 
                    "children against prejudice.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1008, 'A Clockwork Orange', 'Crime', '1972-02-02', 48, 12.77, 'R',"
                    "'In the future, a sadistic gang leader is imprisoned and volunteers for a conduct-aversion experiment, but it doesn''t go as planned.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1009, 'The Exorcist', 'Horror', '1973-12-26', 51, 9.48, 'R',"
                    "'When a 12 year-old girl is possessed by a mysterious entity, her mother seeks the help of two priests to save her.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1010, 'Taxi Driver', 'Crime', '1976-02-09', 39, 10.01, 'R',"
                    "'A mentally unstable veteran works as a nighttime taxi driver in New York City, where the perceived decadence and " 
                    "sleaze fuels his urge for violent action by attempting to liberate a presidential campaign worker and an underage prostitute.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1011, 'Star Wars: Episode IV - A New Hope', 'Sci-Fi', '1977-05-25', 15, 19.07, 'PG',"
                    "'Luke Skywalker joins forces with a Jedi Knight, a cocky pilot, a Wookiee and two droids to save the galaxy from the Empire''s " 
                    "world-destroying battle station, while also attempting to rescue Princess Leia from the mysterious Darth Vader.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1012, 'The Shining', 'Drama', '1980-06-13', 47, 15.99, 'R',"
                    "'A family heads to an isolated hotel for the winter where a sinister presence influences the father into violence, "
                    "while his psychic son sees horrific forebodings from both past and future.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1013, 'Back to the Future', 'Sci-Fi', '1985-07-03', 71, 12.99, 'PG',"
                    "'Marty McFly, a 17-year-old high school student, is accidentally sent thirty years into the past in a time-traveling " 
                    "DeLorean invented by his close friend, the eccentric scientist Doc Brown.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1014, 'My Neighbor Totoro', 'Animation', '1990-07-13', 86, 14.99, 'G',"
                    "'When two girls move to the country to be near their ailing mother, they have adventures with the wondrous forest spirits who live nearby.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1015, 'Forrest Gump', 'Drama', '1994-07-06', 54, 15.99, 'PG-13',"
                    "'The presidencies of Kennedy and Johnson, the events of Vietnam, Watergate, and other historical events unfold through the perspective of " 
                    "an Alabama man with an IQ of 75, whose only desire is to be reunited with his childhood sweetheart.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1016, 'The Matrix', 'Drama', '1999-03-31', 26, 19.99, 'R',"
                    "'A computer hacker learns from mysterious rebels about the true nature of his reality and his role in the war against its controllers.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1017, 'The Sixth Sense', 'Drama', '1999-08-06', 61, 9.99, 'PG-13',"
                    "'A boy who communicates with spirits seeks the help of a disheartened child psychologist.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1018, 'The Lord of the Rings: The Fellowship of the Ring', 'Fantasy', '2001-12-19', 19, 15.99, 'PG-13',"
                    "'A meek Hobbit from the Shire and eight companions set out on a journey to destroy the powerful One Ring and save Middle-earth from the Dark Lord Sauron.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1019, 'Spirited Away', 'Animation', '2003-03-28', 74, 14.99, 'PG',"
                    "'During her family''s move to the suburbs, a sullen 10-year-old girl wanders into a world ruled by gods, witches, and spirits, and where humans are changed into beasts.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1020, 'Kill Bill: Vol 1', 'Action', '2003-10-10', 82, 9.50, 'R',"
                    "'After awakening from a four-year coma, a former assassin wreaks vengeance on the team of assassins who betrayed her.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1021, 'Mean Girls', 'Comedy', '2004-04-30', 47, 9.99, 'PG-13',"
                    "'Cady Heron is a hit with The Plastics, the A-list girl clique at her new school, until she makes the mistake of falling for Aaron Samuels, the ex-boyfriend of alpha Plastic Regina George.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1022, 'Saw', 'Horror', '2004-10-29', 81, 8.33, 'R',"
                    "'Two strangers, who awaken in a room with no recollection of how they got there, soon discover they''re pawns in a deadly game perpetrated by a notorious serial killer.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1023, 'V for Vendetta', 'Action', '2006-03-17', 98, 5.99, 'R',"
                    "'In a future British tyranny, a shadowy freedom fighter, known only by the alias of \"V\", plots to overthrow it with the help of a young woman.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1024, 'WALL-E', 'Animation', '2008-06-27', 79, 24.99, 'G',"
                    "'In the distant future, a small waste-collecting robot inadvertently embarks on a space journey that will ultimately decide the fate of mankind.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1025, 'The Dark Knight', 'Action', '2008-07-18', 53, 14.97, 'PG-13',"
                    "'When the menace known as the Joker wreaks havoc and chaos on the people of Gotham, Batman must accept one of the greatest psychological and physical tests of his ability to fight injustice.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1026, 'Avatar', 'Sci-Fi', '2009-12-18', 65, 17.96, 'PG-13',"
                    "'A paraplegic Marine dispatched to the moon Pandora on a unique mission becomes torn between following his orders and protecting the world he feels is his home.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1027, 'Inception', 'Action', '2010-07-16', 61, 6.92, 'PG-13',"
                    "'A thief who steals corporate secrets through the use of dream-sharing technology is given the inverse task of planting an idea into the mind of a C.E.O.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1028, 'We''re the Millers', 'Comedy', '2013-08-07', 61, 10.15, 'R',"
                    "'A veteran pot dealer creates a fake family as part of his plan to move a huge shipment of weed into the U.S. from Mexico.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1029, 'American Hustle', 'Crime', '2013-12-20', 87, 5.99, 'R',"
                    "'A con man, Irving Rosenfeld, along with his seductive partner Sydney Prosser, is forced to work for a wild F.B.I. Agent, Richie DiMaso, who pushes them into a world of Jersey powerbrokers and the Mafia.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1030, 'Get Out', 'Horror', '2017-02-24', 32, 14.16, 'R',"
                    "'A young African-American visits his white girlfriend''s parents for the weekend, where his simmering uneasiness about their reception of him eventually reaches a boiling point.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1031, 'It', 'Horror', '2017-09-08', 40, 14.96, 'R',"
                    "'In the summer of 1989, a group of bullied kids band together to destroy a shape-shifting monster, which disguises itself as a clown and preys on the children of Derry, their small Maine town.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1032, 'Your Name', 'Animation', '2017-04-07', 73, 14.16, 'PG',"
                    "'Two strangers find themselves linked in a bizarre way. When a connection forms, will distance be the only thing to keep them apart?');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1033, 'Avengers: Infinity War', 'Action', '2018-04-27', 28, 14.99, 'PG-13',"
                    "'The Avengers and their allies must be willing to sacrifice all in an attempt to defeat the powerful Thanos before his blitz of devastation and ruin puts an end to the universe.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1034, 'Spider-Man: Into the Spider-Verse', 'Animation', '2018-12-14', 17, 10.22, 'PG',"
                    "'Teen Miles Morales becomes Spider-Man of his reality, crossing his path with five counterparts from other dimensions to stop a threat for all realities.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1035, 'Avengers: Endgame', 'Action', '2019-04-26', 13, 19.99, 'PG-13',"
                    "'After the devastating events of Avengers: Infinity War (2018), the universe is in ruins. With the help of remaining allies, the Avengers assemble " 
                    "once more in order to reverse Thanos'' actions and restore balance to the universe.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1036, 'Once Upon a Time... in Hollywood', 'Comedy', '2019-07-26', 36, 15.00, 'R',"
                    "'A faded television actor and his stunt double strive to achieve fame and success in the film industry during the final years of Hollywood''s Golden Age in 1969 Los Angeles.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1037, 'Joker', 'Drama', '2019-10-4', 1, 14.96, 'R',"
                    "'In Gotham City, mentally troubled comedian Arthur Fleck is disregarded and mistreated by society. He then embarks on a downward spiral of " 
                    "revolution and bloody crime. This path brings him face-to-face with his alter-ego: the Joker.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1038, 'Parasite', 'Comedy', '2019-11-08', 0, 18.99, 'R',"
                    "'A poor family, the Kims, con their way into becoming the servants of a rich family, the Parks. But their easy life gets complicated when " 
                    "their deception is threatened with exposure.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1039, 'Frozen II', 'Animation', '2019-11-22', 4, 19.96, 'PG',"
                    "'Anna, Elsa, Kristoff, Olaf and Sven leave Arendelle to travel to an ancient, autumn-bound forest of an enchanted land. They set out to find " 
                    "the origin of Elsa''s powers in order to save their kingdom.');"
                "INSERT INTO Movies(MID, Title, Genre, Year, Qty, Price, Rating, Des)"
                    "VALUES(1040, 'Knives Out', 'Mystery', '2019-11-27', 1, 19.96, 'PG-13',"
                    "'A detective investigates the death of a patriarch of an eccentric, combative family.');";
        
        work W3(C); // Create a transactional object
        W3.exec(insert);
        W3.commit();

        // Create SQL statements to insert information into the orders table
        insert = "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11001, 107, '2008-04-11', '2008-04-14', '417 12th St NW Albuquerque, NM 87101', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11002, 105, '2009-10-29', '2009-11-01', '9518 Dunder St Scranton, PA 18503', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11003, 105, '2010-01-07', '2010-01-10', '9518 Dunder St Scranton, PA 18503', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11004, 106, '2010-10-23', '2010-10-27', '6789 Schrute Lane Scranton, PA 18503', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11005, 108, '2011-02-23', '2011-02-26', '3958 28th St SW Albuquerque, NM 87103', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11006, 105, '2011-04-15', '2011-04-18', '9518 Dunder St Scranton, PA 18503', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11007, 109, '2012-05-02', '2012-05-05', '193 Sunset Blvd Miami, FL 33101', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11008, 109, '2012-12-14', '2012-12-17', '193 Sunset Blvd Miami, FL 33101', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11009, 108, '2013-11-05', '2013-11-08', '3958 28th St SW Albuquerque, NM 87103', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11010, 108, '2013-12-10', '2013-12-13', '3958 28th St SW Albuquerque, NM 87103', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11011, 105, '2014-02-25', '2014-02-28', '9518 Dunder St Scranton, PA 18503', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11012, 110, '2014-07-18', '2014-07-21', '2037 Madison Ave Manhattan, NY 10021', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11013, 114, '2014-08-20', '2014-08-23', '710 James St Dallas, TX 75063', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11014, 110, '2014-12-12', '2014-12-15', '2037 Madison Ave Manhattan, NY 10021', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11015, 111, '2015-12-01', '2015-12-04', '4031 Pine St Seattle, WA 98101', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11016, 111, '2016-01-08', '2016-01-11', '4031 Pine St Seattle, WA 98101', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11017, 114, '2016-08-06', '2016-08-09', '710 James St Dallas, TX 75063', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11018, 111, '2016-09-19', '2016-09-22', '4031 Pine St Seattle, WA 98101', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11019, 112, '2017-10-01', '2017-10-04', '526 Grace St Tampa, FL 33201', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11020, 115, '2017-12-17', '2017-12-20', '9471 Waters Ave Chicago, IL 60624', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11021, 104, '2018-06-03', '2018-06-06', '1567 Avenger Ave Queens, NY 73290', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11022, 101, '2018-08-31', '2018-09-03', '123 Web Lane Tampa, FL 33612', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11023, 102, '2019-03-02', '2019-03-05', '456 Holly Ave Portland, OR 17892', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11024, 102, '2019-07-26', '2019-07-29', '456 Holly Ave Portland, OR 17892', 'Delivered');"
                "INSERT INTO Orders(OID, CID, Received, Shipped, Address, Status)"
                    "VALUES(11025, 113, '2019-11-14', '2019-11-17', '1895 Cactus Ave Phoenix, AZ 85009', 'Delivered');";

        work W4(C); // Create a transactional object
        W4.exec(insert);
        W4.commit();

        insert = "INSERT INTO Admins(AID, Email, Password, AName, DOB)"
                    "VALUES (1110, 'mikam@usf.edu', 'Admin1!', 'Mika Morrisson', '1998-02-27');"
                 "INSERT INTO Admins(AID, Email, Password, AName, DOB)"
                    "VALUES (1111, 'justinr@usf.edu', 'Admin2!', 'Justin Rodney', '1998-02-27');";
        
        work W5(C); // Create a transactional object
        W5.exec(insert);
        W5.commit();

        insert = "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(107, 1004, 11001, 1, 14.96);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(105, 1010, 11002, 1, 10.01);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(105, 1016, 11002, 1, 19.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(105, 1021, 11003, 1, 9.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(106, 1020, 11004, 1, 9.50);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(106, 1016, 11004, 1, 19.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(108, 1020, 11005, 1, 9.50);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(105, 1024, 11006, 1, 24.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(109, 1020, 11007, 1, 9.50);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(109, 1008, 11007, 1, 12.77);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(109, 1017, 11008, 1, 9.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(108, 1025, 11009, 1, 14.97);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(108, 1011, 11010, 1, 19.07);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(105, 1026, 11011, 1, 17.96);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(110, 1021, 11012, 1, 9.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(110, 1004, 11012, 1, 14.96);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(114, 1005, 11013, 1, 10.80);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(114, 1007, 11013, 1, 8.39);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(110, 1015, 11014, 1, 15.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(111, 1018, 11015, 1, 15.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(111, 1028, 11015, 1, 10.97);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(111, 1013, 11015, 1, 12.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(111, 1026, 11016, 1, 17.96);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(114, 1026, 11017, 1, 17.96);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(111, 1011, 11018, 1, 19.07);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(112, 1030, 11019, 1, 15.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(112, 1028, 11019, 1, 10.15);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(115, 1012, 11020, 1, 15.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(104, 1031, 11021, 1, 14.96);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(104, 1032, 11021, 1, 14.16);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(101, 1030, 11022, 1, 14.16);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(101, 1027, 11022, 1, 6.92);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(101, 1023, 11022, 2, 5.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(102, 1033, 11023, 1, 14.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(102, 1034, 11023, 1, 11.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(102, 1019, 11024, 1, 14.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(102, 1014, 11024, 1, 14.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(113, 1014, 11025, 1, 14.99);"
                 "INSERT INTO Cart(CID, MID, OID, Qty, Purchased) VALUES(113, 1021, 11025, 1, 9.99);";

        work W6(C); // Create a transactional object
        W6.exec(insert);
        W6.commit();

        insert = "CREATE OR REPLACE FUNCTION updateMovCart() RETURNS TRIGGER AS $$"
                    "BEGIN"
                        " UPDATE movies SET qty= movies.qty - lastOrd.qty FROM"
                        "(SELECT mid, qty, cid FROM (SELECT mid,qty,cid FROM cart WHERE oid = -1) as cart2 NATURAL JOIN"
                        " (SELECT * FROM orders ORDER BY oid DESC LIMIT 1) as t) AS lastOrd WHERE movies.mid =lastOrd.mid;"

                        " UPDATE cart SET purchased = cart.qty*lastOrd.Price FROM "
                        "(SELECT mid,cid,oid,price FROM (SELECT mid,cid,price FROM cart  "
                        "NATURAL JOIN (SELECT mid, price FROM movies) as movies "
                        "WHERE oid = -1) as cart2 NATURAL JOIN " 
                        "(SELECT * FROM orders ORDER BY oid DESC LIMIT 1) as t) AS lastOrd "
                        "WHERE cart.cid =lastOrd.cid AND cart.oid = -1;"

                        " UPDATE cart SET OID=lastOrd.oid FROM (SELECT * FROM orders ORDER BY oid DESC LIMIT 1) as lastOrd WHERE"
                        " cart.cid=lastOrd.cid AND cart.oid =-1;"
                        "RETURN NULL;"
                    " END;"
                    "$$ LANGUAGE PLPGSQL;"
                    "CREATE TRIGGER ex_order"
                    " AFTER INSERT ON orders"
                    " EXECUTE PROCEDURE updateMovCart();";
        
        work W7(C); // Create a transactional object
        W7.exec(insert);
        W7.commit();

        C.disconnect(); // Disconnect from the database

    }
    catch(const exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
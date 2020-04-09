all: driver populate

driver: store.cpp driver.cpp
	g++ -std=c++17 driver.cpp store.cpp -lpqxx -lpq -o driver
	
populate: populate.cpp
	g++ -std=c++17 populate.cpp -lpqxx -lpq -o populate

clean:
	rm populate driver